# 3d World

## Overview
This Arduino sketch is designed to run on the Arduboy handheld gaming device. It implements a basic 2D raycasting engine, similar to early first-person shooters, and includes a particle system. The code is based on the raycasting tutorial from [lodev.org](http://lodev.org/cgtutor/raycasting.html).

## Features
- 2D raycasting engine for rendering a simple maze-like environment.
- Particle system with basic physics.
- User input handling for movement and particle generation.
- Procedural map generation based on a hashing function.

## Movement and Controls
- Use the UP and DOWN buttons to move forward and backward.
- Use the LEFT and RIGHT buttons to rotate the view.
- Press the A button to generate particles.

## Particle System
- Particles are generated at the player's position and move in the direction of the player's view.
- Each particle has a limited lifespan, decrementing each frame.
- Particles are rendered as small squares on the screen.

## Additional Notes
- Collision detection with the map boundaries is implemented.
- The game utilizes the Arduboy's 128x64 pixel monochrome display.

## Legal
Copyright (C) 2023 ladroid.

This code is [BSD-3-Clause Licensed](https://github.com/ladroid/The-hunt-has-begun/blob/master/LICENSE.md).