#pragma once
#ifndef INC_THIRD_PERSON_CAMERA_H_
#define INC_THIRD_PERSON_CAMERA_H_

#include "ThPSCamera.h"
#include "include\PhysicsManager.h"

#define CAM_ZOOM_WALK			0.0f
#define CAM_DIST				1.3f	// Distancia de la camara
#define CAM_DIST_AIM			0.8f
#define CAM_MIN_HEIGHT			0.0f
#define CAM_HEIGHT				0.7f

#define CAM_ZOOM_COLLISION_STEP		0.01f	// Zoom in de la camara al colisonar
#define CAM_MIN_DIST				1.f		// Distancia al player de la camara a partir de la cual se sube para verlo desde arriba
#define CAM_MAX_HEIGHT				0.1f	// Altura maxima en caso de colision
#define CAM_LOOKAT_HEIGHT			0.8f	// Altura a la que mira la camara en el player
#define COLLISION_EPSILON			0.2f

#define PLAYER_DISTANCE_FROM_CENTER	0.1f

class CRenderManager;
class CPhysicsManager;
class CThirdPersonCamera : public CThPSCamera
{
public:
	CThirdPersonCamera(float zn, float zf, float fov, float aspect, CObject3D* pObject3D, float Zoom, float Distance, float Height);
	~CThirdPersonCamera();

	Vect3f GetLookAt() const;
	Vect3f GetEye() const;
	Vect3f GetEyePoint() const;

	float	  GetHeight() const	{return m_fHeight;}
	void	  SetHeight(float Height) {m_fHeight = Height;};
	float	  GetFrontDistance() const	{return m_fDistance;}
	float	  GetDistanceFromCenter() const	{return m_fDistanceFromCenter;}
	float	  GetDistance() 	{return m_fDistance/cos(-GetPlayerPitch());}
	void	  SetFrontDistance(float Distance) {m_fDistance = Distance;};
	void	  SetDistanceFromCenter(float Distance) {m_fDistanceFromCenter = Distance;};
	Vect3f   GetCollisionPoint ();

	void Update(float ElapsedTime);
	void RenderDebug(CRenderManager &RM);

private:
	bool CheckCollision();
	bool CheckNextCollision();

private:
	float m_fHeight;
	float m_fDistance;
	float m_fDistanceFromCenter;
	SCollisionInfo m_CollisionInfo;

};

#endif
