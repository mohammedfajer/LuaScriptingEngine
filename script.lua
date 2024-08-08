


function mo.load()
    -- Initialization code, if any
    print("Lua mo.load Call")

    texture = mo.graphics.newImage("./data/bunny.jpg")
        -- Create a quad for the image
     quadId = mo.graphics.newQuad(10, 10, 512, 512)  -- 1 is the imageId
end

function mo.update(dt)
    -- Update logic (e.g., handle input or move objects)
    print("Lua mo.update(dt): " .. dt)
end



function mo.draw()
 
    mo.graphics.setBackgroundColor(0, 0,255, 255)
    
    -- Draw the texture at (50, 50)


    -- mo.graphics.draw(texture, 0, 0)

    
   -- Draw the texture at (100, 100) with rotation and scaling
   -- mo.graphics.draw(texture, 100, 100, 0, 1, 1)




    -- Draw using the quad
    mo.graphics.draw(texture, quadId, 200, 200)

end
