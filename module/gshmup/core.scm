(define-module (gshmup core)
  #:use-module (gshmup keycodes)
  #:export (bind-key))

(load-extension "./gshmup.so" "gshmup_init")

(define (bind-key gshmup-key key)
  (%bind-key (gshmup-keycode gshmup-key) (keycode key)))
