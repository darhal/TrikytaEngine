local a

function OnEngineLoad()
	print("____________________________________________________________________")
	a = createSprite("assets/test.png", 464, 464, 0, 0)
	local b = createAnimation("assets/anim_pack.png", "assets/anim_pack.a", 200, 200, 100, 100, 0.01)
	--attachTo(b,a, 0.5,0.5)
	--setSpriteVisible(a, false)
	--print("Onload1")
end
AddEventHandler("OnEngineLoad", OnEngineLoad)


function OnRender(dt)
	--print("2 "..dt)
end
AddEventHandler("OnEngineRender", OnRender)

function OnRenderCrash(dt)
	--print("1 "..dt)
end
AddEventHandler("OnEngineRender", OnRenderCrash)

function OnKeyboardInput()
	--setSpriteVisible(a, true)
end
--AddEventHandler("OnKeyboardInput", OnKeyboardInput)

