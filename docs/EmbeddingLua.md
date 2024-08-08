

# Embedding Lua

- create lua state and execute a lua code inline
- explanation of the types in lua
- explanation of the lua stack
- get a global variable from lua
- set a global variable to lua
- call a lua function
- call a lua function and get the result(s)
- call a lua function with parameters and get the result(s)
- call a c function form lua
- call a c function from lua with a return value
- call a c functiom from lua with a return value and paramters
- giving data to lua via globals and the registry
- calling c functions with upvalues
- userdata / lightuserdata
- creating your own type for lua
- metatables
- uservalues
- getting values from a table in lua
- setting values in a table in lua
- 



 # Timer Module

```lua
   -- Update frame timing
   mo.timer.step()

   -- Get delta time
   local delta = mo.timer.getDelta()
   print("Delta time: " .. delta .. " ms")

   -- Get average delta time
   local avgDelta = mo.timer.getAverageDelta()
   print("Average delta time: " .. avgDelta .. " ms")

   -- Get current FPS
   local fps = mo.timer.getFPS()
   print("FPS: "  .. fps)

   -- Get current time
   local time = mo.timer.getTime()
   print("Current time: " .. time .. " seconds since epoch")

   -- Sleep for 1000ms (1 second)
   mo.timer.sleep(1000)
```

- 

# Graphics

- [x] Draw a line between two points `drawLine`
- [x] Set Render Draw Colour `setDrawColor`
- [x] Draw point, points, and a polygon
- draws text on screen, if no font is set, one will be created and set (once) if needed
- draws a formatted text, with word wrap and alignment
- draws a quadrilateral shape
- draws rectangle
- draws a triangle
- draws ellipse
- draw (overload) with textures, rotation, scale, pivot etc...

optional

- sfml or raylib with shader support
- sprite batching



200 200 100 100



Topics and Systems

- shapes
- sprites

- sprite sheet animation
- tile maps
- camera system, screen shake
- 2d game math
- audio
- particle system
- lighting and shader coding
- easing functions
- physics and collisions
- path finding algorithms
- ai, behaviour trees
- networking basics
- state machines
- algorithms, data structure, design patterns
- profiling and optimization
- 