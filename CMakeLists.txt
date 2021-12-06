cmake_minimum_required(VERSION 3.0)

project(osm-coordinator CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXE_LINKER_FLAGS "-static")

add_executable(osm-coordinator main.cpp)
find_package(PkgConfig)
pkg_check_modules(GEOGRAPHICLIB REQUIRED geographiclib)
add_library(GEOGRAPHICLIB STATIC IMPORTED)
include_directories(${GEOGRAPHICLIB_INCLUDE_DIRS} lib)
target_link_libraries (osm-coordinator ${GEOGRAPHICLIB_LIBRARIES})

install(TARGETS osm-coordinator DESTINATION ${CMAKE_INSTALL_PREFIX})
