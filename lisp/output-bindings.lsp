(ql:quickload 'cl-ppcre)
(defmacro output-bindings (&body binds)
  `(progn
     ,@(loop for n in binds do
	    (setf (gethash (first n) *output-binding-table*) (list (second n) (third n)))
	  collect
	    `(pass ,(first n) (0 0)))))

(defmacro gen-clip-bindings (template-name amount &key (channel 145) (start-value 0))
  `(output-bindings
     ,@(loop for n upto amount
	  collect (list (intern (cl-ppcre:regex-replace-all #\* template-name (write-to-string n)))
			channel
			(+ start-value n)))))

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
 (layer-trace-activate-0 144 24)
 (layer-trace-0 144 0)
 (layer-trace-activate-1 144 25)
 (layer-trace-1 144 1)
 (layer-trace-activate-2 144 26)
 (layer-trace-2 144 2)
 (layer-trace-activate-3 144 27)
 (layer-trace-3 144 3)
 (layer-shake-activate-0 144 16)
 (layer-shake-0 144 8)
 (layer-shake-activate-1 144 17)
 (layer-shake-1 144 9)
 (layer-shake-activate-2 144 18)
 (layer-shake-2 144 10)
 (layer-shake-activate-3 144 19)
 (layer-shake-3 144 11))

(gen-clip-bindings "CLIP-*-0" 20 :channel 145)
;;(output-bindings
;;  (CLIP-1-0 145 (0-19))
;;  (etc))
(gen-clip-bindings "CLIP-*-1" 20 :channel 146)
(gen-clip-bindings "CLIP-*-2" 20 :channel 147)
(gen-clip-bindings "CLIP-*-3" 20 :channel 148)

(gen-clip-bindings "SELECT-CLIP-*-0" 20 :channel 145 :start-value 20)
(gen-clip-bindings "SELECT-CLIP-*-1" 20 :channel 146 :start-value 20)
(gen-clip-bindings "SELECT-CLIP-*-2" 20 :channel 147 :start-value 20)
(gen-clip-bindings "SELECT-CLIP-*-3" 20 :channel 148 :start-value 20)
