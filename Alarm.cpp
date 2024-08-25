#include "Alarm.h"
#include "SceneManager.h"
#include "SceneAttorney.h"
#include "TimeManager.h"
#include "SceneAttorney.h"
#include "AlarmRegistrationCommand.h"
#include "AlarmDeregistrationCommand.h"

Alarm::Alarm()
	: m_period(-1), m_triggerTime(-1), m_repeat(1), m_regState(RegistrationState::IS_DEREGISTERED),
	  p_regCmd(new AlarmRegistrationCommand(this)), p_deregCmd(new AlarmDeregistrationCommand(this))
{
}

Alarm::~Alarm()
{
	delete p_regCmd;
	delete p_deregCmd;
}

bool Alarm::isRegisteredOrPending()
{
	return m_regState == RegistrationState::IS_REGISTERED || m_regState == RegistrationState::PENDING_REGISTRATION;
}

void Alarm::SubmitRegistration(float sec)
{
	//Trace::out("Alarm: Submit Registration SceneRegistrationCommand\n");

	assert(m_regState == RegistrationState::IS_DEREGISTERED && "Improper registration state before registration submit");

	p_regCmd->SetTimeInSec(sec);
	SceneAttorney::RegCmd::SubmitCommand(SceneManager::GetCurrentScene(), p_regCmd);
	m_regState = RegistrationState::PENDING_REGISTRATION;

	m_period = sec;
	m_repeat = 1;
}

void Alarm::SubmitRegistrationRepeat(float sec, int repeat)
{
	SubmitRegistration(sec);
	m_repeat = repeat;
}

void Alarm::SubmitRegistrationTime(float period, float length)
{
	int rep = (int)(length / period);

	SubmitRegistrationRepeat(period, rep);
	m_repeat = rep;
}

void Alarm::SubmitDeregistration()
{
	//Trace::out("Alarm: Submit Deregistration SceneRegistrationCommand\n");

	assert(m_regState == RegistrationState::IS_REGISTERED && "Improper registration state before deregistration submit");

	SceneAttorney::RegCmd::SubmitCommand(SceneManager::GetCurrentScene(), p_deregCmd);
	m_regState = RegistrationState::PENDING_DEREGISTRATION;
}

void Alarm::RegisterToScene(float sec)
{
	//Trace::out("Alarm: Register\n");

	assert(m_regState == RegistrationState::PENDING_REGISTRATION && "Improper registration state before registration");

	m_triggerTime = SceneAttorney::AlarmReg::Register(SceneManager::GetCurrentScene(), this, sec);
	m_regState = RegistrationState::IS_REGISTERED;
}

void Alarm::DeregisterFromScene()
{
	//Trace::out("Alarm: Deregister\n");

	assert(m_regState == RegistrationState::PENDING_DEREGISTRATION && "Improper registration state before deregistration");

	SceneAttorney::AlarmReg::Deregister(SceneManager::GetCurrentScene(), this);
	m_regState = RegistrationState::IS_DEREGISTERED;
	m_triggerTime = -1;
}

float Alarm::GetTimeToTrigger()
{
	if (m_regState != RegistrationState::IS_REGISTERED)
	{
		assert(m_triggerTime == -1 && "Alarm regState and triggerTime mismatch");
		return -1;
	}

	return m_triggerTime - TimeManager::GetTime();
}

void Alarm::TriggerAlarm()
{
	assert(m_regState == RegistrationState::IS_REGISTERED && "Improper registration state at Trigger");

	m_regState = RegistrationState::IS_DEREGISTERED;
	m_triggerTime = -1;
	AlarmBehavior();

	if (m_repeat > 1)
	{
		m_repeat--;
		SubmitRegistrationRepeat(m_period, m_repeat);
	}
}

AlarmManager::TimelineRef Alarm::GetDeregRef()
{
	return m_deregRef;
}

void Alarm::SetDeregRef(AlarmManager::TimelineRef ref)
{
	m_deregRef = ref;
}
