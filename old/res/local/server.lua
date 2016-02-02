function server.allowJoin(player)
	local reject = false
	local message = ""

	--[[
	if player.name == "lol" then
		io.write(player.name .. " is a stupid name. ")

		reject = true
		message = "stupid name"
	end
	]]

	--[[
	if player.ip == "127.0.0.1" then
		print(player.name .. " must be the host!")

		reject = false
		message = "greetings"
	end
	]]

	if reject then
		print("Rejecting...")
	end

	return not reject, message
end

function server.onJoin(playerInfo)
	print(playerInfo.name .. " was allowed to join.")

	--agent = game.world:addAgent(playerInfo.name, 0, 0, 0)
	--agent:setAsPlayer()
	--player.agent = agent
end
