#define __DONT_INCLUDE_MEM_LEAKS__
#include "Trigger.h"
#include "include\PhysicActor.h"
#include "XML\XMLTreeNode.h"
#include "include\PhysicsManager.h"

#include "Base.h"


CTrigger::CTrigger(CXMLTreeNode &node){

	m_Name=node.GetPszProperty("name","");
	m_ActorName=node.GetPszProperty("actor_name","");
	m_Mask=node.GetPszProperty("mask","");

	Vect3f l_Position=node.GetVect3fProperty("pos",Vect3f(0.0f));
	Vect3f l_Size=node.GetVect3fProperty("size",Vect3f(0.0f));
	
	//Creacion de trigger actor
	m_UserData= new CPhysicUserData(m_Name);
	m_UserData->SetPaint(true);
	m_TriggerActor = new CPhysicActor(m_UserData);

	int l_ColGroup ;
	if(m_Mask != "")
		l_ColGroup = GetCollisionGroup(m_Mask);
	else
		l_ColGroup = ECG_TRIGGERS;// 1<<0


  	m_TriggerActor->CreateBoxTrigger(l_Position,l_Size, l_ColGroup);
	CORE->GetPhysicsManager()->AddPhysicActor(m_TriggerActor);
	m_UserData->SetActor(m_TriggerActor);

	bool l_Active=node.GetBoolProperty("active",true);
	SetActive(l_Active);

	CXMLTreeNode  l_TriggerNode= node;
	int l_count_nodes = l_TriggerNode.GetNumChildren();
	std::string onEnter="",onLeave="",onTrigger="",l_Lua;
	for (int j = 0; j < l_count_nodes; ++j){

		l_Lua=l_TriggerNode(j).GetPszProperty("lua","");

		if(strcmp(l_TriggerNode(j).GetName(),"onEnter")==0)
			onEnter=l_Lua;
		else if(strcmp(l_TriggerNode(j).GetName(),"onLeave")==0)
			onLeave=l_Lua;
		else if(strcmp(l_TriggerNode(j).GetName(),"onTrigger")==0)
			onTrigger=l_Lua;
	}
	SetLua(onEnter,onLeave,onTrigger);
	
	
}

CTrigger::~CTrigger(void){
	if (m_TriggerActor!=NULL)
		CORE->GetPhysicsManager()->ReleasePhysicActor(m_TriggerActor);
	
	CHECKED_DELETE(m_TriggerActor);
	CHECKED_DELETE(m_UserData);
}

void CTrigger::SetLua(std::string& onEnter,std::string& onLeave,std::string& onTrigger){

	m_OnEnter.SetLua(onEnter);
	m_OnLeave.SetLua(onLeave);
	m_OnTrigger.SetLua(onTrigger);
}

SLua* CTrigger::GetLua (unsigned int type){
	SLua *l_sLua;
	switch(type){

	case ONENTER:
		l_sLua=&m_OnEnter;
		break;
	case ONLEAVE:
		l_sLua=&m_OnLeave;
		break;
	case ONTRIGGER:
		l_sLua=&m_OnTrigger;
		break;


	}
	return l_sLua;
}