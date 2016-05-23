#include "ThirdPersonCamera.h"
#include "include\PhysicsManager.h"
#include "RenderManager.h"
#include "Math\MathUtils.h"
#include "Player.h"
#include "Base.h"



CThirdPersonCamera::CThirdPersonCamera(float zn, float zf, float fov, float aspect, CObject3D* pObject3D, float Zoom, float Distance, float Height)
	: CThPSCamera(zn, zf, fov, aspect, pObject3D, Zoom), 
	m_fDistance(Distance), m_fDistanceFromCenter(PLAYER_DISTANCE_FROM_CENTER), m_fHeight(Height), m_CollisionInfo(SCollisionInfo())
{
	//Hacemos una copia del Object3D del player para
	//que cuando la camara rote no lo haga el player tambien
	m_pObject3D=new CObject3D(*pObject3D);
	CPhysicElement* l_Elem;



	//Creamos bounding sphere para la camara
	std::string l_Name="Camera", l_Type="bounding_sphere";
	l_Elem= new CPhysicElement(l_Name,l_Type);


	m_pObject3D->SetPhysicElement(l_Elem);
	//m_pObject3D->CreatePhysics(GetEye(),0.2f);


}

CThirdPersonCamera::~CThirdPersonCamera()
{
	//Elimina la copia que se hizo al object3d del player al crear la camara
	CHECKED_DELETE(m_pObject3D);

}


Vect3f CThirdPersonCamera::GetLookAt () const
{
	float l_fYaw = m_pObject3D->GetYaw();
	Vect3f l_Position(m_fDistance*(m_fDistanceFromCenter-0.05f)* sin(l_fYaw), CAM_LOOKAT_HEIGHT, -m_fDistance*(m_fDistanceFromCenter-0.05f) * cos(l_fYaw));
	assert(m_pObject3D);
	return (m_pObject3D->GetPosition() + l_Position);

}


Vect3f CThirdPersonCamera::GetEye() const
{
	return (GetLookAt() - GetEyePoint());
}

Vect3f CThirdPersonCamera::GetEyePoint() const {



	assert(m_pObject3D);
	float yaw		= m_pObject3D->GetYaw();
	float pitch	= m_pObject3D->GetPitch();

	//float l_Distance=m_fDistance/cos(-pitch);
	//Pasamos de coordenadas esfericas a coordenadas cartesianas
	return Vect3f(	m_fDistance * cos(yaw) * cos(pitch), 
		m_fDistance * sin(pitch)+m_fHeight,
		m_fDistance * sin(yaw) * cos(pitch));


}

Vect3f CThirdPersonCamera::GetCollisionPoint ()
{
	Vect3f l_vec = Vect3f();
	Vect3f l_OriginPosition = GetEye();
	Vect3f l_Direction(GetLookAt() - GetEye());
	l_Direction=l_Direction.Normalize();

	CPhysicUserData * l_UserData=CORE->GetPhysicsManager()->RaycastClosestActor(l_OriginPosition,l_Direction ,5<<0, m_CollisionInfo );
	if(l_UserData)
		return m_CollisionInfo.m_CollisionPoint;
	else{

		return NULL;

	}

}


bool CThirdPersonCamera::CheckCollision()
{
	Vect3f l_OriginPosition = m_pObject3D->GetPosition() + Vect3f(0.0f,1.0f,0.0f)*CAM_LOOKAT_HEIGHT;
	Vect3f l_Direction( GetEye()- l_OriginPosition);
	l_Direction=l_Direction.Normalize();

	CPhysicUserData * l_UserData=CORE->GetPhysicsManager()->RaycastClosestActor(l_OriginPosition,l_Direction ,1<<0, m_CollisionInfo );

	if(l_UserData==NULL && m_CollisionInfo.m_CollisionPoint==NULL) 
		return false;
	else 
	{
		Vect3f l_Eye = GetEye();
		float l_CollisionDistance=m_CollisionInfo.m_fDistance;
		float l_CameraDistance=GetEye().Distance(l_OriginPosition);

		if(l_CollisionDistance>l_CameraDistance)
			return false;
		else 
			return true;
	}
}

