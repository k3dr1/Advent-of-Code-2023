local function shift(num, shift_array)
	num = tonumber(num)
	for _, begendshift in pairs(shift_array) do
		if (tonumber(begendshift["begin"]) <= num and tonumber(begendshift["end"]) >= num) then
			return num + begendshift["shift"]
		end
	end
	return num
end

local function shiftMap(input_array, shift_array)
	local t = {}
	for _, v in pairs(input_array) do
		t[#t+1] = shift(v, shift_array)
	end
	return t
end

local function tblPrint(t)
	for k, v in pairs(t) do
		print("    "..k.." : "..v)
	end
end

local function cndPrint(t)
	local out = ""
	for k,v in pairs(t) do
		out = out..v.." "
	end
	print(out)
end

local file = io.open("input.txt", "r")

if file then

	local parser_state = 0
	local seeds = {}
	-- Each of f_table[i] where 1<=i<=7 is an array of pairs 
	-- denoting the beginning of the range and the shift
	local f_table = {
		[1] = {
			[-math.huge] = 0},--sts
		[2] = {
			[-math.huge] = 0},--stf
		[3] = {
			[-math.huge] = 0},--ftw
		[4] = {
			[-math.huge] = 0},--wtl
		[5] = {
			[-math.huge] = 0},--ltt
		[6] = {
			[-math.huge] = 0},--tth
		[7] = {
			[-math.huge] = 0},--htl
	}

	local file_contents file:read("*a")
--local file_contents = [[seeds: 79 14 55 13
--
--seed-to-soil map:
--50 98 2
--52 50 48
--
--soil-to-fertilizer map:
--0 15 37
--37 52 2
--39 0 15
--
--fertilizer-to-water map:
--49 53 8
--0 11 42
--42 0 7
--57 7 4
--
--water-to-light map:
--88 18 7
--18 25 70
--
--light-to-temperature map:
--45 77 23
--81 45 19
--68 64 13
--
--temperature-to-humidity map:
--0 69 1
--1 0 69
--
--humidity-to-location map:
--60 56 37
--56 93 4
--]]

	--local lineIterator = file_contents:gmatch("[^\n]*\n")

	for line in file:lines() do
	--for line in lineIterator do
		if (line:find("seeds:")) then
			-- Part 1 part
			--for seed in line:gmatch("%d+") do
			--	seeds[#seeds+1] = seed
			--end

			-- Part 2 part
			for seed, length in line:gmatch("(%d+) (%d+)") do
				print(seed.." - "..length)

			end
		end

		if (line:find("seed%-to%-soil map:") ~= nil) then
			parser_state = 1
		end
		if (line:find("soil%-to%-fertilizer map:")) then
			parser_state = 2
		end
		if (line:find("fertilizer%-to%-water map:")) then
			parser_state = 3
		end
		if (line:find("water%-to%-light map:")) then
			parser_state = 4
		end
		if (line:find("light%-to%-temperature map:")) then
			parser_state = 5
		end
		if (line:find("temperature%-to%-humidity map:")) then
			parser_state = 6
		end
		if (line:find("humidity%-to%-location map:")) then
			parser_state = 7
		end

		if (line:find("%d+") ~= nil) then

			if (1 <= parser_state and parser_state <= 7) then
				local dst, src, rng = line:match("(%d+) (%d+) (%d+)")
				-- Adding the begin-shift mapping to the transform
				f_table[parser_state][src] = {src-dst}
				if (f_table[parser_state][src+rng] == nil) then
					f_table[parser_state][src+rng] = 0
				end
			end

		if (parser_state == 1) then
				local kb, vb, r = line:match("(%d+) (%d+) (%d+)")
				kb = tonumber(kb)
				vb = tonumber(vb)
				r = tonumber(r)
				sts[#sts+1] = {
					["begin"] = vb,
					["end"] = vb + r - 1,
					["shift"] = kb-vb}

			elseif (parser_state == 2) then
				local kb, vb, r = line:match("(%d+) (%d+) (%d+)")
				kb = tonumber(kb)
				vb = tonumber(vb)
				r = tonumber(r)
				stf[#stf+1] = {
					["begin"] = vb,
					["end"] = vb + r - 1,
					["shift"] = kb-vb}
			elseif (parser_state == 3) then
				local kb, vb, r = line:match("(%d+) (%d+) (%d+)")
				kb = tonumber(kb)
				vb = tonumber(vb)
				r = tonumber(r)
				ftw[#ftw+1] = {
					["begin"] = vb,
					["end"] = vb + r - 1,
					["shift"] = kb-vb}
			elseif (parser_state == 4) then
				local kb, vb, r = line:match("(%d+) (%d+) (%d+)")
				kb = tonumber(kb)
				vb = tonumber(vb)
				r = tonumber(r)
				wtl[#wtl+1] = {
					["begin"] = vb,
					["end"] = vb + r - 1,
					["shift"] = kb-vb}
			elseif (parser_state == 5) then
				local kb, vb, r = line:match("(%d+) (%d+) (%d+)")
				kb = tonumber(kb)
				vb = tonumber(vb)
				r = tonumber(r)
				ltt[#ltt+1] = {
					["begin"] = vb,
					["end"] = vb + r - 1,
					["shift"] = kb-vb}
			elseif (parser_state == 6) then
				local kb, vb, r = line:match("(%d+) (%d+) (%d+)")
				kb = tonumber(kb)
				vb = tonumber(vb)
				r = tonumber(r)
				tth[#tth+1] = {
					["begin"] = vb,
					["end"] = vb + r - 1,
					["shift"] = kb-vb}
			elseif (parser_state == 7) then
				local kb, vb, r = line:match("(%d+) (%d+) (%d+)")
				kb = tonumber(kb)
				vb = tonumber(vb)
				r = tonumber(r)
				htl[#htl+1] = {
					["begin"] = vb,
					["end"] = vb + r - 1,
					["shift"] = kb-vb}
			end
		end
	end

	--local res = math.huge
	--local res_tbl = seeds

	--res_tbl = shiftMap(res_tbl, sts)
	--res_tbl = shiftMap(res_tbl, stf)
	--res_tbl = shiftMap(res_tbl, ftw)
	--res_tbl = shiftMap(res_tbl, wtl)
	--res_tbl = shiftMap(res_tbl, ltt)
	--res_tbl = shiftMap(res_tbl, tth)
	--res_tbl = shiftMap(res_tbl, htl)

	--for _, v in pairs(res_tbl) do
	--	res = math.min(res, v)
	--end

	file:seek("set")

	local line = file:read("*l")

	print("Just cheking")
	print(line)

	local res3 = math.huge

	if (line:find("seeds:")) then
		-- Part 2 part
		for seed, length in line:gmatch("(%d+) (%d+)") do
			print("Doing seed="..seed.." with l="..length)
			for i = 1, length - 1, 100 do
				local res3_tbl = {seed + i}
				res3_tbl = shiftMap(res3_tbl, sts)
				res3_tbl = shiftMap(res3_tbl, stf)
				res3_tbl = shiftMap(res3_tbl, ftw)
				res3_tbl = shiftMap(res3_tbl, wtl)
				res3_tbl = shiftMap(res3_tbl, ltt)
				res3_tbl = shiftMap(res3_tbl, tth)
				res3_tbl = shiftMap(res3_tbl, htl)

				res3 = math.min(res3, res3_tbl[1])
				if (i % 1000000 == 0) then
					print("i = "..i.." res3 = "..res3)
				end
			end
		end
	end


	local res2 = math.huge
	local res2_tbl = seeds

	res2_tbl = shiftMap(res2_tbl, sts)
	res2_tbl = shiftMap(res2_tbl, stf)
	res2_tbl = shiftMap(res2_tbl, ftw)
	res2_tbl = shiftMap(res2_tbl, wtl)
	res2_tbl = shiftMap(res2_tbl, ltt)
	res2_tbl = shiftMap(res2_tbl, tth)
	res2_tbl = shiftMap(res2_tbl, htl)

	for _, v in pairs(res2_tbl) do
		res2 = math.min(res2, v)
	end

	--print("Part 1 answer is "..res)
	print("Part 2 answer is "..res3)

	file:close()
else
	print("Couldnt open")
end
