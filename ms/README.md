# MS_1

## COMPILE

CMake(Recommended)

```bash
cmake --build build
```

GCC

```bash
gcc -Wall -Wextra -Iinclude \
    src/main.c \
    src/mapMake.c \
    src/vehControl.c \
    lib/*.o \
    -o build/project
```
