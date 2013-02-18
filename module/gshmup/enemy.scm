(define-module (gshmup enemy)
  #:use-module (gshmup core)
  #:export (register-enemy-type
            spawn-enemy))

(define enemy-types (make-hash-table))

(define (register-enemy-type key type)
  "Associates a key with an enemy type"
  (hash-set! enemy-types key type))

(define (get-enemy-type key)
  "Applies the enemy properties associated with the given key."
  (hash-ref enemy-types key))

(define (spawn-enemy type position script)
  (let ((type (get-enemy-type type)))
    (%spawn-enemy type position script)))
