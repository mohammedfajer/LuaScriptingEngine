

WINDOW_WIDTH = 1280
WINDOW_HEIGHT = 720


-- This function is called when the script is loaded
function mo.load()
    
    mo.window.setMode(WINDOW_WIDTH, WINDOW_HEIGHT, {
        fullscreen = false,
        resizable = false,
        vsync = true
    })

   

   mo.window.setTitle('Pong')


	-- Load font objects
	smallFont = mo.graphics.newFont('./data/arial.ttf', 8)
	largeFont = mo.graphics.newFont('./data/arial.ttf', 16)
	scoreFont = mo.graphics.newFont('./data/arial.ttf', 32)

end

-- This function is called every frame with the delta time
function mo.update(dt)
   
end

-- This function is called every frame to draw objects
function mo.draw()

    print("mo.draw()")

   mo.graphics.rectangle(100, 100, 20, 20, {r=255, g=0, b=0})
   mo.graphics.circle(100, 300, 20)


    -- Set the active font to smallFont
    mo.graphics.setFont(smallFont)

    -- Draw some text using the active font
    mo.graphics.drawText("Hello, World!", 100, 100)

    -- Optionally, switch to largeFont and draw more text
    mo.graphics.setFont(largeFont)
    mo.graphics.drawText("Large Font Text", 100, 150)

    -- Optionally, switch to scoreFont and draw more text
    mo.graphics.setFont(scoreFont)
    mo.graphics.drawText("Score: 123", 100, 200)
   
end

--[[
	Keyboard handling, called my MO2D each frame;
	passes in the key we pressed so we can access.
]]

function mo.keypressed(key)
	if key == 'A' then 
		print("Key A pressed")
	end

	if key == 'escape' then
		mo.event.quit()
	end
end
