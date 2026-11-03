# MegaMan Simulator

Explore the Air Man stage, navigate moving platforms and dragons, and fight Copipi enemies. Reach the right end of the level before losing all three lives. Enemies retain damage between lives, and defeated enemies stay defeated until you start a new game.

## Demo

[![Watch the gameplay demo](https://img.youtube.com/vi/tmzkgg_k1xg/maxresdefault.jpg)](https://www.youtube.com/watch?v=tmzkgg_k1xg)

## Controls

| Key | Action |
| --- | --- |
| ← / → | Move |
| Space | Jump while standing on a platform |
| Z (hold) | Shoot every 0.5 seconds |
| Enter | Start from the title screen / return to the title screen after a win or loss |
| O | Open or close instructions in the menus; toggle collision debug overlays during gameplay |
| Esc | Quit |

## Building on Windows

The included solution uses Visual Studio with the **v145** C++ toolset, a Windows SDK, and C++20 support.

1. Clone this repository.
2. Place the raylib source files in `build/external/raylib-master/` so that `build/external/raylib-master/src/raylib.h` exists. This dependency is not tracked in Git because the `build/` directory is ignored. The local copy used for this project reports its version as **6.5-dev**.
3. Open `programacion-grafica-pec2.slnx`, select **Debug | x64**, and build the solution.
4. Run the game from the repository root so it can find `resources/`:

```powershell
.\bin\Debug\programacion-grafica-pec2.exe
```

To run it from Visual Studio, set the debugger's working directory to `$(SolutionDir)`.
