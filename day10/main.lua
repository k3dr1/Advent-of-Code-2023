local T = {
	["|"] = {{0,1},{0,-1}},
	["-"] = {{1,0},{-1,0}},
	["L"] = {{0,-1},{1,0}},
	["J"] = {{0,-1},{-1,0}},
	["7"] = {{0,1},{-1,0}},
	["F"] = {{0,1},{1,0}},
	["."] = {{0,0},{0,0}},
}

local function set(x, c, grid)
	local line = grid[c[2]]
	line = line:sub(1,c[1]-1)..x..line:sub(c[1]+1)
	grid[c[2]] = line
end

-- c1 + c2
local function add(c1, c2)
	local c_out = {c1[1] + c2[1], c1[2] + c2[2]}
	return c_out
end

-- c1 - c2
local function subtract(c1,c2)
	local c_out = {c1[1] - c2[1], c1[2] - c2[2]}
	return c_out
end

-- c1 == c2
local function same(c1,c2)
	local res = true
	res = res and (c1[1] == c2[1])
	res = res and (c1[2] == c2[2])
	return res
end

-- returns char at that position in the grid
local function at(c,grid)
	--print("Called at with "..c[1]..","..c[2])
	if (grid[c[2]] == nil) then return nil end
	return (grid[c[2]]:sub(c[1],c[1]) or ".")
end

local function get_arr(direction)
	if (same(direction, {1,0})) then return "r" end
	if (same(direction,{-1,0})) then return "l" end
	if (same(direction, {0,1})) then return "d" end
	if (same(direction,{0,-1})) then return "u" end
end

local function fill(c, grid)
	local stack = {c}
	while (#stack ~= 0) do
		local cell = table.remove(stack)
		if (at(cell, grid) == ".") then set("I", cell, grid) end
		local top =   {cell[1], cell[2]-1}
		local bot =   {cell[1], cell[2]+1}
		local right=  {cell[1]+1, cell[2]}
		local left =  {cell[1]-1, cell[2]}
		if (at(top,grid) == ".") then table.insert(stack, top) end
		if (at(bot,grid) == ".") then table.insert(stack, bot) end
		if (at(right,grid) == ".") then table.insert(stack, right) end
		if (at(left,grid) == ".") then table.insert(stack, left) end
	end
end

local function left_of(direction)
	if (same(direction, {1,0})) then
		return {0,-1}
	elseif (same(direction, {0, -1})) then
		return {-1,0}
	elseif (same(direction, {-1, 0})) then
		return {0,1}
	elseif (same(direction, {0,1})) then
		return {1,0}
	end
end


local function right_of(direction)
	if (same(direction, {1,0})) then
		return {0,1}
	elseif (same(direction, {0, 1})) then
		return {-1,0}
	elseif (same(direction, {-1, 0})) then
		return {0,-1}
	elseif (same(direction, {0,-1})) then
		return {1,0}
	end
end

local function move(c,c_prev,grid, entry_first)
	-- Optional argument, used to choose which way to go in the loop
	entry_first = entry_first or true
	local tile = at(c,grid)
	local entry, exit = T[tile][1], T[tile][2]
	entry = add(c, entry)
	exit = add(c, exit)
	if entry_first then
		if (not same(c_prev, entry)) then
			return entry
		elseif (not same(c_prev, exit)) then
			return exit
		end
	else
		if (not same(c_prev, exit)) then
			return exit
		elseif (not same(c_prev, entry)) then
			return entry
		end
	end
end

local file = io.open("input.txt", "r")
if file then

	local grid,grid2 = {}, {}
	local found = false
	local start = {1,1}

	for line in file:lines() do
		grid[#grid+1] = line
		grid2[#grid2+1] = line

		if (line:find("S")) then
			start[1] = line:find("S")
			found = true
		end

		if not found then start[2] = start[2] + 1 end
	end

	print("S =",start[1], start[2])
	local start2 = {start[1], start[2]+1}
	set("#", start, grid2)
	set("#", start2, grid2)
	local prev = start
	local m = move(start2,prev,grid)
	local steps = 2
	local direction = {0,0}

	while not (at(m,grid) == "S") do
		local temp = m
		m = move(m, prev, grid)
		prev = temp

		direction = subtract(m, prev)
		local left_tile = add(m, left_of(direction))
		if (at(left_tile, grid2) ~= "#") then
			set("I", left_tile, grid2)
		end

		local right_tile = add(m, right_of(direction))
		if (at(right_tile, grid2) ~= "#" ) then
			if not (at(right_tile,grid2) == nil) then
				set("0", right_tile, grid2)
			end
		end

		set("#", m, grid2)
		steps = steps + 1
	end

	print("m is at "..m[1]..","..m[2].." "..at(m,grid))
	print("Part 1 answer is "..steps/2)

	for k,line in pairs(grid2) do
		line = line:gsub("[^0I#]", ".")
		grid2[k] = line
	end
	for k,line in pairs(grid2) do
		for i = 1, #line do
			if (at({i,k}, grid2) == "I") then
				fill({i,k}, grid2)
			end
		end
	end
	local count_inside_cells = 0
	for k,line in pairs(grid2) do
		for i = 1, #line do
			if (at({i,k}, grid2) == "I") then
				count_inside_cells = count_inside_cells + 1
			end
		end
	end

	print("Part 2 answer is "..count_inside_cells)


	local grid2_file = io.open("generated_grid.txt", "w")
	if grid2_file then
		for _,line in pairs(grid2) do
			grid2_file:write(line.."\n")
		end
	else
		print("Couldnt open bound file")
	end
else
	print("Couldnt open")
end
