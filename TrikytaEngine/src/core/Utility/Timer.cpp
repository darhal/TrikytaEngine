#include "Timer.h"
#include "TimerManager.h"

Timer::Timer(std::function<void()> p_Func, int p_Time, int p_CallTime):
	m_FuncToCall(p_Func), 
	m_CallPeriod(p_Time), 
	m_CallTime(p_CallTime)
{
	m_TimeLeft = (float)m_CallPeriod;
}

Timer::~Timer()
{
	/*if (m_TimerThread.joinable())
		m_TimerThread.join();*/
}

void Timer::update()
{
	Timer::callFunc();
}

void Timer::start()
{
	m_IsActive = true;
	TimerManager::Activate(this);
	mStart_Time = std::chrono::high_resolution_clock::now();
	//m_TimerThread = std::thread(&Timer::callFunc, this);
	//auto fut = std::async(std::launch::async, &Timer::callFunc, this);
}

void Timer::stop()
{
	//printf("STOP!!\n");
	m_IsActive = false;
	TimerManager::Deactivate(this);
	delete this;
}

void Timer::callFunc()
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
		if (m_CallTime >= 0) {
			m_CallTime--;
			m_FuncToCall();
			mStart_Time = std::chrono::high_resolution_clock::now();
			if (m_CallTime <= 0) {
				Timer::stop();
			}
		}
	}
}

