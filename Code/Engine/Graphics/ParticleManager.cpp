#include "ParticleManager.h"
#include "PointEmitter.h"
#include "LineEmitter.h"
#include "PlaneEmitter.h"
#include "BoxEmitter.h"
#include "CircleEmitter.h"
#include "SphereEmitter.h"
#include "TorusEmitter.h"
#include "XML\XMLTreeNode.h"
#include "Exception.h"
#include "Random.h"
#include "RenderManager.h"
#include "Random.h"
#include "Time.h"

CParticleManager::CParticleManager(void): m_Path(""), m_World(1)
{
}


CParticleManager::~CParticleManager(void)
{
	this->Destroy();
}


void CParticleManager::Destroy(){
	CMapManager<CParticleEmitter>::Destroy();
	m_ParticleCores.Destroy();
}

void CParticleManager::Reload()
{
	if (m_Path!="")
	{
		Destroy();
		Load(m_Path);
	}
}

void CParticleManager::Load(const std::string &FileName)
{
	CRandom l_Random=CRandom();
	CXMLTreeNode parser;
	if (!parser.LoadFile(FileName.c_str()))
	{
		std::string msg_error = "CParticleManager::Load->Error al intentar leer el archivo: " + FileName;
		LOGGER->AddNewLog(ELL_ERROR, msg_error.c_str());
		throw CException(__FILE__, __LINE__, msg_error);
	}


	m_Path = FileName;

	int l_count_objects;
	// Obtenemos el nodo "BillboardAnimation"
	CXMLTreeNode  l_ParticleManager = parser["ParticleManager"];
	if (l_ParticleManager.Exists())
	{
		std::string l_Name;
		std::string l_Type;
		CParticleEmitter* l_ParticleEmitter;
		CXMLTreeNode  l_ParticleManagerNode = parser["ParticleEmitters"];
		if (l_ParticleManagerNode.Exists()){

			l_count_objects = l_ParticleManagerNode.GetNumChildren();
			for (int i = 0; i < l_count_objects; ++i){
				//Particle Node
				
				l_Type = l_ParticleManagerNode(i).GetPszProperty("type","");
				if(l_Type == "point")
				{
					l_ParticleEmitter = new CPointEmitter(l_ParticleManagerNode(i));
				} 
				if(l_Type == "line")
				{						
					l_ParticleEmitter = new CLineEmitter(l_ParticleManagerNode(i));
				}	
				if(l_Type == "plane")
				{						
					l_ParticleEmitter = new CPlaneEmitter(l_ParticleManagerNode(i));
				}	
				if(l_Type == "circle")
				{						
					l_ParticleEmitter = new CCircleEmitter(l_ParticleManagerNode(i));
				}	
				if(l_Type == "torus")
				{						
					l_ParticleEmitter = new CTorusEmitter(l_ParticleManagerNode(i));
				}	
				if(l_Type == "sphere")
				{						
					l_ParticleEmitter = new CSphereEmitter(l_ParticleManagerNode(i));
				}	
				if(l_Type == "box")
				{						
					l_ParticleEmitter = new CBoxEmitter(l_ParticleManagerNode(i));
				}
				
				l_ParticleEmitter->SetType(l_Type);
				l_Name=l_ParticleManagerNode(i).GetPszProperty("name","");
				m_ParticleCores.AddResource(l_Name,l_ParticleEmitter);
			}
			CXMLTreeNode l_NodeInstances = parser["Instances"];
			if (l_NodeInstances.Exists()){

				l_count_objects = l_NodeInstances.GetNumChildren();
				for (int i = 0; i < l_count_objects; ++i){
					//Instance Node
					CParticleEmitter* l_Copy = NULL; 
					std::string l_Id = l_NodeInstances(i).GetPszProperty("id","");
					l_ParticleEmitter=m_ParticleCores.GetResource(l_Id);
					
					if(l_ParticleEmitter->GetType() =="point")
					{
						l_Copy=new CPointEmitter(*l_ParticleEmitter);
						Vect3f l_Pos1=l_NodeInstances(i).GetVect3fProperty("pos1",Vect3f(0.0f));
						Vect3f l_Pos2=l_NodeInstances(i).GetVect3fProperty("pos2",Vect3f(0.0f));
						l_Copy->SetPos(l_Random.getRandVect3f(l_Pos1,l_Pos2));
					}
					if(l_ParticleEmitter->GetType() =="line")
					{
						l_Copy=new CLineEmitter(*l_ParticleEmitter);
						Vect3f l_Pos1=l_NodeInstances(i).GetVect3fProperty("pos1",Vect3f(0.0f));
						Vect3f l_Pos2=l_NodeInstances(i).GetVect3fProperty("pos2",Vect3f(0.0f));
						l_Copy->SetPos(l_Pos1);
						((CLineEmitter*)l_Copy)->SetEnd(l_Pos2);
					}
					if(l_ParticleEmitter->GetType() =="plane")
					{
						l_Copy=new CPlaneEmitter(*l_ParticleEmitter);
						Vect3f l_Pos1=l_NodeInstances(i).GetVect3fProperty("pos1",Vect3f(0.0f));
						Vect3f l_Pos2=l_NodeInstances(i).GetVect3fProperty("pos2",Vect3f(0.0f));
						float size = l_NodeInstances(i).GetFloatProperty("size", 0.0f);
						l_Copy->SetPos(l_Random.getRandVect3f(l_Pos1,l_Pos2));
						((CPlaneEmitter*)l_Copy)->SetSize(size);
					}
					if(l_ParticleEmitter->GetType() =="circle")
					{
						l_Copy=new CCircleEmitter(*l_ParticleEmitter);
						Vect3f l_Pos1=l_NodeInstances(i).GetVect3fProperty("pos1",Vect3f(0.0f));
						Vect3f l_Pos2=l_NodeInstances(i).GetVect3fProperty("pos2",Vect3f(0.0f));
						float radius = l_NodeInstances(i).GetFloatProperty("size", 0.0f);
						l_Copy->SetPos(l_Random.getRandVect3f(l_Pos1,l_Pos2));
						((CCircleEmitter*)l_Copy)->SetRadius(radius);
					}
					if(l_ParticleEmitter->GetType() =="torus")
					{
						l_Copy=new CTorusEmitter(*l_ParticleEmitter);
						Vect3f l_Pos1=l_NodeInstances(i).GetVect3fProperty("pos1",Vect3f(0.0f));
						Vect3f l_Pos2=l_NodeInstances(i).GetVect3fProperty("pos2",Vect3f(0.0f));
						float radius = l_NodeInstances(i).GetFloatProperty("size", 0.0f);
						l_Copy->SetPos(l_Random.getRandVect3f(l_Pos1,l_Pos2));
						((CTorusEmitter*)l_Copy)->SetRadius(radius);
					}
					if(l_ParticleEmitter->GetType() =="sphere")
					{
						l_Copy=new CSphereEmitter(*l_ParticleEmitter);
						Vect3f l_Pos1=l_NodeInstances(i).GetVect3fProperty("pos1",Vect3f(0.0f));
						Vect3f l_Pos2=l_NodeInstances(i).GetVect3fProperty("pos2",Vect3f(0.0f));
						float radius = l_NodeInstances(i).GetFloatProperty("size", 0.0f);
						l_Copy->SetPos(l_Random.getRandVect3f(l_Pos1,l_Pos2));
						((CSphereEmitter*)l_Copy)->SetRadius(radius);
					}
					if(l_ParticleEmitter->GetType() =="box")
					{
						l_Copy=new CBoxEmitter(*l_ParticleEmitter);
						Vect3f l_Pos1=l_NodeInstances(i).GetVect3fProperty("pos1",Vect3f(0.0f));
						Vect3f l_Pos2=l_NodeInstances(i).GetVect3fProperty("pos2",Vect3f(0.0f));
						float size = l_NodeInstances(i).GetFloatProperty("size", 0.0f);
						l_Copy->SetPos(l_Random.getRandVect3f(l_Pos1,l_Pos2));
						((CBoxEmitter*)l_Copy)->SetSize(size);
					}

					if (l_Copy != NULL)
					{
						l_Name = l_NodeInstances(i).GetPszProperty("name","");
						int l_World = l_NodeInstances(i).GetIntProperty("world",0);
						l_Copy->SetName(l_Name);
						l_Copy->SetWorld(l_World);
						//this->push_back(l_Copy);
						this->AddResource(l_Name, l_Copy);
					}
				}
			}
		}
	}
}

