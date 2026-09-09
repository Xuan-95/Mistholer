# Mistholer

## About the Name

**Mistholer** is a composition of **Mis**ner, **Tho**rne, and Whee**ler** — the three physicists who authored the monumental textbook *Gravitation* (1973):

- **Charles W. Misner**
- **Kip S. Thorne**
- **John Archibald Wheeler**

Their collaborative work represents one of the most comprehensive treatments of general relativity ever written, known for its depth, rigor, and pedagogical approach.

## Philosophy

This project follows the philosophy embodied in *Gravitation*: **start from the fundamentals and progressively build understanding layer by layer**. 

The goal is to understand how the physics engine works under the hood — not just at the surface level, but by diving deep into the mathematical and conceptual foundations, exploring each layer of abstraction until the full picture emerges.

Just as Misner, Thorne, and Wheeler guided readers from basic principles to the frontiers of gravitational physics, Mistholer aims to build knowledge from the ground up.

---

## Project structure

```text
src/
  mistholer.c       Application entry point and simulation loop
  core/             Shared definitions and memory utilities
  math/             Vector operations
  physics/          Bodies, scenes, and collision detection/resolution
    shapes/         Circles, axis-aligned boxes, and polygons
  io/               Configuration and JSON scene loading
  render/           Shape drawing
  ui/               User interface and input handling
external/           Vendored cJSON and raygui dependencies
build/              Generated object and dependency files (ignored by Git)
config.ini          Window dimensions and target frame rate
test.json           Initial physics scene
```

Project headers use paths relative to `src`, such as `physics/body.h`.
The Makefile and `compile_flags.txt` both include `src` in the header search path.

## Build and run

Install GCC, Make, and raylib development files, then run from the project root:

```sh
make
./mistholer test.json
```

Pass the scene JSON path as the optional first argument:

```sh
./mistholer                         # Loads test.json
./mistholer scenes/example.json     # Loads a relative path
./mistholer /absolute/path/demo.json
./mistholer "scenes/my scene.json"   # Quote paths containing spaces
./mistholer --help
```

Relative paths are resolved from the current working directory. Run from the
project root to use its `config.ini`; the scene path does not change where
configuration is loaded from. A missing or invalid scene causes the program to
exit with an error before opening the window.

Use `make debug` for debug symbols and disabled optimizations,
or `make clean` to remove the executable and build artifacts. Regular and
debug builds keep separate object files under `build/release` and `build/debug`.

## Scene JSON format

A scene is a JSON object with two required fields:

| Field | Type | Meaning |
| --- | --- | --- |
| `gravity` | Number | Vertical acceleration in pixels per second squared. Positive values pull downward; `0` disables global gravity. |
| `bodies` | Array of objects | Initial bodies. An empty array is allowed. |

Each body supports the following fields:

| Field | Type | Required | Default / meaning |
| --- | --- | --- | --- |
| `position` | `[x, y]` | Yes | Center of the shape, in pixels. |
| `shape` | Object | Yes | One of the shape descriptions below. |
| `velocity` | `[vx, vy]` | No | Defaults to `[0, 0]`, in pixels per second. |
| `static` | Boolean | No | Defaults to `false`. When `true`, the body is immovable and its velocity is set to zero. Use for walls and floors. |
| `gravity_scale` | Number | No | Defaults to `1`. Multiplies global gravity for this body: `0` disables its gravity, `0.5` halves it, and `-1` reverses it. |

Coordinates start at the top-left corner of the window: X increases to the
right and Y increases downward. Vectors must contain exactly two finite numbers.
Other numeric values must also be finite. Field names are case-sensitive.

The loader currently supports two shapes:

| `shape.type` | Required dimensions | Meaning |
| --- | --- | --- |
| `"circle"` | `radius` | Circle radius in pixels. |
| `"aabb"` | `halfWidth`, `halfHeight` | Axis-aligned rectangle with full size `2 * halfWidth` by `2 * halfHeight`. |

Use positive dimensions for all shapes. For example, `halfWidth: 60` and
`halfHeight: 20` describe a rectangle 120 pixels wide and 40 pixels tall.
Polygon shapes are not supported by the JSON loader. Mass, density, restitution,
and rotation are not configurable through this format; unknown fields are ignored.

### Complete example

Save the following as `example.json` and run `./mistholer example.json`.
It creates a moving circle and a rectangle inside four static walls sized for
a 1920 × 1080 window:

```json
{
    "gravity": 980,
    "bodies": [
        {
            "position": [200, 150],
            "velocity": [80, 0],
            "gravity_scale": 1,
            "shape": { "type": "circle", "radius": 12 }
        },
        {
            "position": [400, 200],
            "shape": { "type": "aabb", "halfWidth": 15, "halfHeight": 10 }
        },
        {
            "position": [960, 1070],
            "static": true,
            "shape": { "type": "aabb", "halfWidth": 960, "halfHeight": 10 }
        },
        {
            "position": [960, 10],
            "static": true,
            "shape": { "type": "aabb", "halfWidth": 960, "halfHeight": 10 }
        },
        {
            "position": [10, 540],
            "static": true,
            "shape": { "type": "aabb", "halfWidth": 10, "halfHeight": 540 }
        },
        {
            "position": [1910, 540],
            "static": true,
            "shape": { "type": "aabb", "halfWidth": 10, "halfHeight": 540 }
        }
    ]
}
```

The JSON does not set the window dimensions or target frame rate; those remain
in `config.ini`. Window edges do not automatically act as walls, so add static
bodies explicitly when the scene needs a container.
