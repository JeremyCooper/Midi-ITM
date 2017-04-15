(gen-variants ((55))
  (simple board-color (176 ^0)
    (loop for n in '(82 83 84 85 86) do
	 (send-feedback 144 n value))
    (dotimes (x 40) 
      (send-feedback 144 x value))))

(defvar *effects-bg* 7)
(defvar *effects-fg* 117)
(defvar *effects-highlight* 44)

(gen-variants ((0) (1) (2) (3))
  (defparameter *selected-effect-^0* 'layer-trace-^0))
(gen-variants ((0) (1) (2) (3))
  (defparameter *selected-fader-^0* 'layer-opacity-^0))
(gen-variants ((0) (1) (2) (3))
  (defparameter *saved-clip-^0* (list 0 0)))

(defparameter *selected-clip-x* 0)
(defparameter *selected-clip-y* 0)

(defun reset-layer-display ()
  (loop for x in '(148 149 150 151) do
       (send-feedback x 51 0)))
(defun reset-effect-display-0 ()
  (loop for x in '(20 28 36) do
       (send-feedback 144 x *effects-bg*)))
(defun reset-effect-display-1 ()
  (loop for x in '(21 29 37) do
       (send-feedback 144 x *effects-bg*)))
(defun reset-effect-display-2 ()
  (loop for x in '(22 30 38) do
       (send-feedback 144 x *effects-bg*)))
(defun reset-effect-display-3 ()
  (loop for x in '(23 31 39) do
       (send-feedback 144 x *effects-bg*)))

(defun string->cat-call (str &rest params)
  (loop for n in params do
       (setf str (concatenate 'string
			      (string-upcase str)
			      (write-to-string n))))
  (funcall (intern str)))

(defun select-new-clip (x y)
  (if (eql x -1)
      (if (eql *selected-clip-x* 0)
	  (return-from select-new-clip)))
  (if (eql y -1)
      (if (eql *selected-clip-y* 0)
	  (return-from select-new-clip)))
  (setf *selected-clip-x* (+ *selected-clip-x* x))
  (setf *selected-clip-y* (+ *selected-clip-y* y))
  (string->cat-call "clip-"
		    *selected-clip-x*
		     '-
		     *selected-clip-y*))

;;(defmacro gen-variants (variants &body body)
;;  `(progn
;;     ,@(loop for n in variants collect
;;	    (let ((z nil))
;;	      (read-from-string
;;	       (with-output-to-string (s)
;;		 (map 'nil
;;		      #'(lambda (x)
;;			  (if (char= #\^ x)
;;			      (setf z t)
;;			      (if z
;;				  (progn
;;				    (princ (nth (digit-char-p x) n) s)
;;				    (setf z nil))
;;				  (princ x s))))
;;		      (write-to-string (first body) :escape nil))))))))

(simple actually-select-clip (144 8)
  (string->cat-call "select-clip-"
		    *selected-clip-x*
		    '-
		    *selected-clip-y*))

;;82-86
(simple mode-fader-auto (144 86)
  (setf *mode* 3))
(simple mode-effect-auto (144 85)
  (send-feedback 144 82 *effects-bg*)
  (send-feedback 144 83 *effects-bg*)
  (send-feedback 144 84 *effects-bg*)
  (setf *mode* 2))
(simple mode-save-clip (144 10)
  (format t "MODEEEEEEEEEEEEEE")
  (setf *mode* 1))
;;(simple mode-multi-trigger)

;;activate-saved-clip-*
;;activate-saved-clip-0
(gen-variants ((0 16) (1 17) (2 18) (3 19))
  (simple activate-saved-clip-^0 (144 ^1)
    (send-feedback 144 0 100)
    (print *saved-clip-^0*)
    (string->cat-call '"select-clip-"
		      (first *saved-clip-^0*)
		      '-
		      (second *saved-clip-^0*))))

;;clip-select-*-*
;;clip-select-x-+
(gen-variants ((x + 2 1 0)
	       (x - 0 -1 0)
	       (y + 9 0 1)
	       (y - 1 0 -1))
  (simple clip-select-^0-^1 (144 ^2)
    (select-new-clip ^3 ^4)))

;;layer-*-*
;;layer-trace-0
(gen-variants ((trace 0 36)
	       (trace 1 37)
	       (trace 2 38)
	       (trace 3 39)
	       (shake 0 28)
	       (shake 1 29)
	       (shake 2 30)
	       (shake 3 31))
  (pass layer-^0-^1 (0 0)
    (if (< value 2)
	(progn
	  (layer-^0-activate-^1 0)
	  (send-feedback 144 ^2 *effects-fg*))
	(progn
	  (layer-^0-activate-^1 127)
	  (send-feedback 144 ^2 *effects-highlight*)))))

;;layer-effect*-set-*
;;layer-effect0-set-0
(gen-variants ((0 0 trace 52)
	       (1 0 trace 53)
	       (2 0 trace 54)
	       (3 0 trace 55)
	       (0 1 shake 52)
	       (1 1 shake 53)
	       (2 1 shake 54)
	       (3 1 shake 55))
  (rebind layer-effect^1-set-^0 (0 0)
    (176 ^3 layer-^2-^0)))

;;auto-effect-*
;;auto-effect-0
(gen-variants ((0 12) (1 13) (2 14) (3 15))
  (simple auto-effect-^0 (144 ^1)))
;;create-effect-auto-*
;;create-effect-auto-0
(gen-variants ((0 12)
	       (1 13)
	       (2 14)
	       (3 15))
  (simple create-effect-auto-^0 (0 0)
    (auto auto-effect-^0 (144 ^1) (0 127 1 *selected-effect-^0*))
    (format t \"Creating auto for ~a~%\" *selected-effect-^0*)))

(gen-variants ((0 4) (1 5) (2 6) (3 7))
  (simple auto-fader-^0 (144 ^1)))
;;create-fader-auto-*
;;create-fader-auto-0
(gen-variants ((0 4)
	       (1 5)
	       (2 6)
	       (3 7))
  (simple create-fader-auto-^0 (0 0)
    (auto auto-fader-^0 (144 ^1) (0 127 1 *selected-fader-^0*))
    (format t \"Creating auto for ~a~%\" *selected-fader-^0*)))

;;layer-*-effect-*
;;layer-0-effect-0
(gen-variants ((0 0 36 trace)
	       (1 0 37 trace)
	       (2 0 38 trace)
	       (3 0 39 trace)
	       (0 1 28 shake)
	       (1 1 29 shake)
	       (2 1 30 shake)
	       (3 1 31 shake))
  (simple layer-^0-effect-^1 (144 ^2)
    (layer-effect^1-set-^0)
    (setf *selected-effect-^0* 'layer-^3-^0)
    (reset-effect-display-^0)
    (send-feedback 144 ^2 44)))

;;layer-select-*
;;layer-select-0
(gen-variants ((0 148)
	       (1 149)
	       (2 150)
	       (3 151))
  (pass layer-select-^0 (^1 51)
    (setf *current-layer* ^0)
    (reset-layer-display)
    (send-feedback ^1 51 127)))

;;layer-opacity-*
;;layer-opacity-0
(gen-variants ((0 180 4)
	       (1 181 5)
	       (2 182 6)
	       (3 183 7))
  (pass layer-opacity-^0 (^1 7)
    (if (< value 10)
	(send-feedback 144 ^2 0)
	(send-feedback 144 ^2 23))))
