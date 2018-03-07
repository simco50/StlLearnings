# README #

[![MIT licensed](https://img.shields.io/badge/license-MIT-blue.svg)](LICENSE.md)

I am used to go the easy way and use STL everywhere without thinking much about how it all works.
With this project, I try to experiment with the implementation of my own STL-like types.
My goal is to face challenges and think about how the STL types work internally.
Besides that, I use this project as a chance to experiment with combining unit testing with AppVeyor for continuous integration.

## Build Status

| Service | System | Compiler | Branch | Status |
| ------- | ------ | -------- | ------ | ------ |
| [AppVeyor](https://ci.appveyor.com/project/simco50/stdlearnings/branch/master)| Windows x86/x64 | MSVC 2017 | master | [![Build status](https://ci.appveyor.com/api/projects/status/onjouvded36kqcyt/branch/master?svg=true)](https://ci.appveyor.com/project/simco50/stdlearnings/branch/master)
| [AppVeyor](https://ci.appveyor.com/project/simco50/stdlearnings/branch/dev)| Windows x86/x64 | MSVC 2017 | dev | [![Build status](https://ci.appveyor.com/api/projects/status/onjouvded36kqcyt/branch/dev?svg=true)](https://ci.appveyor.com/project/simco50/stdlearnings/branch/dev)

## Current features

* String
* Containers: Vector, Map, HashMap, Array
* Smart Pointers: Unique/Shared/Weak Pointer
* Iterators
* Sorting
* Misc utilities

## Desired types

The idea started off with wanting to try to create a Vector and String class to begin with an think about optimizations specifically to games.
The goal is to expand to some more interesting containers like hashmaps, linked lists, ...

## Goals

* Learning the workings of STL types
* Experimenting with running unit tests on AppVeyor
* Face challenges that require some good thinking

## External libraries

* [Catch2](https://github.com/catchorg/Catch2)