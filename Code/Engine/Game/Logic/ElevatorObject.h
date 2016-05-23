#ifndef __CLASS_ELEVATOROBJECT_H__
#define __CLASS_ELEVATOROBJECT_H__
#pragma once

#include "Vector3.h"
#include "LogicObject.h"
#include "Utils\LerpAnimator1D.h"

#include <vector>

#define ELEVATOR_ANIM_BLEND_TIME	1.0f
#define ELEVATOR_ANIM_TOTAL_TIME	4.0f

class CElevatorAnimatedPhysicModel;
class CPlayer;
class CInstanceMesh;

class CElevatorObject : public CLogicObject
{
public:
enum tAnimationStates {
	IDDLEDOWN_ANIM,
	IDDLEUP_ANIM,
	MOVEUP_ANIM,
	MOVEDOWN_ANIM,
};

public:
	CElevatorObject();
	CElevatorObject(CXMLTreeNode &atts);
	~CElevatorObject();

	void Update(float ElapsedTime);
	void Trigger(const std::string& action, CPlayer* player);

	void SetStart(bool start) { m_bStart = start; }
	void SetPlayer(CPlayer* player) { m_player = player; }
	void SetIsUp(bool up) { m_bIsUp = up; }

private:
	void DisplayUse(bool show);
	void UpdateAnimationPhysX();
	void Restart();

private:
	CElevatorAnimatedPhysicModel* m_AnimatedPhysicModel;
	Vect3f m_vElevatorIniPos;
	CInstanceMesh* m_DoorPhysicMesh;
	Vect3f m_vDoorIniPos;
	bool m_bIsUp, m_bStart;

	std::string m_UseItemFile;
	bool m_bUseActive;
	float m_fUseTime;

	CPlayer* m_player;

	std::string m_sOtherElevator;
};

#endif
