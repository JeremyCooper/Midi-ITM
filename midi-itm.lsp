(require 'ecl-quicklisp)
(ql:quickload 'bordeaux-threads)
(defparameter *function-table* (make-hash-table))
(defparameter *active-table* (make-hash-table :test #'equal))
(defparameter *output-binding-table* (make-hash-table))
(defparameter *bpm* 180)

(defun bind (binds)
  (loop for n in binds do
       (setf (gethash (first n) *output-binding-table*) (list (second n) (third n)))))

;(defun send-feedback (channel note value)
 ; (format t "feedback: ~a, ~a, ~a~%" channel note value))
(defun send-feedback (channel note value)
  (send_feedback channel note value))
;(defun send-midi (channel note value)
 ; (format t "~a, ~a, ~a~%" channel note value))
(defun send-midi (channel note value)
  (send_midi channel note value))
(defun look-up-control (name)
  (gethash name *output-binding-table*))
(defun store-function (name key new-function)
  (format t "~%Creating and storing: ~a~%" name)
  (setf (gethash name *function-table*) new-function)
  ;;make sure new-function is only evaluated once. vvvv put (gethash name fucntiontable here)
  (setf (gethash key *active-table*) new-function))
(defmacro midi-function (name midi-bind &body body)
  (let ((midi-info (cons 'list midi-bind)))
    `(store-function ',name ,midi-info
		     (defun ,name (&optional (value 0))
		       ,@body))))

(defun rgb->color (rgb)
    (let ((red (first rgb)) (green (second rgb)) (blue (third rgb)))
      red))
(defmacro feedback (channel note value)
    `(send-feedback ,channel ,note ,(rgb->color value)))
    
;;;___________________________________
(defmacro simple (name midi-bind &body body)
  "make new passthrough list and store in function table with midi-bind as key"
  (let ((bind-list (look-up-control name)))
    (if bind-list
	`(midi-function ,name ,midi-bind
	   (send-midi ,@bind-list value)
	   ,@body)
	(format t "ERROR: No binding found for ~a~%" name))))
;;;___________________________________
(defmacro multi (name midi-bind &body body)
  "bind multiple function calls with predefined values to the same midi-bind"
  `(midi-function ,name ,midi-bind
     ;;redo so that multi expands to send-midi calls instead of
     ;;direct function calls. So that the additional forms don't
     ;;get evaluated
     ,@(loop for n in body collect
	    `(send-midi ,@(look-up-control (first n)) ,(second n))) value))
;;;___________________________________
(defmacro rebind (name midi-bind &body body)
  "rebinds values in the active-table with values from the function-table"
  `(midi-function ,name ,midi-bind
     ,@(loop for n in body collect
	    `(setf (gethash (list ,(first n) ,(second n)) *active-table*)
		   (gethash ',(third n) *function-table*))) value))
;;;___________________________________
(defmacro auto (name midi-bind automation-values &body body)
  "creates automation using start, end, and bar duration values"
  ;(let ((automation-values (first raw-values))
    (let ((start (first automation-values))
	  (end (second automation-values))
	  (bars (third automation-values))
	  (fn (fourth automation-values))
	  (iterator 0))
      (if (> start end)
	  (setf iterator -1)
	  (setf iterator 1))
      (let ((distance (abs (- start end)))
	    (length (* (* 4 bars) (/ *bpm* 60))))
	(let ((wait-time (/ (/ 60 length) distance)))
	  ;;before auto defun is generated, create new mutex 'name
	  ;;(bordeaux-threads::make-lock(name))
	  `(midi-function ,name ,midi-bind
	     ;;determine wait-time here. so subsequent triggers re-quantize
	     ;;to current bpm
	     ;;this will require restructuring the algebra so that bpm
	     ;;is the last variable needed
	     
	     ;;aquire lock 'name (wait-p NIL)
	     (bordeaux-threads::make-thread
	      (lambda ()
		(do ((i ,start (+ i ,iterator)))
		    ((eql i ,end))
		    (send-feedback ,@(look-up-control fn) i)
		  (sleep ,wait-time))))
	      ,@body)))))


(defun route (channel note &optional (value 0))
  (let ((fn (gethash (list channel note) *active-table*)))
    (if fn
	(funcall (gethash (list channel note) *active-table*) value))))
