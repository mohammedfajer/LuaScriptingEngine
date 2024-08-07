-- Define the `mo` table
-- mo = {}


-- Import the custom graphics library
-- local graphics = require("graphics")
-- Load the keyboard library
local keyboard = require("keyboard")

local player = require("player")

local x = 50

function mo.load()
    -- Initialization code, if any
    print("Lua mo.load Call")

    mo.graphics.setBackgroundColor(12, 100, 239, 255)

    player.load()

    bunny = mo.graphics.newImage("data/bunny.jpg")

    top_left = mo.graphics.newQuad(0, 0, 512, 512, bunny)

end

function mo.update(dt)
    -- Update logic (e.g., handle input or move objects)
  --  print("Lua mo.update(dt): " .. dt)


  

    -- Use SDL Keycode for 'A' (typically SDLK_a in SDL)
    

    -- Check if 'A' is pressed
    if keyboard.isKeyDown(KeyCode.KEY_A) then
        print("A was pressed")
        x = x + 1
    end

end

function mo.draw()
 -- graphics.setBackgroundColor(0, 123, 239, 255)
    mo.graphics.setBackgroundColor(12, 100, 239, 255)

    -- Draw a filled rectangle at coordinates (20, 50) with width 60 and height 120


    mo.graphics.rectangle("fill", x, 50, 60, 120)

    mo.graphics.rectangle("line", 100, 100, 50, 100)


    mo.graphics.draw(bunny, 0, 0)
   --- graphics.drawQuad(bunny, top_left, 100, 50)

end
