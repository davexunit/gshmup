GSHMUP
======

A bullet hell shooter written in C and Guile.

This is an experiment to write a small game that uses the GNU
extension language, Guile, for configuration and event scripting.

Lua is often the go-to language for game scripting, but I wanted to
try something different.

Try it
------
```
make
./gshmup
```

Refer to `module/gshmup/init.scm` for keybindings and other configuration.

For those feeling more adventurous, Gshmup runs a REPL server. Connect
using Geiser in Emacs and have fun manipulating the game as it is
running.

Dependencies
------------

* Allegro >= 5.0
* GNU Guile >= 2.0

License
-------

GNU GPL v3

Thanks
------
Thanks to:
* mark_weaver and ijp for helping me with everything Guile
  related on a regular basis.
* wingo for his coroutine implementation.
* MIT Press for SICP.
* the other fine folks that I forgot to mention on the #guile and
  #allegro rooms on freenode.