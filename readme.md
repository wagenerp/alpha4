# Alpha4 common code library
## Summary

This is a library for me to dump common code I keep using in various places.

Although this could theoretically be built as a library (shared or otherwise) and installed system-wide, it is usually included on source level as a submodule in whatever repository is using it.

To do so, simply add the submodule and supply info to your (root) CMakeLists.txt, e.g.:

    include_directories(alpha4/src/)
    add_subdirectory(alpha4/src/)

This way your project can find all necessary files via including the C++ (`alpha4/**/*.hpp`) or C (`alpha4c/**/*.h`) headers.
Note that although there are some pure C headers, the C library depends on the C++ one, so make sure to link against both.


If you wish to build and install Alpha4 anyway, simply use the root `CMakeLists.txt` the usual way:

    mkdir -p build; cd build
    cmake ../
    make 
    make install

