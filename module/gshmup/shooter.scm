(define-module (gshmup shooter)
  #:use-module (gshmup core)
  #:use-module (gshmup math)
  #:use-module (gshmup coroutine)
  #:use-module (gshmup helpers)
  #:use-module (gshmup bullets)
  #:use-module (gshmup stages stage-1))

;; Player shot
(set! *player-on-shoot* (lambda () (test-shot)))

(define-coroutine (test-shot)
  (when (player-shooting?)
    (let ((speed 10)
          (p (player-position)))
      (emit-bullet bullet-blue p speed 268)
      (emit-bullet bullet-blue p speed 270)
      (emit-bullet bullet-blue p speed 272)
      (emit-bullet bullet-blue p speed 210)
      (emit-bullet bullet-blue p speed 245)
      (emit-bullet bullet-blue p speed 295)
      (emit-bullet bullet-blue p speed 330))
    (wait 5)
    (test-shot)))

;; Stages
(set! *stages* (list stage-1))
