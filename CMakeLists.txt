cmake_minimum_required (VERSION 2.8)
cmake_policy(SET CMP0015 OLD)
project (Hypnotoad)
aux_source_directory(c_src SRC)
aux_source_directory(deps/clips/core CLIPS_SRC)
set(MAIN_CLIPS_SRC ${CLIPS_SRC})
list(REMOVE_ITEM CLIPS_SRC deps/clips/core/main.c)
list(REMOVE_ITEM CLIPS_SRC deps/clips/core/userfunctions.c)
link_directories(deps/zeromq/src/.libs)
find_library(M_LIB m)
add_executable(clips ${MAIN_CLIPS_SRC})
add_executable(hypnotoad ${SRC} ${CLIPS_SRC})
target_link_libraries(clips ${M_LIB})
target_link_libraries(hypnotoad libzmq.a ${M_LIB})
include_directories("deps/clips/core" "deps/zeromq/include")

add_custom_command(TARGET hypnotoad PRE_BUILD
                   COMMAND ./autogen.sh
                   COMMAND ./configure
                   COMMAND make
                   WORKING_DIRECTORY deps/zeromq)
