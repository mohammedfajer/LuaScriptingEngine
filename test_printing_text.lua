
WINDOW_WIDTH = 1280
WINDOW_HEIGHT = 720
 
angle = 0

-- This function is called when the script is loaded
function mo.load()
   print("mo.load() called from c++")
   mo.window.setTitle('Pong')
    -- Assuming push is bound and available
    push:setupScreen(1280, 720, 1280, 720, {
        fullscreen = false,
        resizable = false,
        vsync = false,
        title = "My SFML Window"
    })
    smallFont = mo.graphics.newFont("./data/arial.ttf", 32)
    largeFont = mo.graphics.newFont("./data/arial.ttf", 64)
    scoreFont = mo.graphics.newFont("./data/arial.ttf", 128)
 
     -- Set active fonts
    mo.graphics.setFont(scoreFont)


local transform = Transform.new()
transform:translate(20, 20)
transform:scale(2, 2)
transform:rotate(45)

-- Get and print the transformation matrix
local matrix = transform:getMatrix()

-- Print matrix values in a readable 3x3 format
io.write(matrix.a, " ", matrix.b, " ", matrix.c, "\n")
io.write(matrix.d, " ", matrix.e, " ", matrix.f, "\n")
io.write(matrix.g, " ", matrix.h, " ", matrix.i, "\n")

-- Combine with another transform
local otherTransform = Transform.new()
otherTransform:translate(10, 10)  -- Example transformation
transform:combine(otherTransform)
-- Transform a point
local point = transform:transformPoint(50, 50)
print("Transformed point: x =", point.x, ", y =", point.y)


end

-- This function is called every frame with the delta time
function mo.update(dt)
    angle = angle + 100 * dt
end

-- This function is called every frame to draw objects
function mo.draw()
     push:start()
     mo.graphics.clear({r=255, g=34, b=0})
  
    -- end our drawing to push
    mo.graphics.setFont(smallFont)

    mo.graphics.print("Hello", 10, 10)

    mo.graphics.print("Hi", 55, 40, angle)
    mo.graphics.print("Hi", 180, 40, 0, 2)
    mo.graphics.print("Hi", 280, 40, 0, 1, 1, 0, 0, 0, 1)

    mo.graphics.drawText("Hllo", 100, 200)

    mo.graphics.print("WITH FONTS BABY", largeFont, 400, 50)


    local textTransform = Transform.new()
    textTransform:translate(10, 500)  -- Example transformation
   
   --- mo.graphics.print("TRANSFORM", textTransform)

    mo.graphics.print("TRANSFORM", largeFont, textTransform)

    push:finish()
end

--[[
	Keyboard handling, called my MO2D each frame;
	passes in the key we pressed so we can access.
]]
function mo.keypressed(key)
end

function mo.resize(w, h)
    push:resize(w, h)
end 