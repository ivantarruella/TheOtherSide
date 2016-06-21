//----------------------------------------------------------------------------------
// CObject3D class
// Author: Enric Vergara
//
// Description:
// Esta clase será la base de toda instancia que pueda existir en la escena
//----------------------------------------------------------------------------------
#pragma once
#ifndef INC_OBJECT_3D_H_
#define INC_OBJECT_3D_H_

#include "Math\MathTypes.h"
#include "Math\Vector3.h"
#include "Math\Matrix44.h"
#include "include\PhysicActor.h"
#include <string>
class CPhysicActor;
class CPhysicUserData;

class CPhysicElement {
public:
	CPhysicActor * m_PhysicActor;
	CPhysicUserData * m_UserData;
	std::string	m_PhysicsType;

	CPhysicElement(): m_UserData(NULL),m_PhysicActor(NULL),m_PhysicsType("") {}

	CPhysicElement(std::string &name,std::string& physics_type);
	CPhysicElement(const CPhysicElement & other);
	~CPhysicElement();

};

class CObject3D
{
public:
	CObject3D(const Vect3f& pos, const Vect3f& scale, float yaw, float pitch, float roll = 0, bool visible = true, bool cast_shadows=true, const Mat44f& matrix = Mat44f(), bool create_physics=false,std::string physics_type="", std::string user_data="", bool enabled=true);
	CObject3D();
	virtual ~CObject3D(void);

	//---Get Functions
	float						GetYaw			() const {return m_fYaw;}
	float						GetRoll			() const {return m_fRoll;}
	float						GetPitch		() const {return m_fPitch;}
	const Vect3f&				GetPosition		() const {return m_Position;}
	const Vect3f&				GetScale		() const {return m_Scale;}
	bool						GetVisible		() const {return m_Visible;}
	bool						GetEnabled		() const {return m_bEnabled;}
	bool						GetCastShadows	() const {return m_CastShadows;}
	bool						GetCreatePhysics() const {return m_CreatePhysics;}
	Mat44f						GetTransform	();
	Vect3f						GetFront		();
	CPhysicUserData*			GetPhysicUserData() const;
	CPhysicActor*				GetPhysicActor	() const {return m_PhysicElement->m_PhysicActor;}
	CPhysicElement*				GetPhysicElement	() const {return m_PhysicElement;}
	//---Set Functions
	void		SetPosition		( const Vect3f& pos )		{m_Position = pos;}
	void		SetScale		( const Vect3f& scale )		{m_Scale = scale;}
	void		SetYaw			( float yaw )				{m_fYaw = yaw;}
	void		SetPitch		( float pitch )				{m_fPitch = pitch;}
	void		SetRoll			( float roll )				{m_fRoll = roll;}
	void		SetVisible		( bool vis )				{m_Visible = vis;}
	void		SetEnabled		( bool enabled )			{m_bEnabled = enabled;}
	void		SetTransform	(const Mat44f& mat);			
	void		SetCastShadows  ( bool cast_shadows )		{m_CastShadows = cast_shadows;}
	void		SetCreatePhysics  ( bool create_physics )	{m_CreatePhysics = create_physics;}
	void		SetPhysicData(std::string & name,std::string& physics_type)		{m_PhysicElement=new CPhysicElement(name,physics_type);}
	void		SetPhysicElement	(CPhysicElement* PhysicElement) { m_PhysicElement=PhysicElement;}
	
	bool		CreateMeshPhysics(const std::vector<Vect3f>& vertices, const std::vector<uint32>& faces);
	bool		CreatePhysics(Vect3f& vSize=(Vect3f)NULL, float radius=0.0f, float height=0.0f, uint32 CollisionGroup=0);
	
	Vect3f		xzFromAngle( float radians );
protected:
	Mat44f		m_Matrix;
	Vect3f		m_Position;
	Vect3f		m_Scale;
	float		m_fYaw;
	float		m_fPitch;
	float		m_fRoll;
	bool		m_Visible;
	bool        m_CastShadows;
	bool		m_CreatePhysics;
	
	CPhysicElement* m_PhysicElement;
	bool		m_bEnabled;
};

#endif //INC_CORE_H_