#ifndef __CLASS_BROKEN_MIRROR_OBJECT_H__
#define __CLASS_BROKEN_MIRROR_OBJECT_H__
#pragma once

#include "XML\XMLTreeNode.h"
#include "LogicObject.h"

#include <string>

#define TOTAL_BROKEN_MIRROR_PIECES	4

class CPlayer;
class CInstanceMesh;

class CBrokenMirrorObject : public CLogicObject
{
public:
	CBrokenMirrorObject();
	CBrokenMirrorObject(CXMLTreeNode &atts);
	~CBrokenMirrorObject();

	void Update(float ElapsedTime);
	void Trigger(const std::string& action, CPlayer* player);

	const std::string& GetFinalMirrorTrigger() { return m_sFinalMirrorTrigger; }
	CInstanceMesh* GetBrokenMirrorMesh() { return m_BrokenMirrorMesh; }

	bool IsActive() { return m_bTextActive; }

private:
	void DisplayUse(bool show);
	void Restart();

private:
	std::string m_UseItemFile;	// xml del gui donde esta el grafico de montar espejo
	std::string m_sNotComplete;	// xml del gui donde esta el texto de que te faltan piezas
	bool m_bUseActive;			// 'montar' en pantalla
	bool m_bTextActive;			// 'te faltan trozos' en pantalla

	CPlayer* m_player;
	CInstanceMesh* m_BrokenMirrorMesh;
	std::string m_sFinalMirrorTrigger;
	std::string m_sLink;

	float m_fTime;
	bool m_bIsBroken;
	bool m_bStart, m_bDone;
};

#endif