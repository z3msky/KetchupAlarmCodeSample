#ifndef ALARMMANAGER_H
#define ALARMMANAGER_H

#include <map>
#include <chrono>
#include <time.h>

class Alarm;

class AlarmManager
{
public:

	using Timeline = std::multimap<float, Alarm*>;
	using TimelineRef = Timeline::iterator;

	AlarmManager();
	AlarmManager(const AlarmManager&) = delete;
	AlarmManager& operator = (const AlarmManager&) = delete;
	~AlarmManager() = default;

	// User has no access to manager through Scene
	// User should keep their alarm for (de)registration
	float Register(Alarm* alarm, float sec);
	void Deregister(Alarm* alarm);
	void Process();

private:

	Timeline m_timeline;

};

#endif
