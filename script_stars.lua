


local screen_width, screen_height = mo.graphics.getWindowDimensions()

 local max_stars = 100
    stars = {}

function mo.load()
    -- Initialization code, if any
    print("Lua mo.load Call")

  

    for i = 1, max_stars do 
        local x = math.random(5, screen_width - 5)
        local y = math.random(6, screen_height - 5)
        stars[i] = {x, y}
    end

end

function mo.update(dt)
    -- Update logic (e.g., handle input or move objects)
    print("Lua mo.update(dt): " .. dt)

 
    
  

     local speed = 1  -- Speed of the star movement (pixels per second)

     for i = 1, #stars do
       local star = stars[i]
        local dx = speed * dt * (math.random() - 0.5)
        local dy = speed * dt * (math.random() - 0.5)
        star[1] = star[1] + dx
        star[2] = star[2] + dy

        -- Wrap around screen edges
        if star[1] < 0 then star[1] = screen_width end
        if star[1] > screen_width then star[1] = 0 end
        if star[2] < 0 then star[2] = screen_height end
        if star[2] > screen_height then star[2] = 0 end

        -- Debug output
        print("Star", i, "Position:", star[1], star[2])
    end
end

function mo.draw()
 -- graphics.setBackgroundColor(0, 123, 239, 255)
    mo.graphics.setBackgroundColor(0, 0, math.random(0, 255), 255)

      -- Draw a Single Point 
     mo.graphics.setDrawColor(0, 255, 0, 255)
    mo.graphics.point(100, 100)
    
     local integerStars = {}
    for i = 1, #stars do
        local star = stars[i]
        integerStars[i] = {math.floor(star[1] + 0.5), math.floor(star[2] + 0.5)}  -- Round to nearest integer
    end

   mo.graphics.setDrawColor(math.random(0, 255),math.random(0, 255),math.random(0, 255),math.random(0, 255))
   mo.graphics.drawPointsWithSize(integerStars, math.random(0, 10))

end
