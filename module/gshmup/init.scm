(define-module (gshmup init)
  #:use-module (gshmup core)
  #:use-module (gshmup shooter))

;; Debug
(set! debug-mode #f)

;; Set preferred keybindings.
(bind-key 'up 'up)
(bind-key 'down 'down)
(bind-key 'left 'left)
(bind-key 'right 'right)
(bind-key 'shoot 'z)
(bind-key 'bomb 'x)
(bind-key 'start 'enter)

;; Splash screen settings
(set! splash-screen-background "data/sprites/splash_background.png")
(set! splash-screen-title "data/sprites/splash_title.png")

;; Player settings.
(set! num-credits 5)
(set! lives-per-credit 5)
(set! player-speed 3.5)
