(define-module (gshmup shooter)
  #:use-module (gshmup core)
  #:use-module (gshmup bullet)
  #:use-module (gshmup coroutine)
  #:use-module (gshmup helpers)
  #:export (init-shooter))

;; Bullet types
(register-bullet-type 'medium-blue
                      (make-bullet-type 0 (make-rect -4 -4 8 8) 'alpha #f))
(register-bullet-type 'small-diamond
                      (make-bullet-type 1 (make-rect -2 -2 4 4) 'alpha #t))
(register-bullet-type 'large-orange
                      (make-bullet-type 2 (make-rect -5 -5 10 10) 'alpha #f))
(register-bullet-type 'sword
                      (make-bullet-type 3 (make-rect -4 -4 8 8) 'alpha #t))
(register-bullet-type 'bright
                      (make-bullet-type 4 (make-rect -8 -8 16 16) 'add #f))
(register-bullet-type 'fire
                      (make-bullet-type 5 (make-rect -2 -2 4 4) 'alpha #t))

(add-hook! player-shoot-hook (lambda () (test-shot)))

(define-coroutine (test-shot)
  (when (player-shooting?)
    (let ((speed 15)
          (p (player-position)))
      (emit-bullet p speed 268 'sword)
      (emit-bullet p speed 270 'sword)
      (emit-bullet p speed 272 'sword)
      (emit-bullet p speed 180 'sword)
      (emit-bullet p speed 225 'sword)
      (emit-bullet p speed 315 'sword)
      (emit-bullet p speed 360 'sword))
    (wait 5)
    (test-shot)))

(add-hook! shooter-init-hook (lambda () (test-level)))

(define-coroutine (test-level)
  (spawn-test-enemy))

(define (spawn-test-enemy)
  (spawn-enemy (make-vector2 120 -32) 200 (lambda () (test-script))))

(define-coroutine (test-script)
  (move-in)
  (test-pattern))

(define (test-pattern)
  (let fire ((i 0)
             (a 0)
             (n 12))
    (when (< i 20)
      (repeat n (lambda (i)
                  (emit-bullet (entity-position) 2 (+ a (* 360 (/ i n))) 'fire)))
      (wait 12)
      (fire (1+ i) (+ a 6) n)))
  (move-in)
  (test-pattern))

(define (move-in)
  (repeat 32 (lambda (i)
               (move-entity (make-vector2 0 2))
               (wait 1))))

(define-coroutine (test-move)
  (repeat 120 (lambda (i)
                (move-entity (make-vector2 3.5 0))
                (wait 1)))
  (repeat 120 (lambda (i)
                (move-entity (make-vector2 -3.5 0))
                (wait 1)))
  (test-move))
