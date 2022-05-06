# envi

demo implementation for

[wrapProgram should not move script files. instead, patch shebang lines #171689](https://github.com/NixOS/nixpkgs/issues/171689)

## demo output

```
$ ./build.sh && ./test.py a b c d

envi: argv:
envi: $0 = ./envi
envi: $1 = ./test.py
envi: $2 = a
envi: $3 = b
envi: $4 = c
envi: $5 = d
envi: script = ./test.py
envi: dir = .
envi: base = test.py
envi: envSize = 23
envi: args:
envi: $0 = /nix/store/d60gkg5dkw4y5kc055n4m0xyvcjz65im-bash-interactive-5.1-p16/bin/bash
envi: $1 = -e
envi: $2 = ./.test.py.env0.sh
envi: $3 = ./.test.py.env1.sh
envi: $4 = --
envi: $5 = ./test.py
envi: $6 = a
envi: $7 = b
envi: $8 = c
envi: $9 = d
envi: exec bash
env0: hello
env0: $0 = ./.test.py.env0.sh
env0: $1 = ./.test.py.env1.sh
env0: $2 = --
env0: $3 = ./test.py
env0: $4 = a
env0: $5 = b
env0: $6 = c
env0: $7 = d
env0: sourcing env1
env1: hello
env1: $0 = ./.test.py.env0.sh
env1: $1 = --
env1: $2 = ./test.py
env1: $3 = a
env1: $4 = b
env1: $5 = c
env1: $6 = d
env1: no more env's to source -> exec
test.py: sys.argv = ['./test.py', 'a', 'b', 'c', 'd']
test.py: A = 1
test.py: B = 2
```
