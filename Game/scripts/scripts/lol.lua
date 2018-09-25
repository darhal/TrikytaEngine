
function Call()
	print("EVERY 5K MS for 2 times")
end

function OnEngineLoad()
	print("____________________________________________________________________")
	--setTimer(Call, 50, 5)
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

