(define-module (gshmup helpers)
  #:use-module (gshmup core)
  #:export (repeat
            angle-to-player))

(define (repeat times proc)
  "Do something a bunch of times."
  (do ((i 1 (1+ i)))
       ((> i times))
    (proc i)))

(define (angle-to-player)
  "Compute angle in degrees from the current entity to the player."
  (vector2-angle (vector2-sub (player-position) (entity-position))))
