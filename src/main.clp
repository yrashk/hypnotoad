(defmodule Hypnotoad)

(defrule load-constructs
  (require ?filename)
  (not (remote-file ?filename ?))
  => 
  (remote-retrieve-file ?filename))

(defrule load-facts 
  (include ?filename) 
  (not (remote-file ?filename ?))
  => 
  (remote-retrieve-file ?filename))

(defrule load-contructs-ready
  ?fact <- (require ?filename)
  (remote-file ?filename ?local-filename)
  =>
  (load ?local-filename)
  (retract ?fact))

(defrule load-facts-ready
  ?fact <- (include ?filename)
  (remote-file ?filename ?local-filename)
  =>
  (load-facts ?local-filename)
  (retract ?fact))
