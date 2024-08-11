function love.load()
 
end
function love.draw()
  love.graphics.print("hello world", 100, 100)
  love.graphics.print("hello world2", 100, 120, 0, 1, 1, -10, 0)
  love.graphics.print("Hello world", 100, 140, 0, 1, 1, 0, 0, 1, 2)
  love.graphics.print("Hello world", 200, 300, 45)
  love.graphics.print("Hello world", 300, 300, -45, 2)
  love.graphics.print("Hello world", 400, 300, 45, 2, 3)

  love.graphics.printf("This text is aligned right, and wraps when it gets too big.", 25, 25, 125, "right")
  love.graphics.printf("This text is aligned center",400, 200, 200,"center") -- center your text around x = 200/2 + 100 = 200
  love.graphics.printf("This text is aligned right",400, 300, 200,"right") -- align right to x = 100 + 200 = 300
end