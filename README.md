# 2D Modular Particle Engine

A lightweight, modular 2D particle engine built in C++ and OpenGL.

The engine is designed around a simple particle simulation pipeline where emitters create particles, forces influence them, the particle system manages their lifecycle, and a batched renderer efficiently draws thousands of particles per frame.

The project also includes an interactive sandbox/editor for experimenting with particle behavior in real time.

---

## Features

- Modular particle simulation architecture
- Multiple particle types
  - Solid
  - Liquid
  - Gas
- Configurable particle emitters
  - Point emitter
  - Line emitter
  - Area emitter
- Pluggable force system
  - Gravity
  - Buoyancy
  - Cursor attraction
  - Cursor repulsion
- Batched 2D rendering
- Orthographic 2D camera
- Particle pooling and recycling
- Runtime particle spawning and configuration
- ImGui-based debug interface
- Interactive mouse controls
- OpenGL 3.3 Core rendering
- Designed to handle large particle counts efficiently

---

## Overview

The engine follows a layered architecture:

<img width="4680" height="4830" alt="Particle_Engine" src="https://github.com/user-attachments/assets/d8bc4d80-807d-4c82-9c85-bfb806dd20a2" />
