-- Part 1
local function isPossible(s)
	local game_num = string.match(s, "Game %d+:")
	game_num = tonumber(string.match(game_num, "%d+"))
	s = string.sub(s, string.find(s,":"), string.len(s))

	for rd in string.gmatch(s, "%d+ red") do
		local r = tonumber(rd:match("%d+"))
		if (r > 12) then
			return false, game_num
		end
	end

	for gd in string.gmatch(s, "%d+ green") do
		local g = tonumber(gd:match("%d+"))
		if (g > 13) then
			return false, game_num
		end
	end

	for bd in string.gmatch(s, "%d+ blue") do
		local b = tonumber(bd:match("%d+"))
		if (b > 14) then
			return false, game_num
		end
	end
	return true, game_num
end

-- Part 2
local function powerMinCubes(s)

	s = string.sub(s, string.find(s,":"), string.len(s))

	local minr, ming, minb = 0, 0, 0

	for rd in string.gmatch(s, "%d+ red") do
		local r = tonumber(rd:match("%d+"))
		minr = math.max(r, minr)
	end

	for gd in string.gmatch(s, "%d+ green") do
		local g = tonumber(gd:match("%d+"))
		ming = math.max(g, ming)
	end

	for bd in string.gmatch(s, "%d+ blue") do
		local b = tonumber(bd:match("%d+"))
		minb = math.max(b, minb)
	end
	return minr*ming*minb
end


local file = io.open("input.txt", "r")

if file then

	local sum_part1 = 0
	local sum_part2 = 0
	for line in file:lines() do
		local t, n = isPossible(line)
		local pmc = powerMinCubes(line)
		sum_part2 = sum_part2 + pmc
		if (t) then
			sum_part1 = sum_part1 + n
		end
	end

	print("Part 1 answer is "..sum_part1)
	print("Part 2 answer is "..sum_part2)

	file:close()
else
	print("Couldnt open")
end
