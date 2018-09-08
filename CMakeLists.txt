project(genmap)
cmake_minimum_required(VERSION 2.8)
enable_language(C)

option(MPI "Build with MPI" ON)
option(DEBUG "Build with DEBUG mode" ON)
option(UNDERSCORE "Add underscore suffix" ON)

set(SRCROOT "${CMAKE_CURRENT_SOURCE_DIR}")
set(INCDIR "${SRCROOT}/inc")
set(TESTDIR "${SRCROOT}/tests" CACHE STRING "")
set(IODIR "${SRCROOT}/io" CACHE STRING "")

set(GSLIBDIR "${SRCROOT}/../gslib" CACHE STRING "")

find_library(GSLIB_LIBRARY NAMES gs HINTS ${GSLIBDIR})
add_library(gs UNKNOWN IMPORTED)
set_target_properties(gs PROPERTIES IMPORTED_LOCATION ${GSLIB_LIBRARY})

add_subdirectory(inc)
add_subdirectory(src)
