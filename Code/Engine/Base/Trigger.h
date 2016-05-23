#pragma once
#ifndef _TRIGGER_H
#define _TRIGGER_H

#include <string>
#include "Utils\MapManager.h"
#include "Vector3.h"
#include "include\PhysicsManager.h"
class CPhysicActor;
class CXMLTreeNode;
class CPhysicsManager;

enum {ONENTER,ONLEAVE,ONTRIGGER};

struct SLua {
	SLua::SLua(){
		m_sLua="";
		m_fPeriode=0.0f;
		m_fLastTime=0.0f;
	}
	float GetPeriode(){return m_fPeriode;}
	float GetLastTime(){return m_fLastTime;}
	std::string& GetLua(){return m_sLua;}
	void SetLastTime(float LastTime){ m_fLastTime=LastTime;}
	void SetPeriode(float Periode){ m_fPeriode=Periode;}
	void SetLua(std::string& LuaCode){ m_sLua=LuaCode;}
private:
	std::string m_sLua;
	float m_fPeriode;
	float m_fLastTime;
};

class CTrigger 
{
public:
	CTrigger(void): m_Active(true),m_Name(""){}
	CTrigger(CXMLTreeNode &node);
	~CTrigger(void);
	void SetActive(bool Active){m_Active=Active;}
	bool GetActive(){return m_Active;}

	void SetLua(std::string& onEnter,std::string& onLeave,std::string& onTrigger);	
	
	void SetName(std::string& Name){m_Name=Name;}
	std::string& getName(){return m_Name;}

	void SetActorName(std::string &ActorName) { m_ActorName=ActorName;}
	std::string & GetActorName() { return m_ActorName;}

	SLua* GetLua (unsigned int type);

	
	
private:
	std::string m_Name;
	CPhysicUserData * m_UserData;
	CPhysicActor * m_TriggerActor;
	bool m_Active;
	SLua m_OnEnter,m_OnLeave,m_OnTrigger;
	std::string m_ActorName;
	std::string m_Mask;
};

#endif //_TRIGGER_H