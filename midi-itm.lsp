(defparameter *function-table* (make-hash-table))
(defparameter *active-table* (make-hash-table :test #'equal))
(defparameter *output-binding-table* (make-hash-table))
(defparameter *bpm* 180)

(defun bind (binds)
  (loop for n in binds do
       (setf (gethash (first n) *output-binding-table*) (list (second n) (third n)))))

(defun send-midi (channel note value)
  (format t "~a, ~a, ~a~%" channel note value))
;(defun send-midi (channel note value)
;  (send_midi channel note value))
(defun look-up-control (name)
  (gethash name *output-binding-table*))
(defun store-function (name key new-function)
  (format t "~%~a~%" name)
  (setf (gethash name *function-table*) new-function)
  (setf (gethash key *active-table*) new-function)
  key)
(defmacro pass-list (&body body)
  `(apply #'list ',@body))
(defmacro midi-function (name midi-bind &body body)
  `(store-function ',name (pass-list ,midi-bind)
     (defun ,name (&optional (value 0))
       ,@body)))
;;;___________________________________
(defmacro simple (name midi-bind)
  "make new passthrough list and store in function table with midi-bind as key"
  (let ((bind-list (look-up-control name)))
    (if bind-list
	`(midi-function ,name ,midi-bind
	   (send-midi ,@bind-list value))
	(format t "ERROR: No binding found for ~a~%" name))))
;;;___________________________________
(defmacro multi (name midi-bind &body body)
  "bind multiple function calls with predefined values to the same midi-bind"
  `(midi-function ,name ,midi-bind
     ,@body value))
;;;___________________________________
(defmacro rebind (name midi-bind &body body)
  "rebinds values in the active-table with values from the function-table"
  `(midi-function ,name ,midi-bind
     ,@(loop for n in body collect
	    `(setf (gethash (list ,(first n) ,(second n)) *active-table*)
		   (gethash ',(third n) *function-table*))) value))
;;;___________________________________
(defmacro auto (name midi-bind &body body)
  "creates automation using start, end, and bar duration values"
     (let ((automation-values (first body)))
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
	     `(midi-function ,name ,midi-bind
		(sb-thread:make-thread (lambda ()
					 (do ((i ,start (+ i ,iterator)))
					     ((eql i ,end))
					   (send-midi ,@(look-up-control fn) i)
					   (sleep ,wait-time)))) value))))))

(defun route (channel note &optional (value 0))
  (funcall (gethash (list channel note) *active-table*) value))
