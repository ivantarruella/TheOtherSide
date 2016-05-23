#include "ThirdPersonCamera.h"
#include "include\PhysicsManager.h"
#include "RenderManager.h"
#include "Math\MathUtils.h"
#include "Base.h"


CThirdPersonCamera::CThirdPersonCamera(float zn, float zf, float fov, float aspect, CObject3D* pObject3D, float Zoom, float Distance, float Height)
	: CThPSCamera(zn, zf, fov, aspect, pObject3D, Zoom), 
	m_fDistance(Distance), m_fHeight(Height), m_CollisionInfo(SCollisionInfo())
{
	//Hacemos una copia del Object3D del player para
	//que cuando la camara rote no lo haga el player tambien
	m_pObject3D=new CObject3D(*pObject3D);
	CPhysicElement* l_Elem;
	if(pObject3D->GetPhysicElement()!=NULL){
		l_Elem= new CPhysicElement(*pObject3D->GetPhysicElement());}
	else
		l_Elem= new CPhysicElement();

	m_pObject3D->SetPhysicElement(l_Elem);


}

CThirdPersonCamera::~CThirdPersonCamera()
{
	//Elimina la copia que se hizo al object3d del player al crear la camara
	CHECKED_DELETE(m_pObject3D);

}

Vect3f CThirdPersonCamera::GetLookAt () const
{
	float l_fYaw = m_pObject3D->GetYaw();
	Vect3f l_Position(m_fDistance*0.4f * sin(l_fYaw), CAM_LOOKAT_HEIGHT, -m_fDistance*0.4f * cos(l_fYaw));
	assert(m_pObject3D);
	return (m_pObject3D->GetPosition() + l_Position);
	

	//Sumem pk la camara mire al cap
	//return m_pObject3D->GetPosition()+Vect3f(0.0f,CAM_LOOKAT_HEIGHT,0.0f);

}

Vect3f CThirdPersonCamera::GetEye() const
{
	/*float l_fYaw = m_pObject3D->GetYaw();
	Vect3f l_Position(m_fDistance * sin(l_fYaw), m_fHeight, -m_fDistance * cos(l_fYaw));

	return (CThPSCamera::GetLookAt() + l_Position); */
	assert(m_pObject3D);

	float yaw		= m_pObject3D->GetYaw();
	float pitch	= m_pObject3D->GetPitch();
	Vect3f pos	= GetLookAt();

	float l_Distance=m_fDistance/cos(-pitch);

	//Pasamos de coordenadas esfericas a coordenadas cartesianas
	Vect3f vEyePt(	m_fDistance * cos(yaw) * cos(pitch), 
		m_fDistance * sin(pitch)+m_fHeight,
		m_fDistance * sin(yaw) * cos(pitch) );

	return (pos - vEyePt);
}

bool CThirdPersonCamera::IsGoingUp(){
	if(m_fHeight<0.0f)
		return true;
	else
		return false;
}

bool CThirdPersonCamera::CheckCollision(){


	Vect3f l_pos(GetLookAt()),l_dir(GetEye()-l_pos);

	l_dir=l_dir.Normalize();

	SCollisionInfo l_Collision;


	CPhysicUserData * l_UserData=CORE->GetPhysicsManager()->RaycastClosestActor(l_pos,l_dir ,1<<0, m_CollisionInfo );

	if(l_UserData==NULL) 
		return false;
	else {

		float l_CollisionDistance=m_CollisionInfo.m_fDistance;
		float l_CameraDistance=GetEye().Distance(l_pos);

		if(l_CollisionDistance>l_CameraDistance)
			return false;
		else return true;
		

	}


}

bool CThirdPersonCamera::CheckNextCollision(){

	float l_Distance=m_fDistance;
	m_fDistance=CAM_DIST;

	Vect3f l_pos(GetLookAt()),l_dir(GetEye()-l_pos);

	l_dir=l_dir.Normalize();

	SCollisionInfo l_Collision;
	

	CPhysicUserData * l_UserData=CORE->GetPhysicsManager()->RaycastClosestActor(l_pos,l_dir ,1<<0, m_CollisionInfo );

	if(l_UserData==NULL) 
		return false;
	else {

		float l_CollisionDistance=m_CollisionInfo.m_fDistance;
		float l_CameraDistance=GetEye().Distance(l_pos);
		m_fDistance=l_Distance;
		if(l_CollisionDistance>l_CameraDistance+COLLISION_EPSILON)
			return false;
		else return true;
		

	}
}


