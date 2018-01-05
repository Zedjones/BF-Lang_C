Brainfuck_C [![License: GPL v2](https://img.shields.io/badge/License-GPL%20v2-blue.svg)](https://www.gnu.org/licenses/old-licenses/gpl-2.0.en.html) [![build status](https://api.travis-ci.org/Zedjones/Brainfuck_C.svg?branch=master)](https://travis-ci.org/Zedjones/Brainfuck_C)
==========
A Turing-complete Brainfuck interpreter written in C, which includes an interactive mode.

## Usage
    brainfuck_c [-n] [-f filename]
    -n    treat 10 (newline) as 0
    -f    brainfuck program file
    
## Example Programs
Check `brainfuck_examples` for example programs, many of which were written by [Daniel B. Cristofani](http://www.hevanet.com/cristofd/brainfuck/).

## Compiling and Building
The only dependencies for building this program are CMake and a C Compiler.
It can be built with the following set of commands:
```sh
$ mkdir build
$ cd build
$ cmake ..
$ make
```
The executable will now be in the `build` directory you created. 
