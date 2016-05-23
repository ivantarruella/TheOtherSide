#define __DONT_INCLUDE_MEM_LEAKS__
#include "TriggerManager.h"
#include "XML\XMLTreeNode.h"
#include "Exception.h"
#include "RenderManager.h"
#include "Logger.h"
#include "include\PhysicActor.h"
#include <iostream>
#include "Scripting\ScriptManager.h"
//#include "BulletManager.h"
#include "Base.h"


CTriggerManager::CTriggerManager()
{
}


CTriggerManager::~CTriggerManager()
{
	Destroy();
}

void CTriggerManager::Init()
{
	CORE->GetPhysicsManager()->SetTriggerReport(this);
}

void CTriggerManager::Destroy(){

	CMapManager::Destroy();

}

void CTriggerManager::SetActive(const std::string& Name, bool Active){
	this->GetResource(Name)->SetActive(Active);
}

void CTriggerManager::Reload(){
	if (m_Path!="")
	{
		Destroy();
		Init();
		Load(m_Path);
	}

}

void CTriggerManager::Load(const std::string &FileName){

	CXMLTreeNode parser;
	if (!parser.LoadFile(FileName.c_str()))
	{
		std::string msg_error = "CTriggerManager::Load->Error al intentar leer el archivo: " + FileName;
		LOGGER->AddNewLog(ELL_ERROR, msg_error.c_str());
		throw CException(__FILE__, __LINE__, msg_error);
	}

	m_Path = FileName;


	int l_count_objects;
	// Obtenemos el nodo "BillboardAnimation"
	CXMLTreeNode  l_TriggerManager = parser["TriggerManager"];
	if (l_TriggerManager.Exists())
	{
		std::string l_Name;
		CTrigger* l_Trigger;

		l_count_objects = l_TriggerManager.GetNumChildren();
		for (int i = 0; i < l_count_objects; ++i){


			//Trigger Node
			l_Name=l_TriggerManager(i).GetPszProperty("name","");
			l_Trigger=new CTrigger(l_TriggerManager(i));

			AddResource(l_Name,l_Trigger);
		}
	}
}







void CTriggerManager::onTrigger( NxShape& triggerShape, NxShape& otherShape, NxTriggerFlag status )       
{           
	NxActor* actor = &triggerShape.getActor();
	CPhysicUserData* entity_trigger1= (CPhysicUserData*)actor->userData; 
	if (!this->GetResource(entity_trigger1->GetName())->GetActive())
		return;

	actor = &otherShape.getActor();
	CPhysicUserData* entity_trigger2= (CPhysicUserData*)actor->userData; 

	if (entity_trigger2->GetName() != "PLAYER" && entity_trigger2->GetName()!="Weapon" )
		return;

	if(status & NX_TRIGGER_ON_ENTER)           
	{               
		OnEnter(entity_trigger1, entity_trigger2);
	}  
	else if(status & NX_TRIGGER_ON_LEAVE)            
	{                
		OnLeave(entity_trigger1, entity_trigger2);     
	}  
	else if(status & NX_TRIGGER_ON_STAY)
	{
		OnTrigger(entity_trigger1, entity_trigger2); 
	}
}  

//---CPhysicTrigger Interface---
void CTriggerManager::OnEnter	(CPhysicUserData* EntityTrigger, CPhysicUserData* OtherShape)
{


	CTrigger * l_Trigger=this->GetResource(EntityTrigger->GetName());
	if(l_Trigger->GetActorName()!="" && OtherShape->GetName()!=l_Trigger->GetActorName()) 
		return;

	SLua *l_Lua=l_Trigger->GetLua(ONENTER);
	std::string l_LuaCode=l_Lua->GetLua();

	float l_LuaPeriode=l_Lua->GetPeriode();
	float l_dtE=CORE->GetElapsedTime();

	if (l_dtE>l_LuaPeriode)
		CORE->GetScriptManager()->RunCode(l_LuaCode);

	
}

void CTriggerManager::OnLeave	(CPhysicUserData* EntityTrigger, CPhysicUserData* OtherShape)
{
	CTrigger * l_Trigger=this->GetResource(EntityTrigger->GetName());
	if(l_Trigger->GetActorName()!="" && OtherShape->GetName()!=l_Trigger->GetActorName()) 
		return;

	SLua *l_Lua=l_Trigger->GetLua(ONLEAVE);
	std::string l_LuaCode=l_Lua->GetLua();

	float l_LuaPeriode=l_Lua->GetPeriode();
	float l_dtE=CORE->GetElapsedTime();

	if (l_dtE>l_LuaPeriode)
		CORE->GetScriptManager()->RunCode(l_LuaCode);
}

void CTriggerManager::OnTrigger	(CPhysicUserData* EntityTrigger, CPhysicUserData* OtherShape)
{
	CTrigger * l_Trigger=this->GetResource(EntityTrigger->GetName());
	if(l_Trigger->GetActorName()!="" && OtherShape->GetName()!=l_Trigger->GetActorName()) 
		return;

	SLua *l_Lua=l_Trigger->GetLua(ONTRIGGER);
	std::string l_LuaCode=l_Lua->GetLua();

	CORE->GetScriptManager()->RunCode(l_LuaCode);
}
