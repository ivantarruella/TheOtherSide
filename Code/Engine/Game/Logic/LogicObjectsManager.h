#ifndef __CLASS_LOGICOBJECTSMANAGER_H__
#define __CLASS_LOGICOBJECTSMANAGER_H__
#pragma once

#include "LogicObject.h"
#include "Utils/VectorMapManager.h"


class CLogicObjectsManager : public CVectorMapManager<CLogicObject>
{
protected:
	std::string m_FileName;

public:
	CLogicObjectsManager();
	~CLogicObjectsManager();

	void Update(float ElapsedTime);
	void RestartAllObjects();

	bool Load(const std::string &FileName);
	bool Reload();
	void RenderMirrors();

	void SetObjectEnabled(const std::string& Name, bool state);
	void SetAllObjectsEnabled(CLogicObject::TLogicType logic, bool state);
	
	void SetMirrorRenderActive(const std::string& Name, bool state);
	bool GetMirrorRenderActive(const std::string& Name);
	void RestartMirrorRotation();
	void SetMirrorON(const std::string& Name, bool on);
	void CloseDoor(const std::string& Name);

	bool GetTutorialActive();

private:
	float m_fTime;
};

#endif