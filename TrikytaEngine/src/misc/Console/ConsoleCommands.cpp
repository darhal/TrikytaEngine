#include <LStateManager/LStateManager.h>
#include "core/Common/defines.h"
#include "ConsoleCommands.h"
#include "Console.h"
#include "core/Physics/PhysicsEngine.h"
#include <numeric>

void ConsoleCommands::initCommands()
{
	m_ArgFunc = {
		{
			"restart", // adding a command restart!
			[](const std::vector<std::string>& args)
			{
				if (args.size() == 0) return;
				LogConsole(LogInfo, "Restarting script %s ...", args.at(0).c_str());
				LuaEngine::LStateManager::GetLStateManager()->RestertScript(args.at(0));
			}
		},
		{
			"physics_debug",
			[](const std::vector<std::string>& args)
			{
				if (args.size() == 0) return;
				if (args.at(0) == "true") {
					Physics2D::PhysicsEngine::GetPhysicsWorld()->setDebugger();
					LogConsole(LogInfo, "Physics debugger enabled!");
				}else {
					Physics2D::PhysicsEngine::GetPhysicsWorld()->deleteDebugger();
					LogConsole(LogInfo, "Physics debugger disabled!");
				}
			}
		},
		{
			"run",
			[](const std::vector<std::string>& args)
			{
				if (args.size() == 0) return;
				std::string code;
				//code = std::accumulate(args.begin(), args.end(), code);
				for (auto const& s : args) { code += s + " "; }
				LogConsole(LogInfo, "Running Code...");
				LuaEngine::LStateManager::GetLStateManager()->LoadCode(code);
			}
		},
	};

	for (auto& itr : m_ArgFunc) {
		Console::AddCommandHandler(itr.m_Command, itr.m_CmdFunction);
	}
}
