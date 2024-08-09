-- This function is called when the script is loaded
function mo.load()
    print("From mo.load()")
    
    hamster = mo.graphics.newImage("./data/bunny.jpg")

    angle = 0
end

-- This function is called every frame with the delta time
function mo.update(dt)
    print("From mo.update(dt)")
    
    
end

-- This function is called every frame to draw objects
function mo.draw()
    print("From mo.draw()")
    
    mo.graphics.drawSomething()
    mo.graphics.draw(hamster, 100, 100, angle, 50, 50, true)

    angle = angle + 1
end
