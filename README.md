# snek

A 2,5D snake game made using C++ and SFML for an assignment at university.

## Requirements
- CMake 3.20 or higher
- GCC, Clang or MSVC supporting C++23
- Packages required to build SFML (see [SFML CMake guide](https://www.sfml-dev.org/tutorials/3.0/getting-started/cmake/#requirements))

## How to build

```bash
cmake -S . -B build # optionally choose build type with -DCMAKE_BUILD_TYPE=Release/Debug
cmake --build build
```

## How to run

```bash
./build/snek_game
```

## TODO
- [ ] Add more features
- [ ] 2,5D graphics
- [ ] main-menu
- [ ] sound effects and music
- [ ] score system
- [ ] levels & level editor
