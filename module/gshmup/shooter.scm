(define-module (gshmup shooter)
  #:use-module (gshmup core)
  #:use-module (gshmup math)
  #:use-module (gshmup bullet)
  #:use-module (gshmup coroutine)
  #:use-module (gshmup helpers)
  #:export (init-shooter))

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

(add-hook! player-shoot-hook (lambda () (test-shot)))

(define-coroutine (test-shot)
  (when (player-shooting?)
    (let ((speed 10)
          (p (player-position)))
      (emit-bullet p speed 268 'blue)
      (emit-bullet p speed 270 'blue)
      (emit-bullet p speed 272 'blue)
      (emit-bullet p speed 180 'blue)
      (emit-bullet p speed 225 'blue)
      (emit-bullet p speed 315 'blue)
      (emit-bullet p speed 360 'blue))
    (wait 5)
    (test-shot)))

(add-hook! shooter-init-hook (lambda () (test-level)))

(define-coroutine (test-level)
  (spawn-test-enemy)
  (wait (* 60 10))
  (spawn-test-enemy))

(define (spawn-test-enemy)
  (spawn-enemy (make-vector2 120 -32) 200 (make-rect -16 -16 32 32)
               (lambda () (test-script))))

(define-coroutine (test-script)
  (move-in)
  (test-pattern 5)
  (test-pattern -5)
  (move-in)
  (test-pattern 5)
  (test-pattern -5)
  (move-in)
  (kill-entity))

(define (test-pattern step)
  (let ((n 8)
        (times 10))
    (let fire ((i 0)
               (a 0))
      (when (< i times)
        (repeat n (lambda (i)
                    (emit-bullet (entity-position) 2 (+ a (* 360 (/ i n))) 'fire
                                 test-bullet-script)))
        (wait 12)
        (fire (1+ i) (+ a step))))))

(define (move-in)
  (repeat 128 (lambda (i)
                (move-entity (make-vector2 0 1))
                (wait 1))))

(define-coroutine (test-bullet-script)
  (define angle-step 10)

  (define (step angle)
    (set-bullet-direction (+ (bullet-direction) (* 8 (sin-deg angle))))
    (wait 2)
    (step (+ angle angle-step)))

  (step 0))
