#ifndef __CLASS_DOOROBJECT_H__
#define __CLASS_DOOROBJECT_H__
#pragma once

#include "Vector3.h"
#include "LogicObject.h"
#include "Utils\LerpAnimator1D.h"

#define DOOR_ANIM_TIME	1.0f

#include <vector>

class CInstanceMesh;
class CPlayer;
class SDoor;

class CDoorObject : public CLogicObject
{
public:
	CDoorObject();
	CDoorObject(CXMLTreeNode &atts);
	~CDoorObject();

	void Update(float ElapsedTime);
	void Trigger(const std::string& action, CPlayer* player);
	
	const std::vector<SDoor*>& GetDoors() { return m_vDoors; }

	bool IsInside() { return m_bInside; }

	void SetMustClose(bool close) { m_bMustClose = close; }
	bool IsOpen();

private:
	void DisplayUse(bool show);
	void Restart();

private:
	std::string m_UseItemFile;
	bool m_bUseActive;
	float m_fUseTime;

	std::vector<SDoor *> m_vDoors;
	bool m_bAuto;
	
	std::string m_sSwitchName;
	bool m_bInside;

	bool m_bMustClose;
};


class SDoor {
public:
	SDoor()
		: m_DoorMesh(NULL), m_Layer(""), m_bIsOpen(false), m_bStart(false), m_bOpening(false), m_fCloseTime(0.0f), m_fTime(0.f),
		  m_fSizeY(0.0f), m_fY(0.0f), m_vPosIni(Vect3f(0.0f, 0.0f, 0.0f)), m_vPosEnd(Vect3f(0.0f, 0.0f, 0.0f))
	{};
	~SDoor(){};

	bool Update(float ElapsedTime, bool mustClose);

	void OpenDoor(float ElapsedTime);
	bool CloseDoor(float ElapsedTime, bool start);

	void SetMesh(CInstanceMesh* mesh) { m_DoorMesh = mesh; }
	void SetLayer(const std::string& layer) { m_Layer = layer;}
	void SetSizeY(float s) { m_fSizeY = s; }
	void SetPosIni(const Vect3f& pos) { m_vPosIni = pos; }
	void SetCloseTime(float t) { m_fCloseTime = t; }
	void SetStart(bool start) { m_bStart = start; }

	float GetCloseTime() { return m_fCloseTime; }
	bool IsOpen() { return m_bIsOpen; }
	bool IsOpening() { return m_bOpening; }

private:
	void UpdatePhysXMeshPosition(const Vect3f& pos);

private:	
	CInstanceMesh *m_DoorMesh;
	std::string m_Layer;
	CLerpAnimator1D m_LearpAnim;
	bool m_bIsOpen, m_bStart, m_bOpening;
	float m_fSizeY, m_fY;
	Vect3f m_vPosIni, m_vPosEnd;
	float m_fCloseTime;
	float m_fTime;
};

#endif
