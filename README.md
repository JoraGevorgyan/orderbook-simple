# project_name — Minimal C++ CLI template

Overview
--------
Small, conventional C++17 CLI project template using CMake and Conan (Conan 2).

Requirements
------------
- CMake 3.21+
- Conan 2.x
- A C++ compiler (GCC or Clang). MSVC should also work.

Quick start
-----------
1. Detect a Conan profile (run once):

```
conan profile detect --force
```

2. Install dependencies into the build folder (conan will generate a CMake toolchain):

```
conan install . --output-folder=build --build=missing
```

3. Configure and build (Debug example):

```
cmake --preset debug
cmake --build build/debug
```

Apply clang-format:

```
cmake --build build/debug --target apply_format
```

Or with plain cmake:

```
cmake -S . -B build -DCMAKE_TOOLCHAIN_FILE=build/conan_toolchain.cmake -DCMAKE_BUILD_TYPE=Debug
cmake --build build
```

Run the executable:

```
./build/debug/app
```

Run tests with CTest:

```
ctest --test-dir build/debug --output-on-failure
```

Project layout
--------------

```
project-name/
├── CMakeLists.txt
├── CMakePresets.json
├── conanfile.txt
├── .gitignore
├── README.md
├── src/
│   ├── main.cpp
│   └── module1/
│       ├── greet.cpp
│       └── ...
├── include/
│   └── module1/
│       └── greet.hpp
└── tests/
    ├── unit/
    │   └── test_greet.cpp
    └── integration/
        └── test_integration.cpp
```

Where to add code
-----------------
- Put implementation `.cpp` files in `src/<module>/`.
- Put public headers in `include/<module>/`.
- Add unit tests to `tests/unit/` and integration tests to `tests/integration/`.

Adding a Conan dependency
-------------------------
1. Add the dependency to `conanfile.txt` under `[requires]`.
2. Re-run `conan install . --output-folder=build --build=missing`.
3. Re-configure and build with CMake.
# orderbook-simple
just a simple orderbook(experimental)

