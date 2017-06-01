:normalize t ;;0-127 => 0-1
:printvalue t ;;print value at end of function
:default-color 140 ;;idle color
:press-color 20 ;;keydown color
:toggle-off-color 100 ;;color of toggle button in "off" state
:knob-behavior :fill ;;for the apc40s alternate knob modes
:on-value 127 ;;value to send on keydown
:off-value 1) ;;value to send on keyup

(defparameter *control-defaults* (make-hash-table :test #'equal))
(defparameter *function-table* (make-hash-table))
(defparameter c-tag1 nil)
(defparameter c-tag2 nil)
(defun init-defaults ()
  (setf (gethash "knob" *control-defaults*)
	'((tag1 140) (tag2 50) (tag3 1) (tag4 t) (tag5 nil))))
(defmacro setup-function (form)
  ;;append functionality to the beginning and end of `form
  ;;according to the configurations chosen :normalize, :on-value, :color, etc
  `(format t "~a~a~%" tag1 tag2))
(defmacro with-custom-settings (&body body)
  `(let ((tag1 (if c-tag1 c-tag1 tag1))
	 (tag2 (if c-tag2 c-tag2 tag2)))
	  ,@body))
(defmacro knob (name form &optional channel note)
  `(let ,(gethash "knob" *control-defaults*)
     ;;are custom settings bound?
     ;;if so, override lets with
     ;;the used custom settings
     (with-custom-settings
       (if (not ',name)
	   ;;don't register in function-table
	   ;;effectively pointless but leaving here
	   ;;for testing purposes. Could be used to
	   ;;add initial control functionality if
	   ;;note and channel are supplied. If bound
	   ;;over they are lost.
	   (setf (gethash (list ,channel ,note) *active-table*)
		 (lambda (value)
		   (setup-function ,form)))
	   ;;else, obviously add to function table
	   ;;and subsequently add to active-table if
	   ;;the channel and note have been provided
	   (if (and ,channel ,note)
	       (midi-function ,name (,channel ,note)
		 (setup-function ,form))
	       (midi-function ,name (0 0)
		 (setup-function ,form)))))))

;;(defmacro control-settings (options form)
;;  (dotimes (i (length options))

;;is there a way to copy just a reference to the function-table value
;;in the active-table efficiently? ie. not strings
(knob
  "EffectFader1"
  144 7
  (progn
	(print 100)
	(dotimes (i 5)
	  (format "~a~%" i))))
(knob
  "EffectFader1"
  nil nil
  (progn
	(print 100)
	(dotimes (i 5)
	  (format "~a~%" i))))
(knob
  "EffectFader1"
  (progn
	(print 100)
	;;set of control logic modifying macros such as
	;;toggle, piano, etc
	(toggle
	  (print "note is on")
	  (print "note is off"))
	(dotimes (i 5)
	  (format "~a~%" i)))
  144 7)
;;at some point maybe it would be nice have a post-initialization
;;function that copies function-table values into the active-table
;;and is configured via simple text file of name:(channel, note) pairs

	;;control-settings will go through all the passed
	;;keywords and wrap a let around the form. preventing
	;;the control macro (knob, in this case), from initializing
	;;it's own control-specific configurations
(control-settings ;;configuration macro
  (:normalize t
   :printvalue t
   :note-off-threshold 20
   :default-color 140
   :press-color 20			;;colors can be passed vars
   :toggle-off-color 100	;;this will help enable
   :knob-behavior :fill		;;switchable colorschemes
   :on-value 127
   :off-value 1)
  (knob
	"EffectFader1"
	(progn
	  (print 100)
	  (dotimes (i 5)
		(format "~a~%" i)))
	(144 127 81 72 48)		;;if lists are passed to channel
	(7 3 4 2 1)))			;;and note params, perform a bind_add()

  ;;bind_add() will stack functions onto channel/note keys in the
  ;;function-table. triggering the function-table value will
  ;;execute all functions stored in the same channel/note key
  ;;in sequence ;;gonna need to wrap up function-table calls
