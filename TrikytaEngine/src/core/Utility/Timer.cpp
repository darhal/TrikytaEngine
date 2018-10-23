#include "Timer.h"
#include "TimerManager.h"
#include <LuaMisc/LuaTimer.h>
#include "core/Common/defines.h"
#include <chrono>

Timer::Timer(std::function<void()> p_Func, int p_Time, int p_CallTime):
	m_FuncToCall(p_Func), 
	m_CallPeriod(p_Time), 
	m_CallTime(p_CallTime)
{
	m_TimeLeft = (float)m_CallPeriod;
}

Timer::Timer(std::function<void()> p_Func, int p_Time, int p_CallTime, bool p_Start) :
	m_FuncToCall(p_Func),
	m_CallPeriod(p_Time),
	m_CallTime(p_CallTime)
{
	m_TimeLeft = (float)m_CallPeriod;
	if (p_Start) {
		Timer::start();
	}
}

Timer::Timer(int p_LuaFuncIndex, int p_Time, int p_CallTime) :
	m_LuaFunc(p_LuaFuncIndex),
	m_CallPeriod(p_Time),
	m_CallTime(p_CallTime)
{
	m_TimeLeft = (float)m_CallPeriod;
}

Timer::Timer(int p_LuaFuncIndex, int p_Time, int p_CallTime, bool p_Start) :
	m_LuaFunc(p_LuaFuncIndex),
	m_CallPeriod(p_Time),
	m_CallTime(p_CallTime)
{
	m_TimeLeft = (float)m_CallPeriod;
	if (p_Start) {
		Timer::start();
	}
}

Timer::~Timer()
{
	/*if (m_TimerThread.joinable())
		m_TimerThread.join();*/
}

void Timer::update()
{
	Timer::updateTimer();
}

void Timer::start()
{
	m_IsActive = true;
	TimerManager::Activate(this);
	mStart_Time = std::chrono::high_resolution_clock::now();
	//m_TimerThread = std::thread(&Timer::callFunc, this);
}

void Timer::stop()
{
	m_IsActive = false;
	TimerManager::Deactivate(this);
	if (m_LuaFunc != -1)
		LuaEngine::LuaTimer::RemoveFuncFromRegistery(m_LuaFunc);
	delete this;
}

void Timer::updateTimer()
{
	/*while (m_IsActive && m_CallTime >= 0) {
		std::this_thread::sleep_for(std::chrono::milliseconds(m_CallPeriod));
		m_CallTime--;
		m_TimeLeft = 0;
		m_FuncToCall();
		if (m_CallTime <= 0) {
			
			Timer::stop();
		}
	}*/
	auto end_time = std::chrono::high_resolution_clock::now();
	auto time = end_time - mStart_Time;
	m_TimeLeft = (float)std::chrono::duration_cast<std::chrono::milliseconds>(time).count();
	if (m_IsActive && m_TimeLeft >= m_CallPeriod) {
		if (m_CallTime != -1) {
			m_CallTime--;
		}
		callFunc();
		mStart_Time = std::chrono::high_resolution_clock::now();
		if (m_CallTime == 0) {
			Timer::stop();
		}
	}
}

void Timer::callFunc()
{
	if (m_FuncToCall != NULL) { // Normal C++ function
		m_FuncToCall();
	}else if (m_LuaFunc != -1) { // LUA FUNCTION
		LuaEngine::LuaTimer::GetTimerManager()->CallTimerFunc(m_LuaFunc);
	}
}

