# OpenGL FPS Demo � Project Explanation

## Overview
This project is a **C++ OpenGL mini-engine and FPS-style demo** built on **OpenGL 3.3 Core**.  
It demonstrates real-time rendering, point-light shadow mapping, FPS camera controls, shooting mechanics, UI rendering, and debugging with ImGui.

---

## Main Features
- First-person camera (mouse + keyboard)
- Point-light shadow mapping using cubemaps
- Multiple dynamic lights
- 3D model loading and rendering
- Gun shooting animation with muzzle flash
- Ammo counter rendered as textured text
- 2D UI rendering (crosshair, HUD)
- Real-time debugging with ImGui

---

## Core Systems

### Rendering System
- Forward rendering pipeline
- Two-pass rendering:
  1. Shadow pass (depth cubemap per light)
  2. Lighting pass (scene rendering with shadows)

Key classes:
- Renderer
- Shader
- Mesh
- Model
- Texture
- CubeMap

---

### Entity System
All objects are based on a transform-driven `Entity` system.

**Entity**
- Position, rotation (yaw/pitch), scale
- Direction vector
- Movement helpers

**Derived entities**
- PhysicalEntity � 3D world objects
- Camera � FPS camera
- Light � Point light with shadow cubemap
- UIEntity � 2D screen-space objects
- TextEntity � Text logic (ammo counter)

---

### Camera & Controls
- Mouse controls yaw and pitch
- Keyboard:
  - W / A / S / D � Move
  - Left Mouse � Shoot
  - ESC � Exit

Camera direction is calculated using yaw/pitch angles.

---

### Shooting Mechanics
When firing:
- Ammo decreases
- Gun animation starts
- Muzzle flash appears
- A short-lived point light simulates gunfire

Gun position and rotation follow the camera every frame.

---

### Lighting & Shadows
- Omnidirectional point lights
- Shadow mapping via depth cubemaps
- Geometry shader renders depth to 6 faces
- Enables realistic point-light shadows

---

### UI & Text Rendering
- Orthographic projection for UI
- Text rendered using a texture atlas
- Each digit updates UV coordinates dynamically

Used for:
- Crosshair
- Ammo counter

---

### Debugging with ImGui
ImGui panel allows:
- Camera pitch/yaw control
- Light position editing
- Real-time inspection

---

## Project Purpose
This project is a **learning-focused OpenGL FPS demo / mini engine** designed to practice:
- Modern OpenGL rendering
- Engine-style architecture
- Shadow mapping techniques
- FPS camera and interaction logic