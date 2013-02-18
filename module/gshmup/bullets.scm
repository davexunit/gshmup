(define-module (gshmup bullets)
  #:use-module (gshmup core)
  #:export (bullet-fire
            bullet-blue))

;; Leaving this stuff here for reference later.

;; (register-bullet-type 'medium-blue
;;                       (make-bullet-type 0 (make-rect -4 -4 8 8) 'alpha #f kill-player))
;; (register-bullet-type 'small-diamond
;;                       (make-bullet-type 1 (make-rect -2 -2 4 4) 'alpha #t kill-player))
;; (register-bullet-type 'large-orange
;;                       (make-bullet-type 2 (make-rect -5 -5 10 10) 'alpha #f kill-player))
;; (register-bullet-type 'blue
;;                       (make-bullet-type 3 (make-rect -3 -3 6 6) 'alpha #t
;;                                         (lambda () (damage-enemy 1))))
;; (register-bullet-type 'bright
;;                       (make-bullet-type 4 (make-rect -8 -8 16 16) 'add #f kill-player))
;; (register-bullet-type 'fire
;;                       (make-bullet-type 5 (make-rect -2 -2 4 4) 'alpha #t kill-player))

(define bullet-blue
  (make-bullet-type 3 (make-rect -3 -3 6 6) 'alpha #t
                    (lambda () (damage-enemy 1))))

(define bullet-fire
  (make-bullet-type 5 (make-rect -2 -2 4 4) 'alpha #t kill-player))