bool CThirdPersonCamera::CheckNextCollision()
{
	float l_Distance=m_fDistance;
	m_fDistance=CAM_DIST;

	Vect3f l_OriginPosition = m_pObject3D->GetPosition() + Vect3f(0.0f,1.0f,0.0f)*CAM_LOOKAT_HEIGHT;
	Vect3f l_Direction( GetEye()- l_OriginPosition);
	l_Direction=l_Direction.Normalize();

	CPhysicUserData * l_UserData=CORE->GetPhysicsManager()->RaycastClosestActor(l_OriginPosition,l_Direction ,1<<0, m_CollisionInfo );

	if(l_UserData==NULL && m_CollisionInfo.m_CollisionPoint==NULL) 
		return false;
	else {

		float l_CollisionDistance=m_CollisionInfo.m_fDistance;
		float l_CameraDistance=GetEye().Distance(l_OriginPosition);
		m_fDistance=l_Distance;

		if(l_CollisionDistance>l_CameraDistance)
			return false;
		else return true;
	}
}


void CThirdPersonCamera::Update(float ElapsedTime){

	m_pObject3D->GetPhysicActor()->SetGlobalPosition(GetEye());

	Vect3f l_LookAt=GetLookAt();
	Vect3f l_PlayerPosition =  m_pObject3D->GetPosition() + Vect3f(0.0f,1.0f,0.0f)*CAM_LOOKAT_HEIGHT;
	float l_DistanceBeforeCollision=GetEye().Distance(l_PlayerPosition);

	//Volvemos a la posicion inicial si no colisiona
	if(!CheckCollision())
	{
		float l_Pitch=GetPlayerPitch();
		if(m_fDistance<CAM_DIST){
			//Comprobamos que con el nuevo movimiento no colisionara
			if(!CheckNextCollision()){

				m_fDistance+=CAM_ZOOM_COLLISION_STEP;
			}

		}
		//Bajamos la camara a la posicion original
		if(m_fDistance>=CAM_MIN_DIST && m_fHeight<CAM_MIN_HEIGHT){
			m_fHeight+=CAM_ZOOM_COLLISION_STEP;
			float l_Distance=GetEye().Distance(l_PlayerPosition);
			l_Pitch=asin(m_fHeight/l_Distance);

		}
		SetPlayerPitch(l_Pitch);
	}

	//Si hay colision
	else{

		float l_CollisionDistance=m_CollisionInfo.m_fDistance;
		m_fDistance=abs(cos(GetPlayerPitch()))*l_CollisionDistance;
		if(m_fDistance>=COLLISION_EPSILON)
			m_fDistance-=COLLISION_EPSILON;

		if(m_fDistance>=CAM_MIN_DIST && m_fHeight<CAM_MIN_HEIGHT){
			m_fHeight+=CAM_ZOOM_COLLISION_STEP;
			float l_Distance=GetEye().Distance(l_PlayerPosition);
			SetPlayerPitch(asin(m_fHeight/l_Distance));
		}

	}

}


void CThirdPersonCamera::RenderDebug(CRenderManager &RM){

	Mat44f mat;
	Vect3f l_PlayerPosition =  m_pObject3D->GetPosition() + Vect3f(0.0f,1.0f,0.0f)*CAM_LOOKAT_HEIGHT;

	Vect3f l_pos(GetEye()-GetLookAt());
	l_pos=l_PlayerPosition+l_pos.Normalize()*(8.0f);

	CORE->GetRenderManager()->DrawLine(l_PlayerPosition,l_pos,colCYAN);





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

	mat.SetIdentity();
	mat.SetPos(l_PlayerPosition);
	RM.SetTransform(mat);
	RM.DrawSphere(0.1f,(uint32)16,colCYAN);


}