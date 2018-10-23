#include "TimerManager.h"
#include "core/Common/defines.h"
#include <algorithm>

TimerManager* TimerManager::_MANAGER_ = nullptr;
int TimerManager::m_TimerNum = 0;

void TimerManager::InitTimerManager()
{
	if (TimerManager::_MANAGER_ == nullptr) {
		TimerManager::_MANAGER_ = new TimerManager();
	}
}

TimerManager* TimerManager::GetTimerHandler()
{
	return _MANAGER_;
}

std::vector<Timer*>& TimerManager::GetTimerManager()
{
	return TimerManager::GetTimerHandler()->m_TimerHandler;
}

void TimerManager::Deactivate(Timer* T)
{
	m_TimerNum--;
	if (TimerManager::m_TimerNum == 0) {
		TimerManager::GetTimerManager().clear();
		return;
	}
	TimerManager::GetTimerManager().erase(std::find(TimerManager::GetTimerManager().begin(), TimerManager::GetTimerManager().end(), T));
}

void TimerManager::Activate(Timer* T)
{
	TimerManager::GetTimerManager().push_back(T); 
	m_TimerNum++;
}

void TimerManager::Update()
{
	if (TimerManager::m_TimerNum != 0) {
		for (auto T : TimerManager::GetTimerManager()) {
			T->update();
		}
		return;
	}
}