--1024, 768
WINDOW_WIDTH = 1024
WINDOW_HEIGHT = 768
WINDOW_NAME = "LUA GAME CHANGE"
local a
function OnEngineLoad()
	a=CreateSprite("assets/test.png", 464, 464, 50, 100)
	x,y=GetPos(a)
	print(x.." - "..y)
end

function OnInput(key, state)
	if (key==1073741903) then
		SetPos(a, 0,0)
	end
end