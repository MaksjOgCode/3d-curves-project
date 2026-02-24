# 3d-curves-project
- 3d-curves-project is a `C++23` learning project with the implementation of simple 3D parametric curves and a demonstration of working with polymorphism, `std::expected`, smart pointers and STL algorithms.

## What is implemented:
### The library contains the following curves:
  - `circle_3d` is circle in the XY plane
  - `ellipse_3d` is an ellipse in the XY plane.
  - `helix_3d` is helix line around the Z axis
### Implemented for each curve:
- calculating a point on a curve: `point(t)`
- calculation of the first derivative: `derivative(t)`
- getting the class name: `name_class()`
### Error handling:
The `create(...)` factory methods are also used, which return `std::expected <...>` instead of exceptions for explicit error handling.

## Demonstration in `main.cpp`:
### Behavior of the program:
1) Creates a set of random curves (circles, ellipses, spirals).
Outputs for each curve:
  - `class name`
  - `point(π/4)`
  - `derivative(π/4)`
2) Forms a second container with only circles.
3) Sorts circles by radius.
4) Calculates the sum of the radii of the circles.

## Requirements:
1) CMake >= `3.23`
2) Compiler with `C++23` support

## Build via CMake:
The project is assembled into a single executable file
- target name: 3d-curves
### Example:
From the root of the repository:
```bash
cmake -B build -G "Ninja"
cmake --build build
./build/3d-curves
```

## Documentation (Doxygen):
- The repository already has the generated HTML documentation of Doxygen (documentation/html).
- To view it locally, go to the folder with the HTML files and run a simple HTTP server:
```bash
cd documentation/html
python3 -m http.server 8000
```
- After that, open in the browser:
```bash
http://localhost:8000/
```

## Useful notes:
- The project does not use external dependencies - only the C++ standard library.
- In the current CMakeLists.txt there is no `install(...)` and `(CTest)` only the 3d-curves executable file is collected.
