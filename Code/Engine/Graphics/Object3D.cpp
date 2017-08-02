//#include "__PCH_Graphics.h"
#include "Object3D.h"
#include "include\PhysicsManager.h"
#include "include\PhysicActor.h"
#include "include\PhysicCookingMesh.h"
#include "Base.h"

CObject3D::CObject3D(const Vect3f& pos, const Vect3f& scale, float yaw, float pitch, float roll, bool visible, bool cast_shadows, const Mat44f& matrix, bool create_physics,std::string physics_type,std::string user_data, bool enabled)
	: m_Position(pos)
	, m_Scale(scale)
	, m_fYaw(yaw)
	, m_fPitch(pitch)
	, m_fRoll(roll)
	, m_Visible(visible)
	, m_CastShadows(cast_shadows)
	, m_Matrix(matrix)
	, m_CreatePhysics(create_physics)
	, m_PhysicElement(NULL)
	, m_bEnabled(enabled)

{
	if(create_physics==true)
		m_PhysicElement=new CPhysicElement(user_data,physics_type);

}

CObject3D::CObject3D()
	: m_Position( Vect3f(0.f,0.f,0.f))
	, m_Scale( Vect3f(0.f,0.f,0.f))
	, m_fYaw(0.f)
	, m_fPitch(0.f)
	, m_fRoll(0.f)
	, m_Visible(true)
	, m_CastShadows(true)
	, m_Matrix(Mat44f())
	, m_CreatePhysics(false)
	, m_PhysicElement(NULL)
	, m_bEnabled(true)
{
}



CObject3D::~CObject3D(void) {
	
	if (m_PhysicElement!=NULL)
		CORE->GetPhysicsManager()->ReleasePhysicActor(m_PhysicElement->m_PhysicActor);

	CHECKED_DELETE(m_PhysicElement);
}


Mat44f CObject3D::GetTransform()
{
	Mat44f mat, mat1, mat2;
	mat.SetIdentity();
	mat1.SetIdentity();
	mat2.SetIdentity();

	mat.SetFromPos(m_Position);
	mat1.SetFromPitchRollYaw(Vect3f(m_fPitch, m_fYaw, m_fRoll));
	mat2.SetFromScale(m_Scale.x, m_Scale.y, m_Scale.z);

	m_Matrix = mat * mat1 * mat2;
	return (mat * mat1 * mat2);
}


void CObject3D::SetTransform(const Mat44f& mat)
{
	m_Matrix = mat;
}

Vect3f CObject3D::GetFront( ) 
{
	Vect3f front;
	front = xzFromAngle( m_fYaw );
	front.Normalize();
	return front;
}

CPhysicUserData* CObject3D::GetPhysicUserData() const 
{
	if(m_PhysicElement!=NULL)
		return m_PhysicElement->m_UserData;

	return NULL;
}

Vect3f CObject3D::xzFromAngle( float radians ) 
{
	Vect3f l_aux;
	l_aux.x = -sin( radians );
	l_aux.y = float( 0.0f );
	l_aux.z = cos( radians );
	return l_aux;
}

bool CObject3D::CreateMeshPhysics(const std::vector<Vect3f>& vertices, const std::vector<uint32>& faces, uint32 group, float mass){
	if (!m_CreatePhysics) 
		return false;

	if(m_PhysicElement->m_PhysicsType!="triangle_mesh") 
		return false;

	CPhysicActor* l_Actor=GetPhysicActor();
	CPhysicUserData* l_UserData=GetPhysicUserData();
	if(l_UserData) {
		//l_Actor->CreateBody(9.8f);
		l_UserData->SetPaint(true);
	} 
	else 
		return false;

	CORE->GetPhysicsManager()->GetCookingMesh()->CreatePhysicMesh(vertices,faces,l_UserData->GetName());
	NxTriangleMesh* l_TriangleMesh= CORE->GetPhysicsManager()->GetCookingMesh()->GetPhysicMesh(l_UserData->GetName());
	l_Actor->AddMeshShape(l_TriangleMesh,m_Position,v3fZERO,0,group);

	return CORE->GetPhysicsManager()->AddPhysicActor(l_Actor, mass);
}


inline bool isPositive( float Number ) {
	return (Number>0.0f)?true:false;
}


bool CObject3D::CreatePhysics(Vect3f& vSize, float radius, float height, uint32 CollisionGroup) {
	if(!m_CreatePhysics) 
		return false;
	
	CPhysicActor* l_Actor=GetPhysicActor();
	CPhysicUserData* l_UserData=GetPhysicUserData();
	if(l_UserData) {
		//l_Actor->CreateBody(9.8f);
		l_UserData->SetPaint(true);
	} 
	else 
		return false;

	if(m_PhysicElement->m_PhysicsType=="bounding_box") {
		if(vSize!=NULL)
			l_Actor->AddBoxSphape(vSize, m_Position, v3fZERO);
	}
	else if(m_PhysicElement->m_PhysicsType=="bounding_sphere") {
		if(isPositive(radius))
			l_Actor->AddSphereShape(radius, m_Position);
	}
	else if(m_PhysicElement->m_PhysicsType=="capsule") {
		if(isPositive(radius)&&isPositive(height))
			l_Actor->AddCapsuleShape(radius, height, m_Position, v3fZERO, 0, CollisionGroup);
	} 
	else if(m_PhysicElement->m_PhysicsType=="convex_hull") {
		//TODO!
	} 
	else  {
		m_CreatePhysics=false;
		return false;
	}

	return CORE->GetPhysicsManager()->AddPhysicActor(m_PhysicElement->m_PhysicActor);

}

/* CPHYSICELEMENT CLASS */

CPhysicElement::CPhysicElement(std::string &name,std::string & physics_type): m_PhysicsType(physics_type){
	m_UserData= new CPhysicUserData(name);
	m_PhysicActor=new CPhysicActor(m_UserData);
	m_UserData->SetActor(m_PhysicActor);
}

CPhysicElement::CPhysicElement(const CPhysicElement & other){
	std::string str = other.m_UserData->GetName();
	m_UserData= new CPhysicUserData(str);
	if(other.m_PhysicActor){
		m_PhysicActor=new CPhysicActor(m_UserData);
		m_UserData->SetActor(m_PhysicActor);
	}
	m_PhysicsType=other.m_PhysicsType;

}

CPhysicElement::~CPhysicElement(){

	

	CHECKED_DELETE(m_UserData);
	CHECKED_DELETE(m_PhysicActor);
}

