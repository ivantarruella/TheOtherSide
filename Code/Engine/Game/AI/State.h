#pragma once
#ifndef INC_STATE_H_
#define INC_STATE_H_

#include "Renderer\RenderableObjectsLayersManager.h"
#include "RenderableObjectsManager.h"
#include "Utils\VectorMapManager.h"
#include "Base.h"

#include <string>

inline float degToDot( float degrees ) {
	float l_aux = degrees /360.0f;
	return 1 * (1 - l_aux) - 1 *l_aux;
}

class CStateMachine;
class CCharacter;
class CMonster;

class CState
{
protected:
	std::string m_Name;	
	CStateMachine* m_StateMachine;
	float m_Time;
	CCharacter* m_Player;
	CMonster* m_Owner;
	
	float m_HearDistance;
	float m_VisionDistance;
	float m_VisionAngle;
	float m_WalkSpeed;

public:
	CState()
		: m_HearDistance(6.0f)
		, m_VisionDistance(16.0f)
		, m_VisionAngle(180.0f)
		, m_WalkSpeed(0.030f)
		, m_Player(NULL)
	{
		m_Player=(CCharacter*)CORE->GetRenderableObjectsLayersManager()->GetResource("solid")->GetInstance("PLAYER");
	};
	
	virtual ~CState(){};

	virtual void Create()=0;
	virtual void Update(float ElapsedTime)=0;
	virtual void Init()=0;
	virtual void Reset()=0;

	virtual void OnEnter(){};
	virtual void OnExit(){};
		
	float getTime(){return m_Time;};
	void setTime(const float &time){m_Time = time;};

	std::string getName(){return m_Name;};
	void setName(const std::string &name){m_Name = name;};

	CStateMachine* getStateMachine(){return m_StateMachine;};
	void setStateMachine(CStateMachine* stateMachine){m_StateMachine = stateMachine;};
		
	void SetPlayer(CCharacter* player){m_Player = player;};
	void SetOwner(CMonster* owner){m_Owner = owner;};
};

#endif
