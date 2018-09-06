project(genmap)
cmake_minimum_required(VERSION 2.8)
enable_language(C)

option(MPI "Build with MPI" ON)
option(DEBUG "Build with DEBUG mode" ON)
option(UNDERSCORE "Add underscore suffix" ON)

set(SRCROOT "." CACHE STRING "")
set(INCDIR "${SRCROOT}/inc" CACHE STRING "")
set(BUILDDIR "${SRCROOT}/build" CACHE STRING "")
set(TESTDIR "${SRCROOT}/tests" CACHE STRING "")
set(IODIR "${SRCROOT}/io" CACHE STRING "")

set(GSLIBDIR "${SRCROOT}/../gslib" CACHE STRING "")

find_library(GSLIB_LIBRARY NAMES gs HINTS ${GSLIBDIR})
add_library(gs UNKNOWN IMPORTED)
set_target_properties(gs PROPERTIES IMPORTED_LOCATION ${GSLIB_LIBRARY})

add_subdirectory(src)
add_subdirectory(io)

add_library(genmap STATIC ${SOURCE})
target_link_libraries(genmap gs)
target_include_directories(genmap PRIVATE ${HEADERS})

set_target_properties(genmap PROPERTIES LINKER_LANGUAGE C)

if(${MPI})
	target_compile_definitions(genmap PRIVATE -DGENMAP_MPI)
endif()

if(${DEBUG})
	target_compile_definitions(genmap PRIVATE -DGENMAP_DEBUG)
endif()

if(${UNDERSCORE})
	target_compile_definitions(genmap PRIVATE -DGENMAP_UNDERSCORE)
endif()
