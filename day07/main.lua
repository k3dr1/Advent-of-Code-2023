local function string_get_type(hand)
	local freq = {}
	for k,_ in pairs(T) do
		freq[#freq+1] = {card = k, n = 0}
	end
	for char in hand:gmatch(".") do
		for k,v in pairs(freq) do
			if (v.card == char) then
				v.n = v.n + 1
			end
		end
	end
	table.sort(freq, function(a,b) return a.n>b.n end)
	-- 1 = Highcard 2 = Onepair 3 = Twopair 4 = Threeofkind 5 = Fullhouse 6 = Fourofkind 7 = Fiveofkind
	if (freq[1].n == 5) then
		return 7
	else
		if (freq[1].n == 4) then
			return 6
		else
			if (freq[1].n == 3) then
				if (freq[2].n == 2) then return 5 end
				if (freq[2].n == 1) then return 4 end
			else
				if (freq[1].n == 2) then
					if (freq[2].n == 2) then return 3 end
					if (freq[2].n == 1) then return 2 end
				else
					if (freq[1].n == 1) then return 1 end
				end
			end
		end
	end
end


T = { ["A"] = 14, ["K"] = 13, ["Q"] = 12, ["J"] = 11, ["T"] = 10, ["9"] = 9, ["8"] = 8, ["7"] = 7, ["6"] = 6, ["5"] = 5, ["4"] = 4, ["3"] = 3, ["2"] = 2, }
T2 = { ["A"] = 14, ["K"] = 13, ["Q"] = 12, ["J"] = 1, ["T"] = 10, ["9"] = 9, ["8"] = 8, ["7"] = 7, ["6"] = 6, ["5"] = 5, ["4"] = 4, ["3"] = 3, ["2"] = 2, }


-- True if hand1 < hand2
local function compare_1(play1, play2)
	local hand1, hand2 = play1.hand, play2.hand

	local type1 = string_get_type(hand1)
	local type2 = string_get_type(hand2)

	if (type1 ~= type2) then
		return type1 < type2
	else
		for i = 1,5 do
			local c1, c2 = T[hand1:sub(i,i)], T[hand2:sub(i,i)]
			if (c1 ~= c2) then
				return c1 < c2
			end
		end
	end
	return nil
end


-- True if hand1 < hand2
local function compare_2(play1, play2)
	local hand1, hand2 = play1.hand, play2.hand
	local card_variants = "AKQJT98765432"

	local type1 = string_get_type(hand1)
	if (hand1:find("J")) then
		for jsub in card_variants:gmatch(".") do
			type1 = math.max(type1, string_get_type(hand1:gsub("J", jsub)))
		end
	end
	local type2 = string_get_type(hand2)
	if (hand2:find("J")) then
		for jsub in card_variants:gmatch(".") do
			type2 = math.max(type2, string_get_type(hand2:gsub("J", jsub)))
		end
	end

	if (type1 ~= type2) then
		return type1 < type2
	else
		for i = 1,5 do
			local c1, c2 = T2[hand1:sub(i,i)], T2[hand2:sub(i,i)]
			if (c1 ~= c2) then
				return c1 < c2
			end
		end
	end
end


local file = io.open("input.txt", "r")

if file then
	local part1 = {}
	local part2 = {}
	for line in file:lines() do
		part1[#part1+1] = {hand = line:match("[AKQJT98765432]+%s"):match("%S+"), bid = line:match("%s%d+")}
		part2[#part2+1] = {hand = line:match("[AKQJT98765432]+%s"):match("%S+"), bid = line:match("%s%d+")}
	end
	table.sort(part1, compare_1)
	table.sort(part2, compare_2)

	local res1 = 0
	local res2 = 0

	for k,v in pairs(part1) do
		res1 = res1 + k*v.bid
	end
	for k,v in pairs(part2) do
		res2 = res2 + k*v.bid
	end

	print("The answer for part 1 is "..res1)
	print("The answer for part 2 is "..res2)
else
	print("Couldnt open")
end