void CThirdPersonCamera::Update(float ElapsedTime){

	Vect3f l_LookAt=GetLookAt();

	float l_DistanceBeforeCollision=GetEye().Distance(m_pObject3D->GetPosition());
	
	if(!CheckCollision())
	{
		float l_Pitch=GetPlayerPitch();
		if(m_fDistance<CAM_DIST){
			//Comprobamos que con el nuevo movimiento no colisionara
			if(!CheckNextCollision()){
						
				m_fDistance+=CAM_ZOOM_COLLISION_STEP;
			}

			// Menor del minimo --> Subimos la camara si estamos muy cerca (con colision)
			if(m_fDistance<CAM_MIN_DIST && m_fHeight>-CAM_MAX_HEIGHT){
				m_fHeight-=CAM_ZOOM_COLLISION_STEP;
				float l_Distance=GetEye().Distance(m_pObject3D->GetPosition());
				l_Pitch=-asin(m_fHeight/l_Distance);
			}

		}
		//Bajamos la camara a la posicion original
		if(m_fDistance>=CAM_MIN_DIST && m_fHeight<CAM_MIN_HEIGHT){
			m_fHeight+=CAM_ZOOM_COLLISION_STEP;
			float l_Distance=GetEye().Distance(m_pObject3D->GetPosition());
			l_Pitch=-asin(m_fHeight/l_Distance);

		}
		SetPlayerPitch(l_Pitch);
	}

	//Si hay colision
	else{
		//Bajamos la camara a la posicion original

		float l_CollisionDistance=m_CollisionInfo.m_fDistance;
		m_fDistance=abs(cos(GetPlayerPitch()))*l_CollisionDistance;
		if(m_fDistance>=COLLISION_EPSILON)
			m_fDistance-=COLLISION_EPSILON;

		if(m_fDistance<CAM_MIN_DIST && m_fHeight>-CAM_MAX_HEIGHT){
			m_fHeight-=CAM_ZOOM_COLLISION_STEP;
			float l_Distance=GetEye().Distance(m_pObject3D->GetPosition());
			SetPlayerPitch(-asin(m_fHeight/l_Distance));
		}
		if(m_fDistance>=CAM_MIN_DIST && m_fHeight<CAM_MIN_HEIGHT){
			m_fHeight+=CAM_ZOOM_COLLISION_STEP;
			float l_Distance=GetEye().Distance(m_pObject3D->GetPosition());
			SetPlayerPitch(-asin(m_fHeight/l_Distance));
		}

	}





}


void CThirdPersonCamera::RenderDebug(CRenderManager &RM){

	Mat44f mat;
	Vect3f l_ColPos(GetLookAt());

	float l_CollisionDistance=m_CollisionInfo.m_CollisionPoint.Distance(l_ColPos);

	Vect3f l_pos(GetEye()-GetLookAt());
	l_pos=GetLookAt()+l_pos.Normalize()*(8.0f);

	CORE->GetRenderManager()->DrawLine(GetLookAt(),l_pos,colCYAN);





	if(m_CollisionInfo.m_CollisionPoint!=NULL){

		mat.SetIdentity();
		mat.SetPos(m_CollisionInfo.m_CollisionPoint);
		RM.SetTransform(mat);
		RM.DrawSphere(0.1f,(uint32)16,colRED);


		//		mat.SetIdentity();
		//		RM.SetTransform(mat);
		////		CORE->GetRenderManager()->DrawLine(,l_pos,colCYAN);
		//
		//		m_fDistance=abs(sin(GetPlayerPitch()))*l_CollisionDistance;
	}
	mat.SetIdentity();
	mat.SetPos(GetEye());
	RM.SetTransform(mat);
	RM.DrawSphere(0.1f,(uint32)16,colCYAN);



}