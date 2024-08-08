
function mo.load()
    -- Initialization code, if any
    print("Lua mo.load Call")

end

function mo.update(dt)
    -- Update logic (e.g., handle input or move objects)
    print("Lua mo.update(dt): " .. dt)
end

function drawPolygon(...)
    local points = {...}
    local formattedPoints = {}
    for i, p in ipairs(points) do
        table.insert(formattedPoints, p)
    end
    mo.graphics.drawPolygon(table.unpack(formattedPoints))
end


-- Utility function to round numbers
function round(n)
    return math.floor(n + 0.5)
end

-- Draw a star with a specified center, radius, and number of points
function drawStar(centerX, centerY, radius, pointsCount)
    local starPoints = {}
    local innerRadius = radius / 2
    local angleStep = math.pi / pointsCount

    for i = 0, pointsCount * 2 - 1 do
        local angle = i * angleStep
        local isOuter = i % 2 == 0
        local currentRadius = isOuter and radius or innerRadius
        local x = centerX + currentRadius * math.cos(angle)
        local y = centerY + currentRadius * math.sin(angle)
        table.insert(starPoints, round(x))
        table.insert(starPoints, round(y))
    end

    -- Close the star by connecting the last point to the first
    table.insert(starPoints, starPoints[1])
    table.insert(starPoints, starPoints[2])

    -- Draw the star
    mo.graphics.drawPolygon(table.unpack(starPoints))
end

-- Draw a regular polygon (e.g., triangle, pentagon)
function drawPolygonN(sides, centerX, centerY, radius)
    local points = {}
    local angleStep = 2 * math.pi / sides
    
    for i = 0, sides - 1 do
        local angle = i * angleStep
        local x = centerX + radius * math.cos(angle)
        local y = centerY + radius * math.sin(angle)
        table.insert(points, round(x))
        table.insert(points, round(y))
    end

    -- Close the polygon by connecting the last point to the first
    table.insert(points, points[1])
    table.insert(points, points[2])

    -- Draw the polygon
    mo.graphics.drawPolygon(table.unpack(points))
end

function mo.draw()
 -- graphics.setBackgroundColor(0, 123, 239, 255)
    mo.graphics.setBackgroundColor(0, 0,255, 255)


  

    mo.graphics.setDrawColor(255,0,0,255)
    drawPolygon(100, 100, 200, 100, 150, 200)

   drawStar(500, 300, 100, 5) -- Draw a star with 5 points


   mo.graphics.setDrawColor(0, 255, 0, 255) -- Green color
    drawPolygonN(10, 300, 200, 100) -- Draw a pentagon with center (300, 200) and radius 100


    
end
