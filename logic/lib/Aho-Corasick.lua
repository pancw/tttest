function chsize(char)
	assert(char)
    if char > 240 then
        return 4
    elseif char > 225 then
        return 3
    elseif char > 192 then
        return 2
    else
        return 1
    end
end
------------------------------------------------utf8

local test = {
	"abcrr",
	"bc",
	"d",
	"你好吗",
	"好吗",
	"你好",
}
local input = "不好你好吗bcccbbabcrrrrbdbc"
------------------------------------------------test data

function createNode()
	return {
		_allChildrenTbl = {
			--[[
			[char] = node,
			--]]
		},
		_fail = nil; -- node
		_word = nil; -- "string"
	}
end

function getFailPointer(node)
	return node._fail
end

function setFailPointer(node, fail)
	node._fail = fail
end

function hasChild(node, c)
	return node._allChildrenTbl[c] and true	
end

function getChildByChar(node, c)
	return node._allChildrenTbl[c]
end

function addChild(node, c)
	node._allChildrenTbl[c] = createNode()
end

function setWord(node, s)
	node._word = s	
end

function getWord(node)
	return node._word
end

function getAllChildrenTbl(node)
	return node._allChildrenTbl
end
------------------------------------------------func

local root = createNode() -- global

function add(s)
	if (not s) or (string.len(s) == 0) then
		return
	end	

	local now = root
	local idx = 1
	while (idx <= #s) do
		local utfLen = chsize(string.byte(s, idx))
		local c = s:sub(idx, utfLen + idx - 1)
		idx = idx + utfLen

		if not hasChild(now, c) then
			addChild(now, c)	
		end	
		now = getChildByChar(now, c)
	end
	setWord(now, s)	
end

function build_tree()
	for _, s in pairs(test) do
		add(s)				
	end	
end

function build_fail_pointer()
	-- bfs
	local queue = {root}	
	while (#queue > 0) do
		local now = table.remove(queue, 1) -- pop
		local allChildrenTbl = getAllChildrenTbl(now)		

		-- 儿子c的失败指针=父亲的失败指针指向的节点的儿子c
		-- 找不到就指向root
		-- 第一层节点默认指向root
		for c, node in pairs(allChildrenTbl) do
			local fail = getFailPointer(now)
			while ( fail and (not hasChild(fail, c)) ) do
				fail = getFailPointer(fail)
			end
			if not fail then
				setFailPointer(node, root)
			else
				setFailPointer(node, getChildByChar(fail, c))
			end
			table.insert(queue, node)
		end
	end	
	setFailPointer(root, root)
end

function try_match(input)
	local now = root
	local idx = 1
	while (idx <= #input) do
		local utfLen = chsize(string.byte(input, idx))
		local c = input:sub(idx, utfLen + idx -1)
		idx = idx + utfLen

		while ((not hasChild(now, c)) and (now ~= root)) do
			now = getFailPointer(now)
		end

		if hasChild(now, c) then
			now = getChildByChar(now, c)	
		end	

		-- 如果只是把敏感变*，这个if可以开着，match的子串没必要再找出来了。
		--if getWord(now) then -- matched
		--	print("matched:", getWord(now))
		--else
			local t = now
			while( t ~= root ) do
				if getWord(t) then
					print("matched:", getWord(t))
				end
				t = getFailPointer(t) -- 往回找有可能匹配的子串
			end
		--end
	end
end

function main()
	build_tree()
	build_fail_pointer()
	try_match(input)
end

--main()
