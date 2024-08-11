
Player = require('player')

WINDOW_WIDTH = 1280
WINDOW_HEIGHT = 720


-- This function is called when the script is loaded
function mo.load()
    
    print("mo.load() called from c++")

   --[[ mo.window.setMode(WINDOW_WIDTH, WINDOW_HEIGHT, {
        fullscreen = false,
        resizable = false,
        vsync = true
    })]]

    print('here1')
  
  

   mo.window.setTitle('Pong')

     print('here2')
    -- Assuming push is bound and available
    push:setupScreen(10, 10, 700, 700, {
        fullscreen = false,
        resizable = true,
        vsync = true,
        title = "My SFML Window"
    })
    print('here3')

	smallFont = mo.graphics.newFont("./data/arial.ttf", 32)
	largeFont = mo.graphics.newFont("./data/arial.ttf", 64)
	scoreFont = mo.graphics.newFont("./data/arial.ttf", 128)
   


   -- Check if fonts are created
    if not smallFont then
        print("Failed to create smallFont")
    end

    if not largeFont then
        print("Failed to create largeFont")
    end

    if not scoreFont then
        print("Failed to create scoreFont")
    end

     -- Set active fonts
    mo.graphics.setFont(scoreFont)
    print('jere4')

   
end

-- This function is called every frame with the delta time
function mo.update(dt)
   
end

-- This function is called every frame to draw objects
function mo.draw()
    
     push:start()
    
  --  print("mo.draw()")

   mo.graphics.rectangle(0, 0, 1, 1, {r=255, g=0, b=0})
 


    -- end our drawing to push
    push:finish()
   
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
