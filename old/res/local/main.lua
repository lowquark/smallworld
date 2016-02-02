-- Random naming stuff

evilLanguage = {}

evilLanguage.vowels = {"aa", "ae", "ai", "a", "ee", "e", "ui", "u", "oo", "oi", "ou", "o"}
evilLanguage.vowelCount = 12
evilLanguage.consonants = {"r", "dr", "n", "z", "c", "k", "sch", "sh", "l", "th", "m", "y"}
evilLanguage.consonantCount = 12

evilAdjectives = {"Rabid", "Mad", "Insane", "Bloody", "Rampant", "Crazed", "Cunning", "Livid", "Shunning", "Lucid"}
evilTitles = {"King", "Eater", "Slayer", "Killer", "Butcher", "Doombringer", "Decimator", "Destroyer"}
evilNouns = {"Demons", "Worlds", "Kobolds", "Hounds", "Planets", "the Universe", "Man"}

function getRandomWord(language)
	local word = ""
	for i = 1,math.random(3, 5) do
		local segment
		if i % 2 == 0 then
			segment = language.vowels[math.random(language.vowelCount)]
		else
			segment = language.consonants[math.random(language.consonantCount)]
		end
		word = word .. segment
	end
	return word
end

function getRandomEvilTitle()
	local title = ", the "

	local adjective = math.random(3) == 1
	if adjective then
		title = title .. evilAdjectives[math.random(10)] .. " "
	end

	title = title .. evilTitles[math.random(8)] .. " of " .. evilNouns[math.random(7)]

	return title
end

function capitalize(word)
	return word:gsub("%a", string.upper, 1)
end

-- Game stuff

--[[
function Agent:setAsPlayer(player)
	self.strength = 5000
	self.player = player
	self.update = function()
		local force = self.strength

		if self.player:isKeyDown("lshift") then
			force = self.strength*1.3
			self:setMaxSpeed(10)
		else
			self:setMaxSpeed(4)
		end

		local move = false
		if self.player:isKeyDown("w") then self:push( 0,  force, 0) move = true end
		if self.player:isKeyDown("a") then self:push(-force,  0, 0) move = true end
		if self.player:isKeyDown("s") then self:push( 0, -force, 0) move = true end
		if self.player:isKeyDown("d") then self:push( force,  0, 0) move = true end

		if not move then
			self:dampen(self.strength)
		end

		if self.player:isKeyDown("space") then self:jump(force*3/10) end
	end
end
function Agent:setAsFollower(leader)
	self.strength = 5000
	self.leader = leader
	self.update = function()
		local x, y, z = self:getPosition()
		local targetX, targetY, targetZ = leader:getPosition()

		local dir = {}

		dir.x = targetX - x
		dir.y = targetY - y

		local magnitude = math.sqrt(dir.x*dir.x + dir.y*dir.y)

		if magnitude > 1 then
			dir.x = dir.x/magnitude --Normalize the direction
			dir.y = dir.y/magnitude

			self:push(self.strength*dir.x, self.strength*dir.y, 0) --Push in that direction
		else
			self:dampen(self.strength)
		end

		if targetZ > z + 1 then
			self:jump(self.strength*3/10)
		end
	end
end
function Agent:setAsNPC()
	self.strength = 5000
	self.target = {}
	self.target.x, self.target.y = self:getPosition()
	self:setMaxHealth(math.random(80, 200))
	self:heal()
	--print(self:getName() .. " was given " .. self:getMaxHealth() .. " health.")

	self.update = function()
		if math.random(200) == 1 then
			self.target.x = math.random(-15, 15)
			self.target.y = math.random(-15, 15)
		end

		local dir = {}

		local x, y, z = self:getPosition()

		--Find the difference
		dir.x = self.target.x - x
		dir.y = self.target.y - y

		local magnitude = math.sqrt(dir.x*dir.x + dir.y*dir.y)

		if magnitude > 1 then
			dir.x = dir.x/magnitude --Normalize the direction
			dir.y = dir.y/magnitude

			self:push(self.strength*dir.x, self.strength*dir.y, 0) --Push in that direction
		else
			self:dampen(self.strength) --Stop moving, because we're close enough
			self:jump(1500) --Jump!
		end
	end
	self.setTarget = function(x, y)
	end
end
function Agent:update()
end
]]--

--[[
function Game:onPlayerJoined(player)
	print(player:getName() .. " has joined the game!")

	--local playerAgent = self.world:addAgent(player:getName(), 0, 0, 10)
	playerAgent:setAsPlayer(player)

	local previousLeader = playerAgent;
	for i = 1,50 do
		local follower = self.world:addAgent(player:getName() .. " Junior", 0, 0, 10)
		follower:setAsFollower(previousLeader)
		previousLeader = follower;
	end

	player:follow(playerAgent);
end
]]

--game:setName("Dave's Test World")

--[[
math.randomseed(os.time())
for i = 1,10 do
	local x = math.random(-15, 15)
	local y = math.random(-15, 15)
	local z = 0

	local npc = game.world:addAgent(capitalize(getRandomWord(evilLanguage)), x, y, z)
	npc:setAsNPC()
end
]]
