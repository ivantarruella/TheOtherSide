#include "FanObject.h"
#include "Renderer\RenderableObjectsLayersManager.h"
#include "LogicObjectsManager.h"
#include "MeshInstance.h"
#include "Logger.h"
#include "Base.h"

CFanObject::CFanObject()
	: CLogicObject(), m_FanMesh1(NULL), m_fSpinVel(0.f), m_fTime(0.f), m_fAngle(0.f)
{
	m_Type = FAN;
}

CFanObject::CFanObject(CXMLTreeNode &atts)
	: CLogicObject(), m_FanMesh1(NULL), m_fSpinVel(0.f), m_fTime(0.f), m_fAngle(0.f)
{
	m_Type = FAN;

	m_fSpinVel = atts.GetFloatProperty("spin_vel", 0.0f);

	std::string l_layer="", l_Renderable_Object="";
	l_layer = atts.GetPszProperty("layer","");
	if (l_layer!="")
	{
		l_Renderable_Object = atts.GetPszProperty("renderable_object","");
		m_FanMesh1=(CInstanceMesh*)CORE->GetRenderableObjectsLayersManager()->GetResource(l_layer)->GetResource(l_Renderable_Object);
		if (m_FanMesh1==NULL)
		{
			std::string msg_error = "CFanObject::CFanObject->No se encuentra renderable object " + l_Renderable_Object + " del ventilador!";
			LOGGER->AddNewLog(ELOG_LEVEL::ELL_ERROR, msg_error.c_str());
		}
	}	
}

CFanObject::~CFanObject()
{
}

void CFanObject::Update(float ElapsedTime)
{
	m_fTime += ElapsedTime;

	if (m_fTime >= 1/60)
	{
		m_fAngle += (e2PIf*m_fSpinVel)/120;
		if (m_fAngle >= e2PIf)
			m_fAngle = 0.0f;

		RotatePhysXMesh(m_FanMesh1, m_fAngle);

		m_fTime = 0.f;
	}
}

void CFanObject::RotatePhysXMesh(CInstanceMesh* mesh, float angle)
{
	if (mesh!=NULL)
	{
		mesh->SetYaw(angle);
		if (mesh->GetCreatePhysics() && mesh->GetPhysicActor() != NULL)
			mesh->GetPhysicActor()->SetRotation(Vect3f(0.f, angle, 0.f));
	}
}

