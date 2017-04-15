(require 'ecl-quicklisp)
(ql:quickload 'bordeaux-threads)
(defparameter *mode* 0)
(defparameter *function-table* (make-hash-table))
(defparameter *active-table* (make-hash-table :test #'equal))
(defparameter *output-binding-table* (make-hash-table))
(defparameter *groups-table* (make-hash-table))
(defparameter *threads* (make-hash-table))
(defparameter *bpm* 180)
(defparameter *last-function* 'red)
(defparameter *current-layer* 0)
(defparameter *layer* "")

(defun send-feedback (channel note value)
  (format t "feedback: ~a, ~a, ~a~%" channel note value))
(defun send-feedback (channel note value)
  (send_feedback channel note value))
(defun send-midi (channel note value)
  (format t "~a, ~a, ~a~%" channel note value))
(defun send-midi (channel note value)
  (send_midi channel note value))
(defun look-up-control (name)
 ; (if (functionp (function name))
      (gethash name *output-binding-table*))
 ;     (gethash (eval name) *output-binding-table*)))
(defun store-function (name key new-function)
  (format t "~%Creating and storing: ~a~%" name)
  (setf (gethash name *function-table*) new-function)
  (setf (gethash key *active-table*) (gethash name *function-table*)))
(defmacro midi-function (name midi-bind &body body)
  (let ((midi-info (cons 'list midi-bind)))
    (let ((name-string (symbol-name name)))
      (if (> (length name-string) 4)
	  (if (string= name-string "LAYER" :end1 5 :end2 5)
	      (setf name-string (char name-string (- (length name-string) 1)))
	      (setf name-string "")))
      `(store-function ',name ,midi-info
		       (defun ,name (&optional (value 0))
			 (let ((*layer* ,name-string))
			   ,@body))))))

(defun rgb->color (rgb)
    (let ((red (first rgb)) (green (second rgb)) (blue (third rgb)))
      (+ red green blue)))
(defmacro feedback (channel note value)
    `(send-feedback ,channel ,note ,(rgb->color value)))

(defmacro gen-variants (variants &body body)
  `(progn
     ,@(loop for n in variants collect
	    (let ((z nil))
	      (read-from-string
	       (with-output-to-string (s)
		 (map 'nil
		      #'(lambda (x)
			  (if (char= #\^ x)
			      (setf z t)
			      (if z
				  (progn
				    (princ (nth (digit-char-p x) n) s)
				    (setf z nil))
				  (princ x s))))
		      (write-to-string (first body) :escape nil))))))))

;;;___________________________________
(defmacro simple (name midi-bind &body body)
  `(midi-function ,name ,midi-bind
     ,@body value))
;;;___________________________________
(defmacro pass (name midi-bind &body body)
  "make new passthrough list and store in function table with midi-bind as key"
  (let ((bind-list (look-up-control name)))
    (if bind-list
	`(midi-function ,name ,midi-bind
	   (send-feedback ,@bind-list value)
	   ,@body)
	(format t "ERROR: No binding found for ~a~%" name))))
;;;___________________________________
(defmacro piano (name midi-bind on-form off-form &body body)
  `(midi-function ,name ,midi-bind
     ,on-form ,@body value)
  (let ((off-midi-bind (list (first midi-bind) (- 16 (second midi-bind))))
	(off-name (intern (concatenate 'string (symbol-name name) "-OFF"))))
    `(midi-function ,off-name ,off-midi-bind
       ,off-form ,@body value)))

;;;___________________________________
(defmacro toggle (name midi-bind on-form off-form &body body)
    `(midi-function ,name ,midi-bind
       (if x
	    (progn
	      ,off-form
	      (setq x nil))
	    (progn
	      ,on-form
	      (setq x t)))
       ,@body value))
;;;___________________________________
(defmacro multi (name midi-bind &body body)
  "bind multiple function calls with predefined values to the same midi-bind"
  `(midi-function ,name ,midi-bind
     ,@(loop for n in body collect

	    `(send-midi ,@(look-up-control (first n)) ,(second n)))
     value))
;;;___________________________________
(defmacro rebind (name midi-bind &body body)
  "rebinds values in the active-table with values from the function-table"
  `(midi-function ,name ,midi-bind
     ,@(loop for n in body collect
	    `(setf (gethash (list ,(first n) ,(second n)) *active-table*)
		   (gethash ',(third n) *function-table*)))
     value))
;;;___________________________________
(defmacro auto (name midi-bind automation-values &body body)
  "creates automation using start, end, and bar duration values"
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
	 `(let ((control-values (look-up-control ,fn)))
	  (midi-function ,name ,midi-bind
	     ;;determine wait-time here. so subsequent triggers re-quantize
	     ;;to current bpm
	     ;;this will require restructuring the algebra so that bpm
	     ;;is the last variable needed

	     (let ((thread-id-G! (gensym)))
	       (setf (gethash ',name *threads*) thread-id-G!)
	       (format t "THRZZZZZZEAD ID: ~a~%" thread-id-G!)
	       (format t "FN: ~a~%" ,fn)
	       (format t "control: ~a~%" control-values)
	       (bordeaux-threads::make-thread
		(lambda ()
		  (do ((i ,start (+ i ,iterator)))
		      ((eql i ,end))
		  (if (not (eq thread-id-G!(gethash ',name *threads*)))
		      (progn
			(return)))
		    ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
		    (let* ;;((control-values (look-up-control fn))
			((feedback-values
			  (cons i (cons (cadr control-values) (cons (car control-values) nil))))
			   (feedback-values (nreverse feedback-values)))
		      (apply #'send-feedback feedback-values))
		    ;;(send-feedback (look-up-control ,fn) i)
		    ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
		    (sleep ,wait-time))))
	       ,@body value)))))))

(defun route (channel note &optional (value 0))
  (case *mode*
    (0 (basic channel note value))
    (1 (save-clip channel note))
    (2 (effect-auto channel note))
    (3 (fader-auto channel note))
    (4 (multi-trigger channel note value))))

(defun effect-auto (channel note)
  (let ((fn-str (symbol-name (gethash (list channel note) *active-table*))))
    (format t "~a~%" fn-str)
    (cond ((string= fn-str "AUTO-EFFECT-0")
	   (progn
	     (create-effect-auto-0)
	     (setf *mode* 0)))
	  ((string= fn-str "AUTO-EFFECT-1")
	   (progn
	     (create-effect-auto-1)
	     (setf *mode* 0)))
	  ((string= fn-str "AUTO-EFFECT-2")
	   (progn
	     (create-effect-auto-2)
	     (setf *mode* 0)))
	  ((string= fn-str "AUTO-EFFECT-3")
	   (progn
	     (create-effect-auto-3)
	     (setf *mode* 0))))))
(defun fader-auto (channel note)
  (let ((fn-str (symbol-name (gethash (list channel note) *active-table*))))
    (format t "~a~%" fn-str)
    (cond ((string= fn-str "AUTO-FADER-0")
	   (progn
	     (create-fader-auto-0)
	     (setf *mode* 0)))
	  ((string= fn-str "AUTO-FADER-1")
	   (progn
	     (create-fader-auto-1)
	     (setf *mode* 0)))
	  ((string= fn-str "AUTO-FADER-2")
	   (progn
	     (create-fader-auto-2)
	     (setf *mode* 0)))
	  ((string= fn-str "AUTO-FADER-3")
	   (progn
	     (create-fader-auto-3)
	     (setf *mode* 0))))))
(defun multi-trigger (channel note value)
  (let ((fn-str (symbol-name (gethash (list channel note) *active-table*))))
    (if (search "MODE-MULTI-TRIGGER" fn-str) 1 value)))
(defun save-clip (channel note)
  (let ((fn-str (symbol-name (gethash (list channel note) *active-table*))))
    (if (search "ACTIVATE-SAVED-CLIP-" fn-str)
	(let* ((clip-num (elt fn-str (1- (length fn-str))))
	       (saved-clip-name (concatenate 'string
					     "*SAVED-CLIP-"
					     (list clip-num)
					     "*")))
	  (print (setf (symbol-value (intern saved-clip-name)) (list *selected-clip-x* *selected-clip-y*)))
	  (setf *mode* 0)))))
(defun basic (channel note &optional (value 0))
  (let ((fn (gethash (list channel note) *active-table*)))
    (if fn
	(funcall fn value))
    (setf *last-function* fn)))
