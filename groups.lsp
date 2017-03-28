(defmacro gen-group-send (name send-list)
  (write-line "hmm")
  `(defun ,name (value)
     (loop for n in ,send-list collect
	    `(send-feedback 144 ,n value))))

(defun layer-0-effects (value)
  (send-feedback 144 12 value)
  (send-feedback 144 20 value)
  (send-feedback 144 28 value)
  (send-feedback 144 36 value))

(defmacro groups (groups)
  (loop for n in groups do
       ;(progn
	 ;(setf (gethash (first n) *groups-table*) (second n))
	 (gen-group-send #.`(@,(first n)) (second n))))

(defmacro groups (groups)
  `(defun call-group-defuns ()
     ,@(loop for n in groups do
	    (setf (gethash (first n) *groups-table*) (second n))
	  collect
	    `(loop for x in ,(second n) collect
		  `(defun ,(first n) ()
		     (send-feedback 144 ,x value))))))
;;(defun expand-group (group-list)
;;  (loop for n in group-list collect
;;       `(send-feedback 144 ,n value)))
;; (groups) will fill global *groups* table.
;; gen-group-resets will defun reset functions
(groups
 ((layer-0-effects (12 20 28 36))
   (layer-1-effects (13 21 29 37))
   (layer-2-effects (14 22 30 38))
   (layer-3-effects (15 23 31 39))))
