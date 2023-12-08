local function gcd(x, y)
	if (y == 0) then return x
	else return gcd(y, x%y) end
end

local function lcm(x,y)
	return x*y/gcd(x,y)
end

local file = io.open("input.txt", "r")

if file then


	local instructions = file:read("*l")
	local network = {}

	for line in file:lines() do
		if (line:find("%a")) then
			local state = line:sub(1, 3)
			local left = line:sub(8, 10)
			local right = line:sub(13, 15)

			network[state] = {["L"] = left, ["R"] = right}
		end
	end

	-- Part 1
	local current_state = "AAA"
	local steps = 0
	while not (current_state == "ZZZ") do
		local ip = steps%(#instructions) + 1
		current_state = network[current_state][instructions:sub(ip, ip)]
		steps = steps + 1
	end
	print("Part 1 answer is "..steps)

	-- Part 2
	local current_states = {"MCA", "AAA", "DCA", "LGA", "NLA", "VPA"}
	local end_states = {{},{},{},{},{},{}}
	steps = 0
	while (steps < 100000) do
		local ip = steps%(#instructions) + 1

		current_state = network[current_state][instructions:sub(ip, ip)]
		for k, state in pairs(current_states) do
			current_states[k] = network[state][instructions:sub(ip,ip)]

			if (current_states[k]:match("%a%aZ")) then
				table.insert(end_states[k],
					{["state"] = current_states[k], ["start"] = k, ["steps"] = steps + 1})
			end
		end

		steps = steps + 1
	end

	local res = math.floor(lcm(lcm(end_states[1][1].steps,lcm(end_states[2][1].steps,end_states[6][1].steps)),lcm(end_states[3][1].steps,lcm(end_states[4][1].steps,end_states[5][1].steps))))

	print("Part 2 answer is "..res)

else

end
