#pragma once
#include <functional>
#include <chrono>
//#include <thread>

class Timer
{
public:
	Timer(std::function<void()>, int, int);
	~Timer();

	void update();
	void start();
	void stop();
	void callFunc();
	bool IsActive() const {return m_IsActive;}

private:
	int m_CallPeriod;
	bool m_IsLoop;
	bool m_IsActive;
	int m_CallTime;
	float m_TimeLeft;
	std::chrono::steady_clock::time_point  mStart_Time;
	std::function<void()> m_FuncToCall;
	//std::thread m_TimerThread;
};