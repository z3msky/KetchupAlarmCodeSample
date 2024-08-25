#include "AlarmManager.h"
#include "TimeManager.h"
#include "AlarmAttorney.h"

AlarmManager::AlarmManager()
{

}

float AlarmManager::Register(Alarm* al, float sec)
{
	float triggerTime = TimeManager::GetTime() + sec;
	TimelineRef ref = m_timeline.insert({triggerTime, al});
	AlarmAttorney::DeregRef::SetDeregRef(al, ref);

	return triggerTime;
}

void AlarmManager::Deregister(Alarm* al)
{
	TimelineRef ref = AlarmAttorney::DeregRef::GetDeregRef(al);
	m_timeline.erase(ref);
}

void AlarmManager::Process()
{
	while (m_timeline.size() > 0 && m_timeline.begin()->first <= TimeManager::GetTime())
	{
		//Trace::out("Trigger alarm at %f\n", TimeManager::GetTime());

		Alarm* al = m_timeline.begin()->second;
		AlarmAttorney::Trigger::TriggerAlarm(al);
		m_timeline.erase(m_timeline.begin());
	}
}