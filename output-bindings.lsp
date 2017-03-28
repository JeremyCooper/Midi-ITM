(defmacro output-bindings (&body binds)
  `(progn
     ,@(loop for n in binds do
	    (setf (gethash (first n) *output-binding-table*) (list (second n) (third n)))
	  collect
	    `(pass ,(first n) (0 0)))))

(output-bindings
 (red 144 1)
 (green 176 2)
 (blue 144 3)
 (layer-select-0 144 0)
 (layer-select-1 144 1)
 (layer-select-2 144 2)
 (layer-select-3 144 3)
 (layer-select-4 144 4)
 (layer-select-5 144 5)
 (layer-select-6 144 6)
 (layer-select-7 144 7)
 (layer-opacity-0 176 0)
 (layer-opacity-1 176 1)
 (layer-opacity-2 176 2)
 (layer-opacity-3 176 3)
 (layer-trace-activate-0 144 8)
 (layer-trace-0 176 4)
 (layer-trace-activate-1 144 9)
 (layer-trace-1 176 5)
 (layer-trace-activate-2 144 10)
 (layer-trace-2 176 6)
 (layer-trace-activate-3 144 11)
 (layer-trace-3 176 7)
 (layer-shake-activate-0 144 12)
 (layer-shake-0 176 8)
 (layer-shake-activate-1 144 13)
 (layer-shake-1 176 9)
 (layer-shake-activate-2 144 14)
 (layer-shake-2 177 10)
 (layer-shake-activate-3 144 15)
 (layer-shake-3 178 11))