void CParticleManager::AddParticleEmitterInstance(const std::string& core_id, const std::string& instance_name, const Vect3f& Pos1, const Vect3f& Pos2, float size, int World)
{
	CParticleEmitter* l_Copy = NULL;
	CParticleEmitter* l_ParticleEmitterCore = m_ParticleCores.GetResource(core_id);
					
	if(l_ParticleEmitterCore->GetType() =="point")
	{
		l_Copy=new CPointEmitter(*l_ParticleEmitterCore);
		l_Copy->SetPos(CRandom().getRandVect3f(Pos1,Pos2));
	}
	if(l_ParticleEmitterCore->GetType() =="line")
	{
		l_Copy=new CLineEmitter(*l_ParticleEmitterCore);
		l_Copy->SetPos(Pos1);
		((CLineEmitter*)l_Copy)->SetEnd(Pos2);
	}
	if(l_ParticleEmitterCore->GetType() =="plane")
	{
		l_Copy=new CPlaneEmitter(*l_ParticleEmitterCore);
		l_Copy->SetPos(CRandom().getRandVect3f(Pos1,Pos2));
		((CPlaneEmitter*)l_Copy)->SetSize(size);
	}
	if(l_ParticleEmitterCore->GetType() =="circle")
	{
		l_Copy=new CCircleEmitter(*l_ParticleEmitterCore);
		l_Copy->SetPos(CRandom().getRandVect3f(Pos1,Pos2));
		((CCircleEmitter*)l_Copy)->SetRadius(size);
	}
	if(l_ParticleEmitterCore->GetType() =="torus")
	{
		l_Copy=new CTorusEmitter(*l_ParticleEmitterCore);
		l_Copy->SetPos(CRandom().getRandVect3f(Pos1,Pos2));
		((CTorusEmitter*)l_Copy)->SetRadius(size);
	}
	if(l_ParticleEmitterCore->GetType() =="sphere")
	{
		l_Copy=new CSphereEmitter(*l_ParticleEmitterCore);
		l_Copy->SetPos(CRandom().getRandVect3f(Pos1,Pos2));
		((CSphereEmitter*)l_Copy)->SetRadius(size);
	}
	if(l_ParticleEmitterCore->GetType() =="box")
	{
		l_Copy=new CBoxEmitter(*l_ParticleEmitterCore);
		l_Copy->SetPos(CRandom().getRandVect3f(Pos1,Pos2));
		((CBoxEmitter*)l_Copy)->SetSize(size);
	}

	if (l_Copy != NULL)
	{
		l_Copy->SetName(instance_name);
		l_Copy->SetWorld(World);
		
		CMapManager<CParticleEmitter>::AddResource(instance_name, l_Copy);
	}
}

