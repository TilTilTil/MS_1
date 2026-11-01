# MS_1 & MS_2

### PACKAGES

Do download readline:

```bash
sudo apt install libreadline-dev
```

## COMPILE

### 1. First Compile / Clean Rebuild

Run these commands from the project root directory:

```bash
rm -rf build
cmake -S . -B build
cmake --build build
```

- `rm -rf build` — Removes the existing build directory and its contents.
- `cmake -S . -B build` — Configures the project and generates build files in `build/`.
- `cmake --build build` — Compiles the project.

**Note:** Only delete `build/` when you want a clean rebuild. You normally don't need to do this every time.

### 2. Subsequent Builds (Recommended)

After the initial configuration, use:

```bash
cmake --build build
```

CMake will rebuild the parts of the project that need updating.

### 3. Manual Compilation Using GCC

Alternatively, compile manually using GCC:

```bash
gcc -Wall -Wextra -Iinclude \
    src/*.c \
    lib/*.o \
    -lreadline \
    -o build/project
```

- `-Wall -Wextra` — Enables additional compiler warnings.
- `-Iinclude` — Tells GCC where to find header files.
- `src/*.c` — Specifies the source files to compile.
- `lib/*.o` — Includes existing object files.
- `-o build/project` — Specifies the output executable.

**Important:** The GCC command assumes that the `build/` directory already exists and that the object files in `lib/` are compatible with your source code and system.
