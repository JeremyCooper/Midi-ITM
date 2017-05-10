(defmacro knob (name form &optional channel note)
  (if (not name)
	;;don't register in function-table
	;;effectively pointless but leaving here
	;;for testing purposes. Could be used to
	;;add initial control functionality if
	;;note and channel are supplied. If bound
	;;over they are lost.
	t
	;;else, obviously add to function table
	;;and subsequently add to active-table if
	;;the channel and note have been provided
	form)
  (setf (gethash name *value-table*) value))
;;is there a way to copy just a reference to the function-table value
;;in the active-table efficiently? ie. not strings
(knob
  "EffectFader1"
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
