(define-module (gshmup math)
  #:export (pi
            degrees->radians
            radians->degrees
            cos*
            sin*
            atan*))

(define pi 3.141592654)

(define (degrees->radians angle)
  (* angle (/ pi 180)))

(define (radians->degrees angle)
  (* angle (/ 180 pi)))

;; Sine, consine, and arctangent that accept/return angles in degrees.
(define (cos* angle)
  (cos (degrees->radians angle)))

(define (sin* angle)
  (sin (degrees->radians angle)))

(define (atan* y x)
  (radians->degrees (atan y x)))
