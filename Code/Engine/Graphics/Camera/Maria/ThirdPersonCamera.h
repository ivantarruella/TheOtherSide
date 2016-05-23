#pragma once
#ifndef INC_THIRD_PERSON_CAMERA_H_
#define INC_THIRD_PERSON_CAMERA_H_

#include "ThPSCamera.h"
#include "include\PhysicsManager.h"
#define CAM_ZOOM_RUN_STEP		0.02f		// Zoom in de la camara al correr
#define CAM_ZOOM_AIM_STEP		0.02f		// Zoom in de la camara al apuntar
#define CAM_ZOOM_COLLISION_STEP	0.03f		// Zoom in de la camara al colisonar
#define CAM_ZOOM_SIMPLE_STEP	0.01f

#define CAM_ZOOM_WALK			0.0f
#define CAM_ZOOM_RUN			1.0f
#define CAM_MIN_DIST			1.f	// Distancia al player de la camara a partir de la cual se sube para verlo desde arriba
#define CAM_MAX_DIST			2.5f
#define CAM_DIST				1.3f	// Distancia de la camara

#define CAM_ZOOM_AIM			1.2f
#define CAM_AIM_DIST			0.7f

#define CAM_MAX_HEIGHT			0.6f	// Altura maxima en caso de colision
#define CAM_MIN_HEIGHT			0.0f
#define CAM_LOOKAT_HEIGHT		0.8f	// Altura a la que mira la camara en el player

#define COLLISION_EPSILON		0.2f

class CRenderManager;
class CPhysicsManager;
class CThirdPersonCamera : public CThPSCamera
{
public:
	CThirdPersonCamera(float zn, float zf, float fov, float aspect, CObject3D* pObject3D, float Zoom, float Distance, float Height);
	~CThirdPersonCamera();

	Vect3f GetLookAt() const;
	Vect3f GetEye() const;

	float	  GetHeight() const	{return m_fHeight;}
	void	  SetHeight(float Height) {m_fHeight = Height;};
	float	  GetFrontDistance() const	{return m_fDistance;}
	float	  GetDistance() 	{return m_fDistance/cos(-GetPlayerPitch());}
	void	  SetFrontDistance(float Distance) {m_fDistance = Distance;};



	void Update(float ElapsedTime);
	void RenderDebug(CRenderManager &RM);

	bool IsGoingUp();
	bool CheckCollision();
	bool CheckNextCollision();
	
private:
	float m_fHeight;
	float m_fDistance;
	SCollisionInfo m_CollisionInfo;

};

#endif
