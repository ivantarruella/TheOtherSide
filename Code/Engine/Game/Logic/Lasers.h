#ifndef __CLASS_LASERS_H__
#define __CLASS_LASERS_H__
#pragma once

#include "LogicObject.h"

#define LASER_PLAYER_DAMAGE		0.25f	//0.03f
#define	LASER_TIME_DAMAGE		0.001f

class CPlayer;

class CLasers : public CLogicObject
{
public:
	CLasers();
	~CLasers();

	void Update(float ElapsedTime);
	void Trigger(const std::string& action, CPlayer* player);

private:
	void Restart();

private:
	float m_fTime;
	bool m_bStart, m_bOnTrigger;
	CPlayer* m_player;
};

#endif
