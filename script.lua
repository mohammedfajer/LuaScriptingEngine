

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
end

-- This function is called every frame with the delta time
function mo.update(dt)
   
end

-- This function is called every frame to draw objects
function mo.draw()
   mo.graphics.rectangle(100, 100, 20, 20, {r=255, g=0, b=0})
   mo.graphics.circle(100, 300, 20)
   
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
