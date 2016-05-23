#pragma once
#ifndef _PROCESS_TRIGGER_REPORT_H
#define _PROCESS_TRIGGER_REPORT_H

#include "include\PhysicTriggerReport.h"

class CPhysicUserData;

class CProcessTriggerReport: public CPhysicTriggerReport   
{    
public:
	CProcessTriggerReport() {;}
	virtual ~CProcessTriggerReport() {/*Nothing*/;}

	//---NxUserTriggerReport Interface---
	void					onTrigger	(NxShape& triggerShape, NxShape& otherShape, NxTriggerFlag status);
	//-----------------------------------

	//---CPhysicTrigger Interface---
	void	OnEnter		(CPhysicUserData* entity_trigger1, CPhysicUserData* other_shape);
	void	OnLeave		(CPhysicUserData* entity_trigger1, CPhysicUserData* other_shape);
};

#endif // _PHYSIC_TRIGGER_REPORT_H