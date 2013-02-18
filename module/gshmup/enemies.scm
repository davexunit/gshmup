(define-module (gshmup enemies)
  #:use-module (gshmup core)
  #:export (enemy-test))

(define enemy-test
  (make-enemy-type "Testing"
                   "data/sprites/enemy_1.sheet"
                   "idle"
                   60
                   (make-rect -16 -16 32 32)
                   (lambda () (test-on-death))))

(define (test-on-death)
  (end-stage))
