local test = {
	"abcrr",
	"bc",
	"d",
}
local input = "abcd"

function createNode()
	local node = {
		_allChildrenTbl = {
			--[[
			[char] = node,
			--]]
		},
		_fail = nil; -- node
		_word = nil;	-- "string"
	}
	return node
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

-- work
local root = createNode()

function add(s)
	if (not s) or (#s == 0) then
		return
	end	

	local now = root;
	for i=1, #s do
		-- TODO utf8
		local c = s:sub(i,i)
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
	local queue = {root}	
	while (#queue > 0) do
		local now = table.remove(queue, 1) -- pop
		local allChildrenTbl = getAllChildrenTbl(now)		

		-- c儿子的失败指针=父亲的失败指针指向的节点的儿子c
		-- 找不到就指向root
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
	for i=1, #input do
		-- TODO utf8
		local c = input:sub(i,i)
		while ((not hasChild(now, c)) and (now ~= root)) do
			now = getFailPointer(now)
		end

		if hasChild(now, c) then
			now = getChildByChar(now, c)	
		end	

		if getWord(now) then -- matched
			print("matched a word:", getWord(now))
		else
			local t = now
			while( t ~= root ) do
				if getWord(t) then
					print("matched a sub word:", getWord(t))
				end
				t = getFailPointer(t)
			end
		end
	end
end

build_tree()
build_fail_pointer()

try_match(input)
