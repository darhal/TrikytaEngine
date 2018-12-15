#pragma once
#include <vector>
#include <memory>
#include "Timer.h"

class TimerManager
{
public:
	static void InitTimerManager();
	static TimerManager* GetTimerHandler();
	static std::vector<Timer*>& GetTimerManager();
	static void Activate(Timer*);
	static void Deactivate(Timer*);
	static void Update();

	static Timer* CreateTimer(std::function<void()> p_Func, int p_Time, int p_CallTime, bool p_Start=true) {
		return new Timer(p_Func, p_Time, p_CallTime, p_Start);
	}

	static Timer* CreateTimer(int p_Func, int p_Time, int p_CallTime) {
		return new Timer(p_Func, p_Time, p_CallTime);
	}

	static Timer* CreateTimer(int p_Func, int p_Time, int p_CallTime, bool p_Start) {
		return new Timer(p_Func, p_Time, p_CallTime, p_Start);
	}
	
	static int m_TimerNum;
private:
	static TimerManager* _MANAGER_;
	
	TimerManager(){};
	std::vector<Timer*> m_TimerHandler;
};
