(define-module (gshmup core)
  #:use-module (gshmup keycodes)
  #:use-module (gshmup coroutine)
  #:use-module (gshmup agenda)
  #:export (bind-key
            wait))

(define %bind-key #f)
(define game-schedule #f)

(load-extension "./gshmup.so" "gshmup_init")

(define (bind-key gshmup-key key)
  (%bind-key (gshmup-keycode gshmup-key) (keycode key)))

(define (wait delay)
  (abort-to-prompt 'coroutine-prompt
                   (lambda (resume) (game-schedule delay resume))))
