local function kvPrint(t)
	for k, v in pairs(t) do
		print("    "..k.." : "..v["begin"].."-"..v["end"])
	end
end

local function transformSort(t)
	return table.sort(t, function(a,b) return a["b"] < b["b"] end)
end

-- t must be a sorted table
local function probe(num, t)
	local prev = 0
	for k, v in pairs(t) do
		--print("Comparing vb"..v["b"].." and "..num)
		if (v["b"] > num) then return prev
		else
			prev = v["b"]
		end
	end
	return nil
end

local function removeDuplicates(t)
	local hash = {}
	local res = {}
	for _,v in ipairs(t) do
		if (not hash[v]) then
			res[#res+1] = v
			hash[v] = true
		end
	end
	return res
end

local function transformMerge(t1, t2)
	local t_out = {}
	local i, j = 1, 1
	for k, v in pairs(t1) do
		t_out[k] = {["b"] = v["b"], ["s"] = 0}
	end
	for k,v in pairs(t2) do
		t_out[#t_out+1] = {["b"] = v["b"], ["s"] = 0}
	end

	transformSort(t_out)

	local cur = 0
	local idx = 2
	for k, v in pairs(t_out) do
		if (t1[idx] ~= nil and v["b"] == t1[idx]["b"]) then
			cur = t1[idx]["s"]
			idx = idx + 1
		end
		v["s"] = v["s"] + cur
	end

	cur = 0
	idx = 2
	for k, v in pairs(t_out) do
		if (t2[idx] ~= nil and v["b"] == t2[idx]["b"]) then
			cur = t2[idx]["s"]
			idx = idx + 1
		end
		v["s"] = v["s"] + cur
	end
	t_out = removeDuplicates(t_out)

	-- Removing duplicates
	return t_out
end

local function intersection(a_begin, a_end, b_begin, b_end)
	local o_begin = math.max(a_begin, b_begin)
	local o_end = math.min(a_end, b_end)
	if (o_begin <= o_end)  then
		return {["begin"] = o_begin, ["end"] = o_end}
	else return nil end
end

local file = io.open("input.txt", "r")

if file then
	local parser_state = 0
	local seeds = {}
	-- Each of f_table[i] where 1<=i<=7 is an array of pairs 
	-- denoting the beginning of the range and the shift
	local f_table = {
		[1] = {},--sts
		[2] = {},--stf
		[3] = {},--ftw
		[4] = {},--wtl
		[5] = {},--ltt
		[6] = {},--tth
		[7] = {},--htl
	}

	--local file_contents = file:read("*a")
local file_contents = [[seeds: 79 14 55 13

seed-to-soil map:
50 98 2
52 50 48

soil-to-fertilizer map:
0 15 37
37 52 2
39 0 15

fertilizer-to-water map:
49 53 8
0 11 42
42 0 7
57 7 4

water-to-light map:
88 18 7
18 25 70

light-to-temperature map:
45 77 23
81 45 19
68 64 13

temperature-to-humidity map:
0 69 1
1 0 69

humidity-to-location map:
60 56 37
56 93 4
]]

	local lineIterator = file_contents:gmatch("[^\n]*\n")

	for line in file:lines() do
	--for line in lineIterator do
		if (line:find("seeds:")) then
			for seed, length in line:gmatch("(%d+) (%d+)") do
				local s, l = tonumber(seed), tonumber(length)
				-- Inclusive/exclusive
				seeds[#seeds+1] = {["begin"] = s, ["end"] = s+l-1}
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
				dst, src, rng = tonumber(dst), tonumber(src), tonumber(rng)
				if parser_state == 2 then
					print(dst,src,rng)
				end
				f_table[parser_state][#f_table[parser_state]+1] = {["dst"] = dst, ["src"] = src, ["rng"] = rng}
			end
		end
	end

	local function applyF(t, f)
		local m_intervals = {}
		for _,seed in pairs(t) do
			local intersections = {}
			for _,interval in pairs(f) do
				--print("    Seed: "..seed["begin"].."-"..seed["end"])
				--print("    Interval: "..interval["src"].."-"..interval["src"]+interval["rng"]-1)
				local intlet = intersection(seed.begin, seed["end"], interval["src"], interval["src"] + interval["rng"] - 1)
				if intlet ~= nil then
					local delta = interval.dst - interval.src
					intlet_shifted = {begin = intlet["begin"] + delta, ["end"] = intlet["end"] + delta}
					intersections[#intersections+1] = intlet
					m_intervals[#m_intervals+1] = intlet_shifted
					--print("intlet "..intlet_shifted.begin.."-"..intlet_shifted["end"])
				else
					--print("no intersection")
				end
			end

			local g = f
			g[#g+1] = {src=-math.huge, rng=0}
			g[#g+1] = {src=math.huge, rng=0}
			table.sort(g, function(a,b) return a.src<b.src end)

			local prev_island = {}
			for k,v in pairs(g) do
				--print("eater "..v["src"].."-"..(v["src"]+v["rng"]-1))
				if (k > 1) then
					--print("checking for intr from "..(prev_island["src"] + prev_island["rng"] - 1).."-"..v["src"])
					local island = intersection(prev_island["src"] + prev_island["rng"] - 1 + 1,
						v["src"] - 1, seed["begin"], seed["end"])
					table.insert(m_intervals, island)
				end
				prev_island = v
			end
		end
		return m_intervals
	end

	local t = {}
	t = applyF(seeds, f_table[1])
	kvPrint(t)
	t = applyF(t, f_table[2])
	--kvPrint(t)
	t = applyF(t, f_table[3])
	--kvPrint(t)
	t = applyF(t, f_table[4])
	--kvPrint(t)
	t = applyF(t, f_table[5])
	--kvPrint(t)
	t = applyF(t, f_table[6])
	--kvPrint(t)
	t = applyF(t, f_table[7])
	print("---------------------")
	--kvPrint(t)

	local res = math.huge

	for k, v in pairs(t) do
		res = math.min(res, v.begin)
	end

	print(res)

	file:close()
else
	print("Couldnt open")
end
