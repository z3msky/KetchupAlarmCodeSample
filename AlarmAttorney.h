#ifndef ALARMAttorney_H
#define ALARMAttorney_H

#include "Alarm.h"

class AlarmAttorney
{
public:
	class DeregRef
	{
		friend class AlarmManager;

		static AlarmManager::TimelineRef GetDeregRef(Alarm* al) { return al->GetDeregRef(); };
		static void SetDeregRef(Alarm* al, AlarmManager::TimelineRef ref) { al->SetDeregRef(ref); };
	};

	class Registration
	{
		friend class AlarmRegistrationCommand;
		friend class AlarmDeregistrationCommand;

		static void RegisterToScene(Alarm* a, float sec) { a->RegisterToScene(sec); };
		static void DeregisterFromScene(Alarm* a) { a->DeregisterFromScene(); };
	};

	class Trigger
	{
		friend class AlarmManager;

		static void TriggerAlarm(Alarm* al) { al->TriggerAlarm(); };
	};
};

#endif
