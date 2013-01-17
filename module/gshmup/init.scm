(define-module (gshmup init)
  #:use-module (gshmup core)
  #:use-module (gshmup shooter))

;; Set preferred keybindings.
(bind-key 'up 'up)
(bind-key 'down 'down)
(bind-key 'left 'left)
(bind-key 'right 'right)
(bind-key 'shoot 'z)
(bind-key 'bomb 'x)
(bind-key 'start 'enter)

;; Player settings.
(set! player-credits 5)
(set! player-lives 5)
(set! player-speed 6.5)
