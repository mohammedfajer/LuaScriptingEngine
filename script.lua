
Player = require('player')

WINDOW_WIDTH = 1280
WINDOW_HEIGHT = 720

  local angle = 0
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
    push:setupScreen(1280, 720, 1280, 720, {
        fullscreen = false,
        resizable = false,
        vsync = false,
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

    --[[
    buffer = mo.audio.SoundBuffer.new()
    buffer:loadFromFile("./data/soundEffect.mp3")

    sound = mo.audio.Sound.new()
    sound:setBuffer(buffer)
    sound:play()

    music = mo.audio.Music.new()
    music:openFromFile("./data/music.mp3")
    music:setLoop(true)
    music:play()]]


    soundEffect = mo.audio.newSource("./data/soundEffect.mp3", 'static')
    soundEffect:play()

     musicTrack = mo.audio.newSource("./data/music.mp3", 'stream')
    musicTrack:setLoop(true)
    musicTrack:play()

    musicPlaying = true
end

-- This function is called every frame with the delta time
function mo.update(dt)
    
    mo.timer:step()
   
    -- Access the timer module via love.timer
    fps = mo.timer:getFPS()
    print("Current FPS:", fps)

    delta = mo.timer:getDelta()
    print("Time between last two frames:", delta)

   -- mo.timer:sleep(0.1) -- Sleep for 100 milliseconds

    angle = angle + 100 * delta

end

-- This function is called every frame to draw objects
function mo.draw()
     push:start()
     mo.graphics.clear({r=255, g=0, b=0})
  --  print("mo.draw()")
    mo.graphics.rectangle(2, 2, 20, 20, {r=255, g=0, b=0})
    -- end our drawing to push
    mo.graphics.setFont(smallFont)

    mo.graphics.print("Hello", 10, 10)


 
  

    mo.graphics.print("Hi", 55, 40, angle)
    mo.graphics.print("Hi", 180, 40, 0, 2)
    mo.graphics.print("Hi", 280, 40, 0, 1, 1, 0, 0, 0, 1)

    mo.graphics.drawText("Hllo", 100, 200)
   
    push:finish()
end

--[[
	Keyboard handling, called my MO2D each frame;
	passes in the key we pressed so we can access.
]]
function mo.keypressed(key)
	if key == 'A' then 
		print("Key A pressed")
         soundEffect:play()
       
          if musicPlaying then 
                musicTrack:pause()
                musicPlaying = false
            else
                musicTrack:play()
                musicPlaying = true
            end
	end

    

	if key == 'escape' then
		mo.event.quit()
       
	end
end

function mo.resize(w, h)
    push:resize(w, h)
end 