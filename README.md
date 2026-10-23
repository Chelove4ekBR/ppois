# PPOIS Labs

## Structure

| Lab | Language | Contents                                                | Docs                                       |
|-----|----------|---------------------------------------------------------|--------------------------------------------|
| 1   | C++      | [Set](./lab1/Set), [Matrix](./lab1/Matrix)              | Doxygen, [Documentation](#documentation) |
| 2   | C++      | [TravelAgency](./lab2)                                  | Doxygen, [Documentation](#documentation) |

Each project has the same layout:

```
include/   class declarations (.h)
src/       class implementation (.cpp)
sli/       console menu (main.cpp)
tests/     unit tests (Catch2)
```

## Requirements

- C++17 compiler (g++)
- [CMake](https://cmake.org/) 3.31+
- [Catch2](https://github.com/catchorg/Catch2) v3 — unit tests
- [gcovr](https://gcovr.com/) — code coverage
- [cppcheck](https://cppcheck.sourceforge.io/) — static analysis
- [Doxygen](https://www.doxygen.nl/) — documentation

## Build and run

From the project directory:

```bash
cmake -B build
cmake --build build
```

Code coverage (the CI requires at least 90% of lines in `src/`):

```bash
gcovr --root . --filter 'src/'
```

## Documentation

Generated with [Doxygen](https://www.doxygen.nl/).

The result in `docs/html`, open `docs/html/index.html` in a browser.

## Releases

Prebuilt binaries for Linux are published in [Releases](https://github.com/Chelove4ekBR/ppois/releases).