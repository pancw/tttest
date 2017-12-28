do
	print("hello main lua.")
end

function Tick()
	print("Tick")	
	llua.send()
end

function ShutDown()
	print("Lua ShutDown")
end
