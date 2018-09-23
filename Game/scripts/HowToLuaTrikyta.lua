--[[
-- Where to Start:
	-To Specify what scripts gonna load modify the meta.txt seprates your script names using at the end';'
	-You can Modify the config.lua

--TRIKYTA ENGINE AVAIALBLE FUNCTIONS:
	*) CreateSprite(String path, int w, int h, int x, int y) return sprite
	*) GetPos(Sprite a) return position of sprite
	*) SetPos(a, x, y) return set pos of sprite to x, y
	*) Sleep(int t) pause program for t milisecond
	*) DelSprite(Sprite a) delete a sprite (DONT DELETE TWICE IT GONNA CRASH TRUST ME)

--EVENTS:
	AddEventHandler("OnKeyboardInput", OnKeyboardInput)
	AddEventHandler("OnMouseClick", OnMouseClick)
	AddEventHandler("OnMouseMove", OnMouseMove)
	AddEventHandler("OnEngineRender", OnRender)
	AddEventHandler("OnEngineLoad", OnEngineLoad)
	AddEventHandler("OnEnginePreInit", OnPreInit) --BROKEN AT THE MOMENT
	
--Function syntax for AddEventHandler:
	AddEventHandler(String event, Function ToBeCalledOnEvent)
	
--Examples:
	Look At test.lua and lol.lua

--Special thanks to:
	-Thomas Decroix (Tricky Patrick aka.ThoThoBoss)
--]]
