//----------------------------------------------------------------------------------
// CCamera class
// Author: Enric Vergara
//
// Description:
// Clase base que contiene la informacion generica de una camara la cual esta atachada a un 
// puntero de la clase base CObject3D
//----------------------------------------------------------------------------------
#pragma once
#ifndef INC_CAMERA_H_
#define INC_CAMERA_H_

#include "Object3D.h"
#include <stdio.h>

class CRenderManager;
enum ETypeCamera {TC_FPS = 0, TC_THPS};
class CCamera
{


public:

	CCamera(float zn, float zf, float fov, float aspect, CObject3D* object3D, ETypeCamera typeCamera);
	CCamera();
	virtual ~CCamera(){ m_pObject3D = NULL; }

	//--- GET FUNCTIONS ---
	virtual Vect3f					GetDirection		() const = 0;
	virtual Vect3f					GetLookAt				() const = 0;
	virtual Vect3f					GetEye					() const = 0;
	virtual Vect3f					GetVecUp				() const = 0;
	CObject3D*							GetObject3D			() {return m_pObject3D;}
	float										GetZf						() const { return m_fZFar;}
	float										GetZn						() const { return m_fZNear;}
	float										GetFov					() const { return m_fFOV;}
	float										GetViewD				() const { return m_fView_d;}
	float										GetAspectRatio	() const { return m_fAspectRatio;}
	ETypeCamera							GetTypeCamera		() const { return m_eTypeCamera;}

	//--- SET FUNCTIONS ---
	void										SetZn						(float amount )	{ m_fZNear = amount; }
	void										SetZf						(float amount )	{ m_fZFar = amount; }
	void										SetObject3D			(CObject3D* object3D )	{ m_pObject3D = object3D;}

	//--- ADD FUNCTIONS ---
	void										AddZf						(float amount )			{ m_fZFar += amount; }
	void										AddZn						(float amount )			{ m_fZNear += amount; }
	void										AddFov					(float delta_fov )	{ m_fFOV += delta_fov; }
	void										AddViewD				(float amount )			{ if( m_fView_d + amount > 1) m_fView_d += amount; }


	void SetPlayerYaw(float Yaw) { m_pObject3D->SetYaw(Yaw);}
	float GetPlayerYaw() { return m_pObject3D->GetYaw();}

	void SetPlayerPitch(float Pitch) { m_pObject3D->SetPitch(Pitch);}
	float GetPlayerPitch() { return m_pObject3D->GetPitch();}

	void SetPlayerRoll(float Roll) { m_pObject3D->SetRoll(Roll);}
	float GetPlayerRoll() { return m_pObject3D->GetRoll();}

	bool IsFPC() { return GetTypeCamera()==TC_FPS;}

	virtual void Update(float ElapsedTime){}
	virtual void RenderDebug(CRenderManager &RM){}


protected:

	CObject3D*	m_pObject3D;
	float				m_fView_d;					// variable de debug utilizada para pintar el objeto de la camara.	
	float				m_fFOV;
	float				m_fAspectRatio;
	float				m_fZNear;						// valor del z near (a partir de que vemos)
	float				m_fZFar;						// valor del z far (hasta donde podemos ver)
	ETypeCamera	m_eTypeCamera;		
};

#endif // INC_CAMERA_H_