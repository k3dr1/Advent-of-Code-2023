local file = io.open("input.txt", "r")

if file then
	local res = 0
	for line in file:lines() do
		local val = 0
		local beforeBar = line:sub(line:find(":"), line:find("|"))
		local afterBar = line:sub(line:find("|"), line:find("\n"))

		for numToWin in beforeBar:gmatch("%d+") do
			for num in afterBar:gmatch("%d+") do
				if (numToWin == num) then
					if (val == 0) then
						val = 1
					else
						val = val * 2
					end
				end
			end
		end
		res = res + val
	end
	print("Part 1 answer is "..res)

	-- Part 2 
	file:seek("set")
	res = 0
	local copies = {}
	for i = 1, 219 do
		copies[i] = 0
	end

	local lineCount = 1
	for line in file:lines() do
		copies[lineCount] = copies[lineCount] + 1
		local beforeBar = line:sub(line:find(":"), line:find("|"))
		local afterBar = line:sub(line:find("|"), line:find("\n"))

		local val = 0

		for numToWin in beforeBar:gmatch("%d+") do
			for num in afterBar:gmatch("%d+") do
				if (numToWin == num) then
					val = val + 1
				end
			end
		end
		if (val > 0) then
			for i = 1, val do
				copies[lineCount + i] = copies[lineCount + i] + copies[lineCount]
			end
		end
		lineCount = lineCount + 1
	end
	for _, v in pairs(copies) do
		res = res + v
	end
	print("Part 2 answer is "..res)
	file:close()
else
	print("Couldnt open")
end
