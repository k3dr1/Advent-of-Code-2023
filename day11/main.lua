local function at(c,grid)
	--print("Called at with "..c[1]..","..c[2])
	if (grid[c[2]] == nil) then return nil end
	return (grid[c[2]]:sub(c[1],c[1]) or ".")
end
-- Returns the taxicab distance between two points, for each crossing of an expanded row/col adds "expansion" amount to the distance
local function distance(c1, c2, rows, cols, expansion)
	local row_intercept, col_intercept = 0, 0
	local res = math.abs(c2[1] - c1[1]) + math.abs(c2[2] - c1[2])
	for k,v in pairs(cols) do
		if (c1[1] < v and v < c2[1]) or (c2[1] < v and v < c1[1]) then
			col_intercept = col_intercept + 1
		end
	end
	for k,v in pairs(rows) do
		if (c1[2] < v and v < c2[2]) or (c2[2] < v and v < c1[2]) then
			row_intercept = row_intercept + 1
		end
	end
	res = res + (row_intercept + col_intercept)*(expansion-1)
	return res
end

local file = io.open("input.txt", "r")
if file then

	local res1, res2 = 0, 0

	local grid, galaxies = {}, {}
	local expanded_rows = {}
	local expanded_cols = {}

	for line in file:lines() do
		grid[#grid+1] = line
	end

	for k,line in pairs(grid) do
		for char= 1,#line do
			if (line:sub(char,char) == "#") then
				print("Galaxy at "..k..","..char)
				galaxies[#galaxies+1] = {char,k}
			end
		end
	end

	-- Going row-wise
	for y = 1, #grid do
		local line = grid[y]
		if (line:find("#") == nil) then
			expanded_rows[#expanded_rows+1] = y
		end
	end
	-- Going col-wise
	for x = 1, #(grid[1]) do
		local column = ""
		for y = 1, #grid do
			local char = at({x,y}, grid)
			column = column..char
		end
		if (column:find("#") == nil) then
			expanded_cols[#expanded_cols+1] = x
		end
	end

	local pair_count = 0
	for k,gal1 in pairs(galaxies) do
		for l = k+1,#galaxies do
			local gal2 = galaxies[l]
			pair_count = pair_count + 1
			res1 = res1 + distance(gal1, gal2, expanded_rows, expanded_cols, 2)
			res2 = res2 + distance(gal1, gal2, expanded_rows, expanded_cols, 1000000)
		end
	end

	print("The answer for part 1 is "..res1)
	print("The answer for part 2 is "..res2)
else
	print("Couldnt open the file")
end
