# SpaceGame

A 2D arcade space shooter written in C with [raylib](https://www.raylib.com/). Dodge falling asteroids, shoot them to increase your score, and collect shields to survive a collision.

## Gameplay

- **Easy mode:** the ship faces upward.
- **Hard mode:** the ship turns in its movement direction, changing where it shoots.
- Destroying an asteroid awards one point. Movement and scrolling get faster as the score increases.
- A collected shield lasts up to five seconds or absorbs one asteroid hit.
- The high score is kept for the current session; it is not saved when you close the game.

## Build and run on Linux

You need a C99 compiler (GCC or Clang), GNU Make, `pkg-config`, and the raylib development headers and library. Install raylib using the [official Linux setup guide](https://github.com/raysan5/raylib/wiki/Working-on-GNU-Linux).

Check that raylib is available:

```sh
pkg-config --modversion raylib
```

Clone and build:

```sh
git clone https://github.com/kaloyanMilev13/SpaceGame.git
cd SpaceGame
make
make run
```

If you already have the repository, run `make` and `make run` from its root folder. The executable is written to `build/spacegame`. Run it from the repository root so it can find the textures in `img/`; `make run` does this when invoked from that folder. A graphical desktop is required to play.

To remove the generated executable:

```sh
make clean
```

The provided Makefile targets Linux. Windows and macOS builds need the corresponding raylib toolchain and platform link settings.

## Controls

| Key | Action |
| --- | --- |
| Enter | Start from the introduction |
| WASD or arrow keys | Move the ship |
| Space | Fire (one projectile can be active at a time) |
| M | Open the menu during play or after game over; return from the menu |
| Up / Down in the menu | Switch between easy and hard mode |
| Enter in the menu | Return to the previous screen |
| R or Enter after game over | Restart |
| Escape | Quit |

## Repository layout

```text
SpaceGame/
├── main.c          # Game states, movement, collisions, and rendering
├── Makefile        # Linux build and run commands
├── img/            # Game textures and alternative artwork
├── doc/            # Analysis, design, and validation documents
│   ├── analysis/
│   └── design/
└── build/          # Generated executable (ignored by Git)
```

See the [documentation index](doc/README.md) for reports, diagrams, and validation files. Keep source code, assets, and project documents in Git; generated executables belong in `build/`.
