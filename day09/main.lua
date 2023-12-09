local function is_zero_sequence(seq)
	local res = true
	for k,v in pairs(seq) do
		if (v ~= 0) then
			return false
		end
	end
	return res
end

local function gen_forward_diff(seq)
	local t = {}
	for i = 1,#seq-1 do
		t[#t+1] = seq[i+1] - seq[i]
	end
	return t
end

local function calc_next(seq)
	local stack = {seq}
	local n_seq = gen_forward_diff(seq)
	stack[#stack+1] = n_seq
	while not (is_zero_sequence(n_seq)) do
		n_seq = gen_forward_diff(n_seq)
		stack[#stack+1] = n_seq
	end
	local res = 0
	for j = #stack,1,-1 do
		local x = stack[j][#stack[j]]
		res = res + x
	end
	return res
end


local file = io.open("input.txt", "r")

if file then
	local res = 0
	local res2 = 0
	for line in file:lines() do
		local sequence = {}
		local r_sequence = {}
		for num in line:gmatch("[^ ]+") do
			num = tonumber(num)
			sequence[#sequence+1] = num
		end
		for i = 0,#sequence-1 do r_sequence[#sequence - i] = sequence[i] end
		local next_num = calc_next(sequence)
		local next_num2 = calc_next(r_sequence)
		res = res + next_num
		res2 = res2 + next_num2

	end
	print("Part 1 answer is "..res)
	print("Part 2 answer is "..res2)
else
	print("Couldnt open")
end
