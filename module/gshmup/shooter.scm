(define-module (gshmup shooter)
  #:use-module (gshmup core)
  #:use-module (gshmup math)
  #:use-module (gshmup bullet)
  #:use-module (gshmup coroutine)
  #:use-module (gshmup helpers)
  #:use-module (gshmup stages stage-1))

;; Bullet types
(register-bullet-type 'medium-blue
                      (make-bullet-type 0 (make-rect -4 -4 8 8) 'alpha #f kill-player))
(register-bullet-type 'small-diamond
                      (make-bullet-type 1 (make-rect -2 -2 4 4) 'alpha #t kill-player))
(register-bullet-type 'large-orange
                      (make-bullet-type 2 (make-rect -5 -5 10 10) 'alpha #f kill-player))
(register-bullet-type 'blue
                      (make-bullet-type 3 (make-rect -3 -3 6 6) 'alpha #t
                                        (lambda () (damage-enemy 1))))
(register-bullet-type 'bright
                      (make-bullet-type 4 (make-rect -8 -8 16 16) 'add #f kill-player))
(register-bullet-type 'fire
                      (make-bullet-type 5 (make-rect -2 -2 4 4) 'alpha #t kill-player))

;; Player shot
(set! *player-on-shoot* (lambda () (test-shot)))

(define-coroutine (test-shot)
  (when (player-shooting?)
    (let ((speed 10)
          (p (player-position)))
      (emit-bullet p speed 268 'blue)
      (emit-bullet p speed 270 'blue)
      (emit-bullet p speed 272 'blue)
      (emit-bullet p speed 210 'blue)
      (emit-bullet p speed 245 'blue)
      (emit-bullet p speed 295 'blue)
      (emit-bullet p speed 330 'blue))
    (wait 5)
    (test-shot)))

;; Stages
(set! *stages* (list stage-1
                     stage-1))
