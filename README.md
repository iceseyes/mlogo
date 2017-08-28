mLogo
=====

A simple Logo Language Interpreter inspired by UCB Logo.

How to compile
--------------

mLogo can be compiled under Linux. It requires libboost >= 1.58, libSDL2 and a version of g++ or clang which support C++14 standard. For example, on ubuntu Xenial or later you can install this packages:

```bash
sudo apt install git cmake libboost-all-dev libsdl2-dev build-essential
```

When you have collected every dependency and cloned the project, you can compile mlogo using Cmake. 
For example, if you want to use `build/Debug` subdirectory to keep mlogo debug compilation you can
write something like that:


```bash
mkdir -p build/Debug
cd build/Debug
cmake -DCMAKE_BUILD_TYPE=Debug . ../../
make
```

To run `mlogo`, run `./mlogo`. To run tests use `test/mlogo_test`.
