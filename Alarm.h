#ifndef Alarm_H
#define Alarm_H

#include "Command.h"
#include "AlarmManager.h"
#include "RegistrationState.h"

class AlarmRegistrationCommand;
class AlarmDeregistrationCommand;

class Alarm : public Command
{
public:
	Alarm();
	Alarm(const Alarm&) = delete;
	Alarm& operator = (const Alarm&) = delete;
	virtual ~Alarm();

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Alarm behavior to be overridden by the user. </summary>
	///
	/// <remarks>	Haru, 3/8/2023. </remarks>
	////////////////////////////////////////////////////////////////////////////////////////////////////
	virtual void AlarmBehavior() = 0;

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Check if this object is registered to a Scene or pending registration. </summary>
	///
	/// <remarks>	Haru, 8/24/2024. </remarks>
	///
	/// <returns>	True if registered or pending, false if not. </returns>
	////////////////////////////////////////////////////////////////////////////////////////////////////
	bool isRegisteredOrPending();

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Submit non-repeating alarm registration to the currently active Scene. </summary>
	///
	/// <remarks>	Haru, 8/24/2024. </remarks>
	///
	/// <param name="sec">	How many seconds into the future the alarm should trigger </param>
	////////////////////////////////////////////////////////////////////////////////////////////////////
	void SubmitRegistration(float sec);

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Submit repeating alarm registration to the currently active Scene. </summary>
	///
	/// <remarks>	Haru, 8/24/2024. </remarks>
	///
	/// <param name="sec">	How many seconds into the future the alarm should trigger </param>
	/// <param name="repeat">	How many times the alarm should repeat </param>
	////////////////////////////////////////////////////////////////////////////////////////////////////
	void SubmitRegistrationRepeat(float sec, int repeat);

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Submit repeating alarm registration to the currently active Scene. </summary>
	///
	/// <remarks>	Haru, 8/24/2024. </remarks>
	///
	/// <param name="sec">	How many seconds should pass between each trigger. </param>
	/// <param name="repeat">	How long the alarm should repeat for. </param>
	////////////////////////////////////////////////////////////////////////////////////////////////////
	void SubmitRegistrationTime(float period, float length);

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Submit alarm deregistration from the currently active scene. </summary>
	///
	/// <remarks>	Haru, 3/8/2023. </remarks>
	////////////////////////////////////////////////////////////////////////////////////////////////////
	void SubmitDeregistration();

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Gets time until the alarm triggers. </summary>
	///
	/// <remarks>	Haru, 3/12/2023. </remarks>
	///
	/// <returns>	The number of seconds until the alarm triggers, or -1 if not currently set. </returns>
	////////////////////////////////////////////////////////////////////////////////////////////////////
	float GetTimeToTrigger();

protected:

	int m_repeat;

private:
	
	float m_period;
	float m_triggerTime;

	RegistrationState m_regState;
	AlarmManager::TimelineRef m_deregRef;
	AlarmRegistrationCommand* p_regCmd;
	AlarmDeregistrationCommand* p_deregCmd;

	friend class AlarmAttorney;

	void RegisterToScene(float sec);
	void DeregisterFromScene();
	virtual void TriggerAlarm();
	AlarmManager::TimelineRef GetDeregRef();
	void SetDeregRef(AlarmManager::TimelineRef ref);

};

#endif 
