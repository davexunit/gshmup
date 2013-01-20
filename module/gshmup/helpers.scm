(define-module (gshmup helpers)
  #:export (repeat))

(define (repeat times proc)
  "Do something a bunch of times."
  (do ((i 1 (1+ i)))
       ((> i times))
    (proc i)))
