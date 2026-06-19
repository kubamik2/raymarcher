# Raymarcher
Simple 3D mathematical shapes editor using raymarching.

## Running
`cmake -S . -B build` \
`cmake --build build && ./build/raymarcher`

## Usage
### Controls
- wasd, space, shift - move the origin of the camera
- hold left mouse button + mouse movement - move the camera around center
- scroll - zoom in/out
### Tree Shape window
Allows viewieng object hierarchy tree. Right click opens up a menu with options to add certain shapes or delete an object. Clicking on an object opens up the Shape Menu.
### Shape Menu window
Allows modifying properties of the selected object, such as it's:
    - translation
    - rotation
    - scale
    - operation type
    - order
    - and possibly more depending on the shape
Order of an object determines how the operations will be folded together. Operations are executed from lowest order to highest.

## Libraries
- GLFW
- Dear ImGui
- GLAD
- OpenGL
