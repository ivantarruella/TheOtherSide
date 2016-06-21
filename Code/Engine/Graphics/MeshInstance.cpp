#include "RenderManager.h"
#include "Matrix44.h"
#include "MeshInstance.h"
#include "StaticMeshManager.h"
#include "Camera\Frustum.h"
#include "Light.h"
#include "Logger.h"
#include "Base.h"

CInstanceMesh::CInstanceMesh(const std::string &Name, const std::string &CoreName)
	: m_StaticMesh(NULL)
{
	m_StaticMesh = CORE->GetStaticMeshManager()->GetResource(CoreName);
	
}

CInstanceMesh::CInstanceMesh(const CXMLTreeNode &XmlData)
	: m_StaticMesh(NULL)
{
	std::string l_Core=XmlData.GetPszProperty("core","");
	std::string l_Name=XmlData.GetPszProperty("name","");
	CObject3D::m_fYaw=XmlData.GetFloatProperty("yaw",0.0f);
	CObject3D::m_fPitch=XmlData.GetFloatProperty("pitch",0.0f);
	CObject3D::m_fRoll = XmlData.GetFloatProperty("roll", 0.0f);
	CObject3D::m_Position=XmlData.GetVect3fProperty("pos", Vect3f(0.0f, 0.0f, 0.0f),true);
	CObject3D::m_Visible=XmlData.GetBoolProperty("visible",false);
	CObject3D::m_CastShadows=XmlData.GetBoolProperty("cast_shadows",false);
	float scale_x = XmlData.GetFloatProperty("scale_x", 1.0f);
	float scale_y = XmlData.GetFloatProperty("scale_y", 1.0f);
	float scale_z = XmlData.GetFloatProperty("scale_z", 1.0f);
	CObject3D::m_Scale = Vect3f (scale_x, scale_y, scale_z); 
	CObject3D::m_CreatePhysics = XmlData.GetBoolProperty("generate_physics",false);
	CObject3D::m_PhysicElement=NULL;
		
	m_StaticMesh = CORE->GetStaticMeshManager()->GetResource(l_Core);

	if (m_StaticMesh != NULL)
	{
		if(m_CreatePhysics)
		{
			std::string l_Type = XmlData.GetPszProperty("physics_type","");
			m_PhysicElement= new CPhysicElement(l_Core,l_Type);
			
			if(l_Type=="triangle_mesh")
				CreateMeshPhysics(m_StaticMesh->getVB(),m_StaticMesh->getIB());
			else {
				Vect3f size = m_StaticMesh->getStaticMeshMaxPoint() - m_StaticMesh->getStaticMeshMinPoint();
				float rad = m_StaticMesh->getStaticMeshBSRadius();
				CreatePhysics(size/2.0f, rad);
			}
		}
	}
}


CInstanceMesh::~CInstanceMesh()
{
}

void CInstanceMesh::PositionMesh(CRenderManager *RM)
{
	Mat44f mat, mat2;
	mat.SetIdentity();
	mat2.SetIdentity();

	//Update posicion objeto fisico	
	if(m_CreatePhysics)
		m_Position= m_PhysicElement->m_PhysicActor->GetPosition();

	mat.RotByAngleY(CObject3D::m_fYaw);
	mat.RotByAngleX(CObject3D::m_fPitch);
	mat.RotByAngleZ(CObject3D::m_fRoll);
	mat.Translate(CObject3D::m_Position);
	mat2.Scale(CObject3D::m_Scale.x, CObject3D::m_Scale.y, CObject3D::m_Scale.z);
	
	RM->SetTransform(mat * mat2);
}

void CInstanceMesh::DrawMesh(CRenderManager *RM, const CFrustum* Frustum, bool forwardRender)
{
	PositionMesh(RM);
	m_StaticMesh->Render(RM, m_Position, Frustum, forwardRender);
}

void CInstanceMesh::DrawMeshShadow(CRenderManager *RM, const CFrustum* Frustum, CLight* Light)
{
	PositionMesh(RM);
	m_StaticMesh->RenderShadow(RM, m_Position, Frustum, Light);
}

void CInstanceMesh::Render(CRenderManager *RM, const CFrustum* Frustum, bool forwardRender)
{
	//const CFrustum& l_Frustum = RM->GetFrustum();			
	Vect3f l_Center = m_StaticMesh->getStaticMeshCenter() +  GetPosition();
	float l_Radius=m_StaticMesh->getStaticMeshBSRadius();
	
	if(Frustum->SphereVisible(l_Center, l_Radius))
		DrawMesh(RM, Frustum, forwardRender);
}

void CInstanceMesh::RenderShadow(CRenderManager *RM, CLight* Light)
{
	if (!m_CastShadows)
		return;		// no generamos shadow map de los objetos que no generan sombra

	Vect3f l_Center = m_StaticMesh->getStaticMeshCenter() +  GetPosition();
	float l_Radius=m_StaticMesh->getStaticMeshBSRadius();
	
	const CFrustum& l_Frustum = Light->GetLightFrustum();
	bool visibleInLightFrustum = l_Frustum.SphereVisible(l_Center, l_Radius);
	if(visibleInLightFrustum)
		DrawMeshShadow(RM, &l_Frustum, Light);
}


void CInstanceMesh::RenderDebug(CRenderManager *RM)
{
}

void CInstanceMesh::RenderReflected(CRenderManager *RM, CRenderableObject* Mesh, const CFrustum* Frustum)
{
	if (this == Mesh)
		return;	// no reflejeamos el propio espejo
	
	Vect3f l_Center = m_StaticMesh->getStaticMeshCenter() +  GetPosition();
	float l_Radius=m_StaticMesh->getStaticMeshBSRadius();
	
	if(Frustum->SphereVisible(l_Center, l_Radius))
		DrawMesh(RM, Frustum, true);
}

