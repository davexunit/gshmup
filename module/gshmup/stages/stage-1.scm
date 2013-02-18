(define-module (gshmup stages stage-1)
  #:use-module (gshmup core)
  #:use-module (gshmup math)
  #:use-module (gshmup coroutine)
  #:use-module (gshmup helpers)
  #:use-module (gshmup bullets)
  #:export (stage-1))

(define test-enemy
  (make-enemy-type 60
                   (make-rect -16 -16 32 32)
                   (lambda () (test-on-death))))

(define stage-1
  (make-stage "Stage 1"
              "Is this thing on?"
              (coroutine ()
                (spawn-test-enemy))))

(define (spawn-test-enemy)
  (spawn-enemy test-enemy
               (make-vector2 120 -32)
               (lambda () (test-script))))

(define (test-on-death)
  (end-stage))

(define-coroutine (test-script)
  (move-in)
  (let fire ()
    (test-pattern-2)
    (wait 25)
    (fire))
  ;; (move-in)
  ;; (test-pattern 5)
  ;; (test-pattern -5)
  ;; (move-in)
  (kill-enemy))

(define (test-pattern step)
  (let ((n 8)
        (times 10))
    (let fire ((i 0)
               (a 0))
      (when (< i times)
        (repeat n (lambda (i)
                    (emit-bullet bullet-fire (entity-position)
                                 2 (+ a (* 360 (/ i n)))
                                 test-bullet-script
                                 #:life 90)))
        (wait 12)
        (fire (1+ i) (+ a step))))))

(define (test-pattern-2)
  (define-coroutine (delay-speed)
    (wait 5)
    (set-bullet-speed 2))
  (let* ((n 7)
         (da (vector2-angle (vector2-sub (player-position)
                                         (entity-position))))
         (arc 90)
         (half-arc (* -1 (/ arc 2))))
    (repeat n
            (lambda (i)
              (let* ((base-angle (* arc (/ i (1+ n))))
                     (a (+ half-arc da base-angle)))
                (emit-bullet bullet-fire (vector2-add (entity-position)
                                                      (vector2-from-polar 40 a))
                             0 (+ half-arc da (- arc base-angle))
                             delay-speed))))))

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
