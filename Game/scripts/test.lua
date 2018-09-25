--[[
Called On Keyboard Input
(int key, int state)
*) For key codes: https://wiki.libsdl.org/SDL_Keycode
*) State: 
	- is equal to 768 On Button Pressed
	- is equal to 769 On Button Release
--]]
function OnKeyboardInput(key, state)
	print("KEY BOARD INPUT DETECTED!!!"..key)
end

--[[
Called on mouse click:
(int EventType, int MouseButton, int PosX, int PosY)
*)EventType:
	- 1025 if button is down
	- 1025 if button is up
*)MouseButton:
	- 3 RMB
	- 1 LMB
	- 2 Mouse wheel
*)PoX and PosY coordinates of the click
--]]
function OnMouseClick(EventType, MouseButton, PosX, PosY)
	print("MOUSE INPUT DETECTED"..PosX)
end

-- Called on mouse move arguments: ([int] x, [int] y) {POSITION OF THE MOUSE MOVE}
function OnMouseMove(x, y) 
	--print("MOUSE MOVE DETECTED")
end

-- Called every frame arguments: ([double] dt)
function OnRender1(dt) 
	--print("    "..dt)
end
AddEventHandler("OnEngineRender", OnRender1)

function Hello()
	print("HELLO FROM LUA EVERY 3k MS")
end

--Called when engine is already loaded!
function OnEngineLoad()
	print("____________________________________________________________________")
	local a = createSprite("assets/test.png", 464, 464, 50, 100)
	deleteSprite(a)
	deleteSprite(a)
	deleteSprite(a)
	--setTimer(Hello, 3000, 5)
	--AddEventHandler("OnKeyboardInput", OnKeyboardInput)
	--AddEventHandler("OnMouseClick", OnMouseClick)
	--AddEventHandler("OnMouseMove", OnMouseMove)
	--AddEventHandler("OnEngineRender", OnRender1)
end
AddEventHandler("OnEngineLoad", OnEngineLoad)

--Called On Engine Pre Init (YOU CANT USE TRIKYTA FUNCTIONS HERE!!)
function OnPreInit()
	--DO SMTHG
end
--AddEventHandler("OnEnginePreInit", OnPreInit) -- NOT WORKING FOR THIS VERSION!
