(define-module (gshmup stages stage-1)
  #:use-module (gshmup core)
  #:use-module (gshmup math)
  #:use-module (gshmup coroutine)
  #:use-module (gshmup helpers)
  #:use-module (gshmup bullets)
  #:use-module (gshmup enemies)
  #:export (stage-1))

(define stage-1
  (make-stage "Stage 1"
              "Is this thing on?"
              (coroutine ()
                (spawn-test-enemy))))

(define (spawn-test-enemy)
  (spawn-enemy enemy-test
               (make-vector2 120 -32)
               (lambda () (test-script))))

(define-coroutine (test-script)
  (move-in)
  (let fire ()
    (test-pattern 3)
    (wait 25)
    (fire))
  ;; (move-in)
  ;; (test-pattern 5)
  ;; (test-pattern -5)
  ;; (move-in)
  (kill-enemy))

(define (test-pattern step)
  (let ((num-bullets 8)
        (times 10)
        (delay 12))
    (repeat times
            (lambda (i)
              (let ((angle (* i step)))
                (repeat num-bullets
                        (lambda (j)
                          (let ((direction (+ angle (* 360 (/ j num-bullets)))))
                            (emit-bullet bullet-fire (entity-position)
                                         2 direction test-bullet-script)))))
              (wait delay)))))

(define (test-pattern-2)
  (define-coroutine (delay-speed)
    (wait 5)
    (set-bullet-speed 2))

  (let* ((num-bullets 7)
         (arc 90)
         ;; Center arc on angle to player.
         (target-angle (- (angle-to-player) (/ arc 2)))
         (position (entity-position)))
    (repeat num-bullets
            (lambda (i)
              (let* ((base-angle (* arc (/ i (1+ num-bullets))))
                     (angle (+ target-angle base-angle))
                     (position (vector2-add position (vector2-from-polar 40 angle)))
                     ;; Direction is the inverse of the angle for a neat effect.
                     (direction (+ target-angle (- arc base-angle))))
                (emit-bullet bullet-fire position 0 direction delay-speed))))))

(define (move-in)
  (repeat 128
          (lambda (i)
            (move-entity (make-vector2 0 1))
            (wait))))

(define-coroutine (test-bullet-script)
  (define (step angle)
    (let* ((angle-step 10)
           (scale 9)
           (direction (+ (bullet-direction)
                         (* scale (sin* angle)))))
      (set-bullet-direction direction)
      (wait 2)
      (step (+ angle angle-step))))

  (step 0))
