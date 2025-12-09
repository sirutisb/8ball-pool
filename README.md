# 8 Ball Pool (C++ / SFML)

A simple 8-ball pool game implementation using C++ and SFML.

## Preview
<img width="1922" height="1112" alt="image" src="https://github.com/user-attachments/assets/cc4c7b92-8eb7-4e59-b414-ad624a154554" />


## Features
- Physics-based ball collisions and movement.
- Cue aiming and shooting mechanics.
- Standard 8-ball rack setup.

## Prerequisites

- **C++ Compiler**: Must support C++20.
- **CMake**: Version 3.28 or later.

## Building the Project

1. **Clone the repository** and navigate to the project root.

2. **Configure with CMake**:
   ```bash
   cmake -S . -B build
   ```

3. **Build**:
   ```bash
   cmake --build build --config Debug
   ```

4. **Run**:
   Navigate to the output directory and run the executable:
   ```bash
   ./build/bin/main
   # Or on Windows:
   .\build\bin\main.exe
   ```
   > **Note:** Ensure `arial.ttf` is in the same folder as the executable. You can copy it from the `assets/fonts/` folder.

## Controls

- **Mouse Movement**: Aim the cue.
- **Left Click**: Shoot.
- **Mouse Scroll**: Adjust shooting strength.
