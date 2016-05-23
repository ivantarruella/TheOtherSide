#ifndef __CLASS_TUTORIALOBJECT_H__
#define __CLASS_TUTORIALOBJECT_H__
#pragma once

#include "XML\XMLTreeNode.h"
#include "LogicObject.h"

#define TUTORIAL_USE_TIME		0.1f

class CPlayer;

class CTutorialObject : public CLogicObject
{
public:
	CTutorialObject();
	CTutorialObject(CXMLTreeNode &atts);
	~CTutorialObject();

	void Update(float ElapsedTime);
	void Trigger(const std::string& action, CPlayer* player);

	bool IsActive() { return m_bInfoActive; }

private:
	void DisplayUse(bool show);
	void DisplayInfo(bool show);
	void Restart(){};

private:
	float m_fTime;
	std::string m_UseItemFile;	// xml del gui donde esta el boton usar/rotar
	std::string m_XMLFile;		// xml del gui donde esta el tutorial
	bool m_bUseActive;			// 'usar' en pantalla
	bool m_bInfoActive;			// tutorial en pantalla
	bool m_bIsAuto;				// tutorial tipo 'auto', no requiere del boton usar para ser mostrado
};

#endif