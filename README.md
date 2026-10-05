# MS_1

## COMPILE

```CMake(Recommended)
cmake --build build
```

```GCC
gcc -Wall -Wextra -Iinclude \
    src/main.c \
    src/mapMake.c \
    src/vehControl.c \
    lib/*.o \
    -o build/project
```
