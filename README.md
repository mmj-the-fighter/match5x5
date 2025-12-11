# Match 5x5 (A tile game for small kids)

This repository contains the core logic and design specification for a tile-matching puzzle game played on a 5×5 grid. This document describes the gameplay mechanics, matching rules, level-generation logic, and the mathematical bounds on possible level configurations.

---
## Build / Play the Game
````md
### How to Run (Visual Studio on Windows)

1. Navigate to the game’s source code folder containing the `CMakeLists.txt` file.  
2. Open a command prompt in that folder and run:
   ```bash
   mkdir build
   cd build
   cmake ../
````

3. Open the generated Visual Studio project.
4. Build all targets.
5. Copy `SDL3.dll` from:

   ```
   build/_deps/sdl3-build/Debug/
   ```

   into:

   ```
   build/
   ```
6. Run the `match_game` executable.

### How to Run (g++ on Linux / macOS)

1. Navigate to the game’s source code folder containing the `CMakeLists.txt` file.
2. Run the following:

   ```bash
   mkdir build
   cd build
   cmake ../
   make
   ./match_game
   ```

```
```
### Play in itch.io

If you can't generate exe from source code, you can play the game on itch.io.

[https://mmjthefighter.itch.io/match5x5](https://mmjthefighter.itch.io/match5x5)

---

## Game Basics

* The game uses **five colors**: *orange, brown, green, blue,* and *yellow*.
* It uses **five numbers**: *1, 2, 3, 4,* and *5*.
* The board is a **5×5 grid** consisting of **25 tiles**.
* Each tile has exactly **one color** and **one number** (analogous to one half of a domino).
* The player begins with a board containing a randomly arranged set of these tiles.

---

## Matching Rules

* The player may **click and drag a tile** across the board.
* Any tile the dragged tile passes over will be **removed** if it qualifies as a matching tile.
* A tile is considered a **matching tile** if:

  * It has the **same color**, or
  * It has the **same number** as the tile being dragged.
* If *any* tile is matched during the drag, the original clicked tile is removed as well.
* The objective is to **clear the board completely**.
* When the board is cleared:

  * A **new solvable level** is generated.
  * The player proceeds to the next level.
* The player's **score** equals the number of boards successfully cleared.
* The game ends if the board reaches a state where **at least one tile cannot be cleared**.

---

## Level Generation

The level generator ensures that **every level is solvable**.

### Core Idea

Every tile placed onto the board is guaranteed to have at least one other tile that can remove it.

---

## Level Generation Logic (Algorithm 0, With Repetition Allowed)

Below are the helper routines used in generation:

* **gen**
  Generates one random tile out of the 25 possible color–number combinations.

* **match-color-tiles(tile)**
  Produces all tiles sharing the **same color** as the input tile.

* **match-number-tiles(tile)**
  Produces all tiles sharing the **same number** as the input tile.

* **combine**
  Combines both match sets into a single set (duplicates removed).

* **pick(set)**
  Selects a single matching tile from the combined set.

### Result Guarantee

For every tile created by `gen`, the tile chosen by `pick` is guaranteed to be able to eliminate it.

---

### Example

Given:

```
gen → (1, orange)
```

**match-color tiles:**

```
(1, orange)
(2, orange)
(3, orange)
(4, orange)
(5, orange)
```

**match-number tiles:**

```
(1, orange)
(1, brown)
(1, green)
(1, blue)
(1, yellow)
```

**pick →** for example:

```
(1, yellow)
```

This ensures `(1, yellow)` can always eliminate `(1, orange)`.

---

## Level Generation Algorithm

```
// Generate 24 tiles in solvable pairs
for 12 times do
    set.clear()
    genTile  = gen()
    set.add(match-color-tiles(genTile))
    set.add(match-number-tiles(genTile))
    pickTile = pick(set)

    level.tiles.push(genTile)
    level.tiles.push(pickTile)
end for

// The 25th tile must match at least one of the existing 24 tiles
set.clear()
for each tile in level.tiles do
    set.add(match-color-tiles(tile))
    set.add(match-number-tiles(tile))
end for
lastTile = pick(set)
level.tiles.push(lastTile)

// Increase difficulty with shuffling
level.tiles.shuffle()

game.addLevel(level)
```

---

## Mathematical Analysis

### Maximum Arrangements for the First 24 Tiles

Each iteration produces:

* One arbitrary tile: **25 choices**
* One matching tile: **9 possible matches**
  (5 same-color + 5 same-number − 1 overlap = 9)

Since 12 such pairs are generated:

```
Maximum arrangements for 24 tiles = 25^12 × 9^12
```

### Arrangements for the 25th Tile

The 25th tile must be a matching tile of **any of the 24 tiles**.
Each tile has **9** possible matches:

```
Maximum arrangements for 25th tile = Σ (i = 1 to 24) of 9 = 24 × 9
```

### Total Level Arrangements

```
Total arrangements =
(25^12 × 9^12) × (24 × 9)
```
### Note
This README has been made using AI with the input of the original manuscript.