

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
    if mo.keyboard.isKeyDown(KeyCode.KEY_A) then
        print("A was pressed")
        x = x + 1
    end


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
