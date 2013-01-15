(define-module (gshmup init)
  #:use-module (gshmup core)
  #:use-module (gshmup shooter))

(add-hook! shooter-init-hook (lambda ()
                               (display "hello world!")
                               (newline)))
