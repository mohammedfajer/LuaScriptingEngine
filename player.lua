-- Define the Player module
local Player = {}
Player.__index = Player

function Player.new(name, score)
    local self = setmetatable({}, Player)
    self.name = name or "Unknown"
    self.score = score or 0
    return self
end

function Player:getName()
    return self.name
end

function Player:getScore()
    return self.score
end

function Player:setName(name)
    self.name = name
end

function Player:setScore(score)
    self.score = score
end

return Player
