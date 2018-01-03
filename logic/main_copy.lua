
function Tick()
	--print("Tick")	
	--llua.send()
end

function ShutDown()
	print("Lua ShutDown")
end

print("hello main lua.")

require"lib.Aho-Corasick"
local S1 = require"1"
local S2 = require"2"

local input = "aaaaaxczfqwqwdassad叫dwqdqdwqd\
    cokkkkkkkkkkkkaskdjlaskldjaslkkkkkkkkkkkkkkkcok\
    kkkirreiooiretuirtperiopterperiutkkkkkkkkkkk乱世kkkkck\
    kkkkaasdadsada哈萨克大家了打了kkkkck\
    何进kkkkaasdadsadakkkckmmllmlml暴击阿拉山口大家了撒七雄mlmllkljlookjllkjljkjl\
    qwd"

-- c part
local cBegin = llua.getUTime()
lac.createRoot()

for k,v in pairs(S1) do
	lac.addNode(v.SensitiveWords)
end 

for k,v in pairs(S2) do
	lac.addNode(v.SensitiveWords)
end 

lac.buildFailPointer()
local cEnd = llua.getUTime()

local t1 = llua.getUTime()
lac.tryMatch(input)
local t2 = llua.getUTime()

print("c part use time:", cEnd-cBegin, t2-t1)

-- lua part

local luaBegin = llua.getUTime()
for k,v in pairs(S1) do
	add(v.SensitiveWords)
end 
for k,v in pairs(S2) do
	add(v.SensitiveWords)
end 
build_fail_pointer()
local luaEnd = llua.getUTime()

local t3 = llua.getUTime()
try_match(input)
local t4 = llua.getUTime()

print("lua part use time:", luaEnd-luaBegin, t4-t3)

