#ifndef __CLASS_MIRROROBJECT_H__
#define __CLASS_MIRROROBJECT_H__
#pragma once

#include "Vector3.h"
#include "LogicObject.h"
#include "Utils\LerpAnimator1D.h"
#include "Camera\Frustum.h"

class CPlayer;
class CInstanceMesh;
class CTexture;
class CRenderManager;
class CFreeCamera;

class CMirrorObject : public CLogicObject
{
public:
	CMirrorObject();
	CMirrorObject(CXMLTreeNode &atts);
	~CMirrorObject();

	void Update(float ElapsedTime);
	void Trigger(const std::string& action, CPlayer* player, const std::string& targetMirror, const std::string& rotatedMirror);

	CInstanceMesh* GetRenderableObject() { return m_MirrorMesh; };

	const std::string& GetMirrorSpawnPoint() { return m_SpawnPointName; }
	float GetMirrorAngleRot() { return m_fAngleRot; }
	bool GetMirrorLinked() { return m_bLinked; } 
	bool GetRotated() { return m_bRotated; }
	void SetRotated(bool rot) { m_bRotated = rot; }
	void SetCurrRotAngle(float angle) { m_fCurrAngle = angle; }

	void Generate();

// Lua
	bool GetMirrorActive() { return m_bMirrorActive; }
	void SetMirrorActive(bool active) { m_bMirrorActive = active; }
	bool GetMirrorON() { return m_bMirrorON; }
	void SetMirrorON(bool on) { m_bMirrorON = on; }
	void SetCanChangeWorld(bool can) { m_bCanChangeWorld = can; }

private:
	void Restart();
	
	void GenerateMirror(CRenderManager* RM);

	void DisplayUse(bool show, bool rot);

	void UpdateMirrorEnabled();
	void UpdateMirrorRotation(float ElapsedTime);
	void UpdateMirrorChangeWorld(float ElapsedTime);

	void Use(CPlayer* player, const std::string& targetMirror,  const std::string& rotatedMirror);
	void Rotate(const std::string& targetMirrorName);
	void CalculateMirrorPlane();
	void RotatePhysXMesh(float angle);

private:
	CLerpAnimator1D m_LearpAnim;
	CInstanceMesh* m_MirrorMesh;
	std::string m_Layer;
	std::string m_SpawnPointName;
	float m_fAngleRot, m_fCurrAngle;
	float m_fUseTime;
	bool m_bLinked;
	bool m_bRotated;
	bool m_bStart, m_bRotating;
	bool m_bAllDisabled;

	std::string m_UseItemFile;
	bool m_bUseActive;

	CTexture *m_MirrorTex;
	CFrustum m_MirrorFrustum;
	Vect3f p[4], pWorld[4];
	D3DXVECTOR3 c,n,u;
	float m_MirrorDarkCoef;

	bool m_bMirrorActive, m_bMirrorON, m_bEnteredTrigger;
	bool m_bChangeWorld;
	float m_fAnimTime;
	CPlayer* m_player;
	std::string m_targetMirror;
	std::string m_rotatedMirror;
	bool m_bCanChangeWorld;

	//CFreeCamera* m_MirrorCam;
};

#endif
