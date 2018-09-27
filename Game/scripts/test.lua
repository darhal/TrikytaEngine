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
	local ba = physicalize(a, 1, 1, "Dynamic")
	setLinearVelocity(ba, 25, 10)
	
	local txt1 = createText("This is a simple output example", "Engine_Assets/fonts/DroidSans.ttf", 16, 500, math.random(50),  255,255,255);
	setLinearVelocity(physicalize(txt1, 1, 1, "Dynamic"), 20, 0)
	
	local txt2 = createText("THIS IS ANOTHER PHYSICLIZED TEXT", "Engine_Assets/fonts/DroidSans.ttf", 16, 410,  math.random(20),  255,255,255);
	setLinearVelocity(physicalize(txt2, 1, 1, "Dynamic"), -20, 0)
	
	local txt3 = createText("This is so funny bro", "Engine_Assets/fonts/DroidSans.ttf", 16, 300,  math.random(10),  255,255,255);
	setLinearVelocity(physicalize(txt3, 1, 1, "Dynamic"), 30, 0)
	--deleteSprite(a)
	--deleteSprite(a)
	--deleteSprite(a)
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
