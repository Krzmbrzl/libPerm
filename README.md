# libPerm

A C++17 library for representing and handling of permutation groups.

This library has been heavily inspired by https://github.com/tremlin/PermLib (which seems to aim at solving essentially the same problem but targeting
an older C++ standard and additionally uses a different philosophy on how and where to use dynamic memory allocations. Plus the project seems to be
unmaintained at this point). Additionally the following literature has been used as references (probably incomplete):
- T. Rehn, "Fundamental Permutation Group Algorithms For Symmetry Computation", Diploma thesis, Otto-von-Guericke University Magdeburg (2010)
- L. R. U. Manssur et al., "Group-theoretic approach for symbolic tensor manipulation", _Int. J. Mod. Phys. C_, 13, **7**, 859-879 (DOI: 10.1142/S0129183102004571)
- G. Butler, "Fundamental Algorithms for Permutation Groups", _Springer_, 1st edition, ISBN: 3-540-54955-2

## Building

### Requirements

- An ISO C++17 conform compiler and standard library implementation
- [cmake](https://cmake.org/) v3.15 or more recent

All other dependencies will be fetched automatically while running cmake (requires an active internet connection). These dependencies are
- [cmake-compiler-flags](https://github.com/Krzmbrzl/cmake-compiler-flags)
- [polymorphic\_variant](https://github.com/Krzmbrzl/polymorphic_variant)
- [GoogleTest](https://github.com/google/googletest)


### Build `libPerm`

From the root of the source tree, execute the following commands:
1. `mkdir build`
2. `cd build`
3. `cmake ..` (potentially with desired arguments, see below)
4. `cmake --build .`

The built libraries can be found in `build/lib/` whereas all executables can be found in `build/bin/`.


### Testing

In order to execute the built test cases, enter the `build` directory and execute `ctest --output-on-failure`.


### Available build options

Build options have to be passed to cmake, when invoking it. They are passed in the form of `-D<option>=<value>`. Thus, a cmake invocation with options
might look like this: `cmake -DCMAKE_BUILD_TYPE="Release" -DLIBPERM_EXAMPLES=ON ..`

| **Option** | **Description** | **Default** |
| ---------- | --------------- | ----------- |
| `CMAKE_BUILD_TYPE` | The type of the build. Most common choices are either `Debug` or `Release` | `Release` |
| `LIBPERM_LTO` | Whether to enable [link time optimization](http://johanengelen.github.io/ldc/2016/11/10/Link-Time-Optimization-LDC.html) (LTO) in `Release` builds | `ON`, if supported |
| `LIBPERM_TESTS` | Whether to build test cases | `ON` |
| `LIBPERM_EXAMPLES` | Whether to build the example applications | `OFF` |
| `LIBPERM_DISABLE_WARNINGS` | Whether to disable all warnings related to `libPerm` source files | `OFF` |
| `LIBPERM_WARNINGS_AS_ERRORS` | Whether to treat compiler warnings as errors | `OFF` |


## Documentation

A few commented example applications are provided in the [examples](./examples/) directory. An in-depth documentation can be found in the
[docs](./docs/) directory.

