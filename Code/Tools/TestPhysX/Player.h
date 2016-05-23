#ifndef __CLASS_PLAYER_H__
#define __CLASS_PLAYER_H__

#pragma once

#include "Object3D.h"
#include "Color.h"
#include "Matrix44.h"

#define VIDA_PLAYER		100

class CRenderManager;
class CPhysicController;

class CPlayer : public CObject3D
{
public:
	CPlayer()
		: m_Size(10.0f, 10.0f, 10.0f), m_fTime(0.0f), m_Color(colMAGENTA), m_iVida(0),m_PhysicController(NULL)
	{
		m_Position(0.0f, 3.0f, 0.0f);
		m_iVida = VIDA_PLAYER;
		m_CreatePhysics=true;
		Init();
	}
	virtual ~CPlayer() ;

	void Init();
	void Update(float ElapsedTime);
	void Render(CRenderManager* rendermanager);

	void SetVidaPlayer(int vida) { m_iVida = vida; }
	int GetVidaPlayer() { return m_iVida; }

private:
	void UpdateInputActions(float ElapsedTime);

private:
	Vect3f m_Size;
	float m_fTime;
	CColor m_Color;
	int m_iVida;
	CPhysicController* m_PhysicController;
};

#endif	// __CLASS_PLAYER_H__