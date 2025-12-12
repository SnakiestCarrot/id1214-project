# Neuroevolution for Snake

This is a project for a course at KTH Royal Institute of Technology: **ID1214 Artificial Intelligence and Applied Methods**.

We were able to freely choose a project to implement in the field of AI. We chose to do a neuroevolution project with the classic game Snake.

We implemented the **Neural Networks** from scratch ourselves, along with a **Genetic Algorithm** that simulates crossover, mutation, and elitism.

## Controls
* **M**: Menu (Reset / Adjust Settings)
* **V**: Visualize Mode (Watch the best snake play)
* **T**: Train Mode (Fast-forward training)

## Requirements

The project is designed for **Linux**. It is possible to get it working for windows but you will need to link SDL2 yourself.

* **C++ Compiler**: GCC (`g++`) or Clang with support for **C++17** or higher.
* **GNU Make**: To use the provided makefile.
* **SDL2**: The core development libraries for Simple DirectMedia Layer.

### Installing Dependencies

Ubuntu / Debian
```bash
sudo apt-get install libsdl2-dev
```

Arch Linux:
```bash
sudo pacman -S base-devel sdl2
```

Fedora:
```bash
sudo dnf install @development-tools SDL2-devel
```

### Building & Running

```bash
make
./bin/release/main
```

## Highlights

### Neural Network
- Developed by us from scratch.
- 11 Inputs (Vision & Orientation)
- 3 Outputs (left, right, forwards)
- RELU, Sigmoid and Tanh activation functions.
- Adjustable hidden nodes.

### Inputs
The snake "sees" the world through 11 normalized values (0 or 1):
**Danger Detection (Relative to Head)**
- Danger Left: Is there a wall or tail body part immediately to the left of my head?
- Danger Straight: Is there a wall or tail body part immediately in front of me?
- Danger Right: Is there a wall or tail body part immediately to the right of my head?

**Food Direction (Relative to Head)**
- Food is Left: Is the food somewhere to the left side of my current path?
- Food is Right: Is the food somewhere to the right side of my current path?
- Food is Straight: Is the food directly ahead of me (in the cone of vision)?
- Food is Behind: Is the food behind me?

**Tail Direction (Absolute / Compass)**
- Tail is West: Is the tail tip to the left (screen X) of the head?
- Tail is East: Is the tail tip to the right (screen X) of the head?
- Tail is North: Is the tail tip above (screen Y) the head?
- Tail is South: Is the tail tip below (screen Y) the head?

### Genetic Algorithm
- Developed by us from scratch
- Elitism: The best snake from the previous generation is copied over unchanged to prevent regression.
- Mutation: Random adjustments to weights to introduce new behaviors.
- Crossover: Combining the "genes" of two parent snakes to create offspring.
