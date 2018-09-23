--[[local b

function lol()
	print("lol called")
end

function OnEngineLoad()
	b=CreateSprite("assets/test.png", 464, 464, 400, 400)
	x,y=GetPos(b)
	print(x.." - "..y)
	AddEventHandler("OnKeyboardInput", lol)
end--]]


