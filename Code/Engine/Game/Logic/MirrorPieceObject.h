#ifndef __CLASS_MIRRORPIECEOBJECT_H__
#define __CLASS_MIRRORPIECEOBJECT_H__
#pragma once

#include "LogicObject.h"

class CPlayer;
class CInstanceMesh;

class CMirrorPieceObject : public CLogicObject
{
public:
	CMirrorPieceObject();
	CMirrorPieceObject(CXMLTreeNode &atts);
	~CMirrorPieceObject();

	void Update(float ElapsedTime);
	void Trigger(const std::string& action, CPlayer* player);
	
	bool IsActive() { return m_bInfoActive; }

private:	
	void Restart();
	void DisplayInfo(bool show);

private:
	CPlayer* m_player;
	CInstanceMesh *m_Mesh;

	bool m_bStart;
	std::string m_GUIFile;
	bool m_bInfoActive;
};

#endif
