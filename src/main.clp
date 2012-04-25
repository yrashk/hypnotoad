(defmodule Hypnotoad)

(defrule load-constructs ?fact <- (require ?filename) 
         => 
         (remote-retrieve-file ?filename)
         (assert (require-ready ?filename))
         (retract ?fact))

(defrule load-facts ?fact <- (include ?filename) 
         => 
         (remote-retrieve-file ?filename)
         (assert (include-ready ?filename))
         (retract ?fact))

(defrule load-contructs-ready
          ?fact <- (require-ready ?filename)
          (remote-file ?filename ?local-filename)
          =>
          (load ?local-filename)
          (retract ?fact))

(defrule load-facts-ready
          ?fact <- (include-ready ?filename)
          (remote-file ?filename ?local-filename)
          =>
          (load-facts ?local-filename)
          (retract ?fact))
