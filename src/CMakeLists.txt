set(SOURCE
  io/genmap-fortran.c
  io/genmap-default.c
  io/genmap-gmsh.c
  genmap-io.c
  genmap-fortran.c
  genmap.c
  genmap-algo.c
  genmap-vector.c
  genmap-comm.c
)

add_library(genmap STATIC ${SOURCE})
target_link_libraries(genmap gs)
target_include_directories(genmap PRIVATE ${INCDIR})

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

