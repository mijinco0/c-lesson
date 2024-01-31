#
# to build, run the following commands in project's top directory.
# % mkdir build/
# % cd build/
# % cmake ..
# % make (or 'gmake' for FreeBSD)
#

CMAKE_MINIMUM_REQUIRED(VERSION 3.14)
set(PRJNAME eval)
project(${PRJNAME} C)

set(CMAKE_C_FLAGS_DEBUG "-g -O0")   # run cmake with '-DCMAKE_BUILD_TYPE=Debug' to enable this

file(GLOB SOURCES *.c)
file(GLOB SOURCES_FOR_TEST test/*.c)

set(INCDIRS
    test
)

add_executable(${PRJNAME} ${SOURCES} ${SOURCES_FOR_TEST})
target_include_directories(${PRJNAME} PUBLIC ${PROJECT_SOURCE_DIR} ${INCDIRS})
