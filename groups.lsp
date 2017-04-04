(defmacro gen-group-send (name send-list)
  `(defun ,name (value)
     ,@(loop for n in send-list collect
	    `(send-feedback 144 ,n value))))

(defmacro groups (&body groups)
  `(progn
     ,@(loop for n in groups collect
	    `(gen-group-send ,(first n) ,(second n)))))

(groups
  (group-effects-0 (12 20 28 36))
  (group-effects-1 (13 21 29 37))
  (group-effects-2 (14 22 30 38))
  (group-effects-3 (15 23 31 39)))
