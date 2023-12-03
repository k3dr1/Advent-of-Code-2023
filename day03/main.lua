local file = io.open("input.txt", "r")

local function isAdjacent(prevLine, curLine, nextLine, i, j)
	if (i == nil or j == nil) then return false end
	local charSet = "[^\n%d%.]"
	local result = false
	result = result or string.find(string.sub(prevLine, math.max(1, i - 1), math.min(string.len(prevLine), j + 1)), charSet)
	result = result or string.find(string.sub(curLine, math.max(1, i - 1), math.min(string.len(curLine), j + 1)), charSet)
	result = result or string.find(string.sub(nextLine, math.max(1, i - 1), math.min(string.len(nextLine), j + 1)), charSet)
	return result
end

local function isDigit(s, idx)
	return string.find(s:sub(idx,idx), "%d") ~= nil
end

local  function extractNum(s, idx)
	--print("extractNum starting at "..s:sub(idx,idx))
	--if (~isDigit(s, idx)) then return nil end
	local leftIdx = idx
	local rightIdx = idx

	while (leftIdx > 1 and isDigit(s, leftIdx - 1)) do
		leftIdx = leftIdx - 1
	end

	while (rightIdx < #s and isDigit(s, rightIdx + 1)) do
		rightIdx = rightIdx + 1
	end

	local extracted = tonumber(s:sub(leftIdx, rightIdx))

	--print("extracted num is "..extracted)

	return (extracted)
end

local function getNumProd(prevLine, curLine, nextLine, i)

	if (i == nil) then return 0 end

	local numFound = 0
	local numProd = 1

	-- Checking the top line
	if (isDigit(prevLine, i)) then
		-- The top middle position contains a digit
		--print("digit "..prevLine:sub(i,i).." on top at "..i)
		local num = extractNum(prevLine, i)
		numFound = numFound + 1
		numProd = numProd * num
	else
		-- Top middle doesn't contain a digit, checking left and right
		if (1 < i and isDigit(prevLine, i - 1)) then
			--print("digit "..prevLine:sub(i-1,i-1).." on top at "..i-1)
			local num = extractNum(prevLine, i - 1)
			numFound = numFound + 1
			numProd = numProd * num
		end

		if (i < #prevLine and isDigit(prevLine, i + 1)) then
			--print("digit "..prevLine:sub(i+1,i+1).." on top at "..i+1)
			local num = extractNum(prevLine, i + 1)
			numFound = numFound + 1
			numProd = numProd * num
		end
	end

	-- Checking the middle line
	if (1 < i and isDigit(curLine, i - 1)) then
		--print("digit "..curLine:sub(i-1,i-1).." in middle at "..i-1)
		local num = extractNum(curLine, i - 1)
		numFound = numFound + 1
		numProd = numProd * num
	end

	if (i < #curLine and isDigit(curLine, i + 1)) then
		--print("digit "..curLine:sub(i+1,i+1).." in middle at "..i+1)
		local num = extractNum(curLine, i + 1)
		numFound = numFound + 1
		numProd = numProd * num
	end

	-- Checking the bottom line
	if (isDigit(nextLine, i)) then
		-- The bottom middle position contains a digit
		--print("digit "..nextLine:sub(i,i).." on bottom at "..i)
		local num = extractNum(nextLine, i)
		numFound = numFound + 1
		numProd = numProd * num
	else
		-- bottom middle doesn't contain a digit, checking left and right
		if (1 < i and isDigit(nextLine, i - 1)) then
			--print("digit "..nextLine:sub(i-1,i-1).." on bottom at "..i-1)
			local num = extractNum(nextLine, i - 1)
			numFound = numFound + 1
			numProd = numProd * num
		end

		if (i < #nextLine and isDigit(nextLine, i + 1)) then
			--print("digit "..nextLine:sub(i+1,i+1).." on bottom at "..i+1)
			local num = extractNum(nextLine, i + 1)
			numFound = numFound + 1
			numProd = numProd * num
		end
	end

	--print("For the * at idx="..i.." numFound="..numFound.." numProd="..numProd)
	if (numFound == 2) then
		return numProd
	else
		return 0
	end
end

if file then

	local res = 0
	local file_contents = file:read("*a")
	file_contents = "............................................................................................................................................\n"..file_contents.."............................................................................................................................................"

	local lineIterator = file_contents:gmatch("[^\n]*\n")
	local prevLine = lineIterator()
	local curLine = lineIterator()
	local nextLine

	-- Part 1
	for line in lineIterator do
		nextLine = line
		local lCharPointer = 1
		local rCharPointer = 1
		while ((lCharPointer ~= nil) and (lCharPointer < string.len(curLine))) do
			lCharPointer, rCharPointer = string.find(curLine, "%d+", lCharPointer)
			if (isAdjacent(prevLine, curLine, nextLine, lCharPointer, rCharPointer)) then
				local num = tonumber(string.sub(curLine, lCharPointer, rCharPointer))
				res = res + num
				--print(num.." is added to the sum")
			end

			if (lCharPointer == nil) then
				--print("lCharPointer is nil")
			else
				lCharPointer = rCharPointer + 1
				--print("lcharpointer="..lCharPointer)
			end
		end
		prevLine = curLine
		curLine = nextLine
	end

	print("Part 1 answer is "..res)

	-- Part 2
	lineIterator = file_contents:gmatch("[^\n]*\n")
	prevLine = lineIterator()
	curLine = lineIterator()

	res = 0

	for line in lineIterator do
		nextLine = line
		local starIdx = 1
		--print("Now curline is "..curLine)
		while ((starIdx ~= nil) and (starIdx < string.len(curLine))) do
			starIdx = string.find(curLine, "%*", starIdx)

			res = res + getNumProd(prevLine, curLine, nextLine, starIdx)

			if (starIdx == nil) then
				--print("starIdx is nil")
			else
				starIdx = starIdx + 1
				--print("starIdx="..starIdx)
			end
		end
		prevLine = curLine
		curLine = nextLine
	end

	print("Part 2 answer is "..res)

	file:close()
else
	print("Couldnt open")
end
