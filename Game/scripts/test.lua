
local a

function lol()
	print("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa")
end

function lol2(x, y)
	print("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxx")
end

function OnEngineLoad()
	a=CreateSprite("assets/test.png", 464, 464, 50, 100)
	x,y=GetPos(a)
	print("__________________")
	AddEventHandler("OnKeyboardInput", lol)
	AddEventHandler("OnKeyboardInput", lol2)
end


function OnInput(key, state)
	if (key==1073741903) then
	end
end


function OnRender(dt)
	--print(dt) -- print delta time each frame
end