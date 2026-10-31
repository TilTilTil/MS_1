
cmake_minimum_required(VERSION 3.16)

project(ms C)

# set(CMAKE_C_STANDARD 11)
# set(CMAKE_C_STANDARD_REQUIRED ON)

find_path(READLINE_INCLUDE_DIR readline/readline.h REQUIRED)
find_library(READLINE_LIBRARY readline REQUIRED)

file(GLOB LIB_OBJECTS
    "${CMAKE_CURRENT_SOURCE_DIR}/lib/*.o"
)

add_executable(project
    src/main.c
    src/mapMake.c
    src/vehControl.c
    src/autoControl.c
    src/errorHandle.c
    ${LIB_OBJECTS}
)

target_include_directories(project PRIVATE
    include
    ${READLINE_INCLUDE_DIR}
)

target_link_libraries(project PRIVATE
    ${READLINE_LIBRARY}
)
