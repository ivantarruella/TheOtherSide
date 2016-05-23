#include "ElevatorAnimatedPhysicModel.h"
#include "StaticMeshManager.h"
#include "XML\XMLTreeNode.h"

// PhysX
#include "include\PhysicsManager.h"
#include "include\PhysicActor.h"
#include "include\PhysicCookingMesh.h"

#include "Base.h"

CElevatorAnimatedPhysicModel::CElevatorAnimatedPhysicModel()
{
}

CElevatorAnimatedPhysicModel::CElevatorAnimatedPhysicModel(const CXMLTreeNode& XmlData, CAnimatedCoreModel* coreModel)
	: CAnimatedInstanceModel(XmlData)
{
	Initialize(coreModel);

	std::string l_Name = XmlData.GetPszProperty("name","");
	std::string l_Core = XmlData.GetPszProperty("core","");
	std::string l_Type = XmlData.GetPszProperty("physics_type","");

	CStaticMesh* l_StaticMesh = CORE->GetStaticMeshManager()->GetResource(l_Name);

	if (l_StaticMesh!=NULL)
	{
		m_PhysicElement= new CPhysicElement(l_Core,l_Type);
		m_PhysicElement->m_UserData->SetPaint(true);
		m_PhysicElement->m_UserData->SetColor(colBLUE);

		if (l_Type=="triangle_mesh")
		{
			m_PhysicElement->m_PhysicActor->CreateBody(100.0f);
				
			CORE->GetPhysicsManager()->GetCookingMesh()->CreatePhysicMesh(l_StaticMesh->getVB(),l_StaticMesh->getIB(),l_Name);

			NxTriangleMesh* l_TriangleMesh= CORE->GetPhysicsManager()->GetCookingMesh()->GetPhysicMesh(l_Name);
			m_PhysicElement->m_PhysicActor->AddMeshShape(l_TriangleMesh,m_Position);
			CORE->GetPhysicsManager()->AddPhysicActor(m_PhysicElement->m_PhysicActor);
			m_PhysicElement->m_PhysicActor->SetKinematic(true);
		}
	}
}

CElevatorAnimatedPhysicModel::~CElevatorAnimatedPhysicModel()
{
	if (m_PhysicElement!=NULL)
		CORE->GetPhysicsManager()->ReleasePhysicActor(m_PhysicElement->m_PhysicActor);
	
	CHECKED_DELETE(m_PhysicElement);
}