void CParticleManager::RemoveParticleEmitterInstance(const std::string& instance_name)
{
	CMapManager<CParticleEmitter>::DestroyResource(instance_name);
}

CParticleEmitter* CParticleManager::GetParticleEmitter(const std::string& name)
{
	CParticleEmitter* l_Emitter = this->GetResource(name);
	/*
	for(uint16 i = 0; i<m_Resources.size();i++)
	{
		if (GetResource() == name && l_Emitter == NULL)
		{
			l_Emitter = ((CParticleEmitter*)m_ResourcesVector.at(i));
		}
	}
	*/
	return l_Emitter;
}

void CParticleManager::Render(CRenderManager * RM, const CFrustum* Frustum)
{
	LPDIRECT3DDEVICE9 l_Device=RM->GetDevice();

	//PreRender
	//Activa el alpha
	l_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	l_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	l_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
	l_Device->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	//End PreRender

	for(uint16 i = 0; i<m_Resources.size();i++)
		((CParticleEmitter*)GetResource(i))->Render(RM, m_World, Frustum);
	
	//PostRender
	//Desactiva el alpha
	//l_Device->SetRenderState(D3DRS_POINTSPRITEENABLE, FALSE);
	//l_Device->SetRenderState(D3DRS_POINTSCALEENABLE, FALSE);
	l_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	l_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_SRCCOLOR);
	l_Device->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	//end PostRender
}

void CParticleManager::Update(float ElapsedTime)
{
	for(uint16 i = 0; i<m_Resources.size();i++){

		((CParticleEmitter*)GetResource(i))->Update(ElapsedTime, m_World);
	}
	

}

void CParticleManager::SetWorld(int world)
{
	m_World = world;
}