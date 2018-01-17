BF-Lang_C [![License: GPL v2](https://img.shields.io/badge/License-GPL%20v2-blue.svg)](https://www.gnu.org/licenses/old-licenses/gpl-2.0.en.html) [![build status](https://api.travis-ci.org/Zedjones/BF-Lang_C.svg?branch=master)](https://travis-ci.org/Zedjones/BF-Lang_C)
==========
A Turing-complete Brainfuck interpreter written in C, which includes an interactive mode.

## Table of Contents
<!-- vim-markdown-toc GFM --> 
* [Usage](#usage)
* [Licensing](#licensing)
* [Example Programs](#example-programs)
* [Compiling and Building](#compiling-and-building)
* [Future Features](#future-features)

## Usage
    brainfuck_c [-s] [-n] [-f filename]
    -n    treat 10 (newline) as 0
    -f    brainfuck program file
	-s    immediately flush output to stdout

## Licensing
All files are licensed under GPL 2.0, unless otherwise specified, as is the case with `src/alternatives.c` (which is public domain).

## Example Programs
Check `brainfuck_examples` for example programs, many of which were written by [Daniel B. Cristofani](http://www.hevanet.com/cristofd/brainfuck/).

## Compiling and Building
The only dependencies for building this program are CMake and a C Compiler.
It can be built on MacOS or Linux with the following set of commands:
```sh
$ mkdir build
$ cd build
$ cmake ..
$ make
```

On Windows, using MinGW and CMake (both binaries must be in $PATH), you must use this set of commands: 
```sh
$ mkdir build
$ cd build
$ cmake .. -G "MinGW Makefiles"
$ mingw32-make
```

The executable will now be in the `build` directory you created. 

## Future Features
* Faster mode for file input
* Add more options for interpreter (e.g. disable wrapping)
