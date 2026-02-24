cmake_minimum_required ( VERSION 3.23 )

project (
    3d-curves-project
    VERSION 0.1.0
    LANGUAGES CXX
)

set ( CMAKE_CXX_STANDARD          23  )
set ( CMAKE_CXX_STANDARD_REQUIRED ON  )
set ( CMAKE_CXX_EXTENSIONS        OFF )



# 3d-curves:
add_executable (
    3d-curves
    ${CMAKE_CURRENT_SOURCE_DIR}/src/main/main.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/curves/curves_3d.cpp
)

target_include_directories(
    3d-curves
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)