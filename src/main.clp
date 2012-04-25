(defmodule Hypnotoad)

(defrule load-constructs ?fact <- (require ?filename) 
         => 
         (load (remote-retrieve-file ?filename))
         (retract ?fact))

(defrule load-facts ?fact <- (include ?filename) 
         => 
         (load-facts (remote-retrieve-file ?filename))
         (retract ?fact))
