(defmodule Hypnotoad)

(defrule load-constructs (require ?filename) 
         => 
         (load (remote-retrieve-file ?filename)))

(defrule load-facts (include ?filename) 
         => 
         (load-facts (remote-retrieve-file ?filename)))
