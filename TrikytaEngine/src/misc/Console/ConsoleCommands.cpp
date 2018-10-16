#include <LStateManager/LStateManager.h>
#include "core/Common/defines.h"
#include "ConsoleCommands.h"
#include "Console.h"
#include "core/Physics/PhysicsEngine.h"

void ConsoleCommands::initCommands()
{
	m_ArgFunc = {
		{
			"restart", // adding a command restart!
			[](const std::vector<std::string>& args)
			{
				LogConsole(LogInfo, "Restarting script %s ...", args.at(0).c_str());
				LuaEngine::LStateManager::GetLStateManager()->RestertScript(args.at(0));
			}
		},
		{
			"physics_debug", // adding a command restart!
			[](const std::vector<std::string>& args)
			{
				if (args.at(0) == "true") {
					Physics2D::PhysicsEngine::GetPhysicsWorld()->setDebugger();
					LogConsole(LogInfo, "Physics debugger enabled!");
				}else {
					Physics2D::PhysicsEngine::GetPhysicsWorld()->deleteDebugger();
					LogConsole(LogInfo, "Physics debugger disabled!");
				}
			}
		},
		
	};

	for (auto& itr : m_ArgFunc) {
		Console::AddCommandHandler(itr.m_Command, itr.m_CmdFunction);
	}
}
