#pragma once

#ifndef _TRIGGER_MANAGER_H
#define _TRIGGER_MANAGER_H
#define __DONT_INCLUDE_MEM_LEAKS__
#include <string>
#include "Utils\MapManager.h"
#include "Trigger.h"
#include "include\PhysicTriggerReport.h"
class CPhysicUserData;



class CTriggerManager: public CPhysicTriggerReport , public CMapManager<CTrigger>
{
public:
	CTriggerManager();
	virtual ~CTriggerManager();
	void SetActive(const std::string& Name, bool Active);

	void Init();

	//---NxUserTriggerReport Interface---
	void					onTrigger	(NxShape& triggerShape, NxShape& otherShape, NxTriggerFlag status);
	//-----------------------------------

	//---CPhysicTrigger Interface---
	void	OnEnter		(CPhysicUserData* EntityTrigger, CPhysicUserData* OtherShape);
	void	OnLeave		(CPhysicUserData* EntityTrigger, CPhysicUserData* OtherShape);
	void    OnTrigger	(CPhysicUserData* EntityTrigger, CPhysicUserData* OtherShape);


	void Reload();
	void Load(const std::string &FileName);
	void Destroy();



private:
	 
	std::string m_Path;
};


#endif //_TRIGGER_MANAGER_H
