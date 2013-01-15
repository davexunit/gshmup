(define-module (gshmup shooter)
  #:use-module (gshmup core)
  #:export (init-shooter))

(define (init-shooter)
  (display "testing")
  (newline))

(add-hook! player-shoot-hook
           (lambda ()
             (let ((p (player-position))
                   (speed 12))
               (emit-bullet p speed 268)
               (emit-bullet p speed 270)
               (emit-bullet p speed 272))))
