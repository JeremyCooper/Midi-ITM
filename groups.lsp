(defmacro gen-group-send (name send-list)
  `(defun ,name (value)
     ,@(loop for n in send-list collect
	    `(send-feedback 144 ,n value))))

(defmacro groups (&body groups)
  `(progn
     ,@(loop for n in groups collect
	    `(gen-group-send ,(first n) ,(second n)))))

(groups
  (layer-0-effects (12 20 28 36))
  (layer-1-effects (13 21 29 37))
  (layer-2-effects (14 22 30 38))
  (layer-3-effects (15 23 31 39)))
