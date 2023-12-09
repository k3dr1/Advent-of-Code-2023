local function is_zero(seq)
	local res = true
	for _,v in pairs(seq) do if (v ~= 0) then return false end end
	return res
end

local function forward_diff(seq)
	local t = {}
	for i = 1,#seq-1 do t[#t+1] = seq[i+1] - seq[i] end
	return t
end

local function calc_next(seq)
	local res = 0
	local stack = {seq}
	local nseq = {}
	while not (is_zero(nseq) or #nseq == 0) do
		nseq = forward_diff(nseq)
		stack[#stack+1] = nseq
	end
	for j = #stack,1,-1 do res = res + stack[j][#stack[j]] end
	return res
end

local file = io.open("input.txt", "r")
if file then
	local res, res2 = 0, 0
	for line in file:lines() do
		local sequence, r_sequence = {}, {}
		for num in line:gmatch("[^ ]+") do
			sequence[#sequence+1] = tonumber(num)
		end
		for i = 0,#sequence-1 do r_sequence[#sequence - i] = sequence[i] end
		local next_num, next_num2 = calc_next(sequence), calc_next(r_sequence)
		res, res2 = res + next_num, res2 + next_num2
	end
	print("Part 1 answer is "..res)
	print("Part 2 answer is "..res2)
else
	print("Couldnt open")
end
