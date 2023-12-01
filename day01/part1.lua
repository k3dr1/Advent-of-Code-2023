-- Function declaration
local function getNum(s)
	print("Input is ", s)
	local d1 = string.match(s, "%d")
	local d2 = string.match(string.match(s, "%d%D*$"), "%d")
	print("First digit is ", d1, " and second is ", d2, " : ", d1..d2)
	return tonumber(d1..d2)
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
