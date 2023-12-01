-- Function declaration
local function getNum(s)
	print("Input is ", s)
	local val1 = 0

	local t = {
		["%d"] = -1,
		["one"]   = 1,
		["two"]   = 2,
		["three"] = 3,
		["four"]  = 4,
		["five"]  = 5,
		["six"]   = 6,
		["seven"] = 7,
		["eight"] = 8,
		["nine"]  = 9,
	}

	I = 10000
	for k, v in pairs(t) do
		local tmp, _ = string.find(s, k)
		if (tmp ~= nil) then
			if (tmp < I) then
				I = tmp
				if (k == "%d") then
					val1 = string.match(s, "%d")
				else
					val1 = v
				end
			end
		end
	end

	s = string.reverse(s)
	local val2 = 0

	local t_rev = {
		["%d"] = -1,
		["eno"]   = 1,
		["owt"]   = 2,
		["eerht"] = 3,
		["ruof"]  = 4,
		["evif"]  = 5,
		["xis"]   = 6,
		["neves"] = 7,
		["thgie"] = 8,
		["enin"]  = 9,
	}


	I = 10000
	for k, v in pairs(t_rev) do
		local tmp, _ = string.find(s, k)
		if (tmp ~= nil) then
			if (tmp < I) then
				I = tmp
				if (k == "%d") then
					val2 = string.match(s, "%d")
				else
					val2 = v
				end
			end
		end
	end

	print("First digit is ", val1, " and second is ", val2, " : ", val1..val2)
	return (val1..val2)
end


local file = io.open("input.txt", "r")

if file then

	local sum = 0
	for line in file:lines() do
		sum = sum + getNum(line)
	end

	print("The output is ")
	print(sum)

	file:close()
else
	print("Couldnt open")
end
