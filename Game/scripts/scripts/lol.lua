local a

function OnEngineLoad()
	print("____________________________________________________________________")
	a = createSprite("assets/test.png", 464, 464, 0, 0)
	local b = createAnimation("assets/anim_pack.png", "assets/anim_pack.a", 200, 200, 100, 100, 0.01)
	attachTo(b,a, 0.5,0.5)
end
AddEventHandler("OnEngineLoad", OnEngineLoad)


function OnRender(dt)
	x, y = getSpritePosition(a)
	setSpritePosition(a, x+1, y)
end
AddEventHandler("OnClientRender", OnRender)

