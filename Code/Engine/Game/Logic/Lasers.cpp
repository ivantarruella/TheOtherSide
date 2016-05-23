#include "Lasers.h"
#include "Player.h"
#include "Base.h"

CLasers::CLasers()
	: CLogicObject(), m_fTime(0.0f), m_bStart(false), m_player(NULL), m_bOnTrigger(false)
{
	m_Type = LASERS;
}

CLasers::~CLasers()
{
}

void CLasers::Update(float ElapsedTime)
{
	if (m_bStart && m_bOnTrigger && !m_player->IsDead())
	{
		m_fTime += ElapsedTime;

		if (m_fTime >= LASER_TIME_DAMAGE)
		{
			m_player->TakeDamage(LASER_PLAYER_DAMAGE);
			m_fTime = 0.0f;
		}
	}
}

void CLasers::Trigger(const std::string& action, CPlayer* player)
{
	if (GetEnabled())
	{
		if (action=="OnEnter")
		{
			m_bOnTrigger = true;
		}
		else if (action=="OnLeave")
		{
			m_bStart = false;
			m_bOnTrigger = false;
		}
		else if (action=="OnTrigger")
		{
			if (m_bOnTrigger)
			{
				m_player = player;
				bool dead = m_player->IsDead();
				SetEnabled(!dead);
				m_bStart = !dead;
				m_bOnTrigger = !dead;
			}
		}
	}
	else
	{
		m_bStart = false;
		m_bOnTrigger = false;
	}
}


void CLasers::Restart()
{
	SetEnabled(true); 
	m_bStart = false; 
	m_bOnTrigger = false;
}