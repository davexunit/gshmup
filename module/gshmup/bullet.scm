(define-module (gshmup bullet)
  #:use-module (gshmup core)
  #:export (get-bullet-type
            register-bullet-type
            emit-bullet
            set-bullet-type))

(define bullet-types (make-hash-table))

(define (register-bullet-type key type)
  "Associates a key with a procedure for setting particular bullet properties."
  (hash-set! bullet-types key type))

(define (get-bullet-type key)
  "Applies the bullet properties associated with the given key."
  (hash-ref bullet-types key))

;;(define* (emit-bullet system pos speed direction type . keyword-args)
(define* (emit-bullet position speed direction type #:optional thunk
                      #:key (acceleration 0) (angular-velocity 0) (life 0))
  "Emits a non-scripted bullet."
  (let ((type (get-bullet-type type)))
    (%emit-bullet position speed direction acceleration angular-velocity
                  life type thunk)))

(define (set-bullet-type bullet type)
  (let ((type (get-bullet-type type)))
    (%set-bullet-type bullet type)))
