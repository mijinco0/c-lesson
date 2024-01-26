#
# how to build:
# % mkdir build/
# % cd build/
# % cmake ..
# % make (or 'gmake' for FreeBSD)
#

CMAKE_MINIMUM_REQUIRED(VERSION 3.14)
set(PRJNAME parser)
project(${PRJNAME} C)

file(GLOB SOURCES *.c)
file(GLOB SOURCES_FOR_TEST test/*.c)

set(INCDIRS
    test
)

add_executable(${PRJNAME} ${SOURCES} ${SOURCES_FOR_TEST})
target_include_directories(${PRJNAME} PUBLIC ${PROJECT_SOURCE_DIR} ${INCDIRS})
