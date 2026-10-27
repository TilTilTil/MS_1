cmake_minimum_required(VERSION 3.16)

project(ms C)

# set(CMAKE_C_STANDARD 11)
# set(CMAKE_C_STANDARD_REQUIRED ON)

file(GLOB LIB_OBJECTS
    "${CMAKE_CURRENT_SOURCE_DIR}/lib/*.o"
)

add_executable(project
    src/main.c
    src/mapMake.c
    src/vehControl.c
    ${LIB_OBJECTS}
)

target_include_directories(project PRIVATE
    include
)