(define-module (gshmup core)
  #:use-module (gshmup keycodes)
  #:use-module (gshmup coroutine)
  #:use-module (gshmup agenda)
  #:export (bind-key
            wait
            emit-bullet))

(define %bind-key #f)
(define game-schedule #f)
(define %emit-bullet #f)

(load-extension "./gshmup.so" "gshmup_init")

(define (bind-key gshmup-key key)
  (%bind-key (gshmup-keycode gshmup-key) (keycode key)))

(define (wait delay)
  (abort-to-prompt 'coroutine-prompt
                   (lambda (resume) (game-schedule delay resume))))

(define* (emit-bullet type position speed direction #:optional script
                      #:key (acceleration 0) (angular-velocity 0) (life 0))
  "Emits a bullet into the current bullet system."
  (%emit-bullet type position speed direction acceleration angular-velocity
                life script))
