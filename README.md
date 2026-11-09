# Fate-Breaker: Little Rebel (OpenGL 3D Animation)

A short animated fantasy film made entirely in code with C++ and OpenGL (GLUT), built by a team of 5 for KP24803 Computer Graphics at Universiti Malaysia Sabah (2024/25, grade A). It follows Liang, a boy born with uncontrollable powers, from a cosmic "typo" in his destiny to reclaiming it.

[![Watch the animation](https://img.youtube.com/vi/J78d4rGYrVE/hqdefault.jpg)](https://youtu.be/J78d4rGYrVE)

*Click the image to watch the film on YouTube.*

## Scenes

| File | Scenes |
|---|---|
| `src/scene1.cpp` | Scene 1 – The Cursed Birth |
| `src/scene2.cpp` | Scene 2 – Growing Up Rejected |
| `src/scene3_4.cpp` | Scenes 3 and 4 – Revelation in the Cave, Breakthrough |
| `src/scene5_6.cpp` | Scenes 5 and 6 – Final Clash, New Dawn (my part) |

Each file is its own program with its own `main()`, and they are played one after another to form the film.

## Techniques

- 3D models built from OpenGL primitives (no imported models)
- Keyframe-style animation driven by GLUT timers
- Camera moves, lighting, particles and effects such as fire wheels and explosions
- Sound effects through the Windows multimedia API (`mciSendString`)

## Build and run (Windows)

1. Install a C++ compiler with freeglut, for example Code::Blocks with freeglut, or Visual Studio with the freeglut NuGet package.
2. Compile one scene at a time and link `freeglut`, `opengl32`, `glu32` and `winmm`. With MinGW:
   ```
   g++ src/scene1.cpp -o scene1.exe -lfreeglut -lopengl32 -lglu32 -lwinmm
   ```
3. Put the sound files listed in `src/sounds/README.md` into `src/sounds/` and run the program from the `src` folder.

## Team

Built by a team of 5 UMS students for KP24803 Computer Graphics. I built Scenes 5 and 6 (`src/scene5_6.cpp`): the final battle between Liang and the Evil Guardian, and the closing scene.
