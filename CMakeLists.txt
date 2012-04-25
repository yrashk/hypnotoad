cmake_minimum_required (VERSION 2.8)
cmake_policy(SET CMP0015 OLD)
project (Hypnotoad)
aux_source_directory(c_src SRC)
aux_source_directory(deps/clips/core CLIPS_SRC)
set(MAIN_CLIPS_SRC ${CLIPS_SRC})
list(REMOVE_ITEM CLIPS_SRC deps/clips/core/main.c)
list(REMOVE_ITEM CLIPS_SRC deps/clips/core/userfunctions.c)
find_library(M_LIB m)
add_executable(clips ${MAIN_CLIPS_SRC})
add_executable(hypnotoad ${SRC} ${CLIPS_SRC})
add_library(zmq STATIC IMPORTED)
set_target_properties(zmq PROPERTIES IMPORTED_LOCATION deps/zeromq/src/.libs/libzmq.a)
target_link_libraries(clips ${M_LIB})
target_link_libraries(hypnotoad zmq stdc++ ${M_LIB})
add_dependencies(hypnotoad deps/zeromq/src/.libs/libzmq.a)
include_directories("deps/clips/core" "deps/zeromq/include")

add_custom_command(OUTPUT deps/zeromq/src/.libs/libzmq.a
                   COMMAND ./autogen.sh
                   COMMAND ./configure
                   COMMAND make
                   WORKING_DIRECTORY deps/zeromq)
