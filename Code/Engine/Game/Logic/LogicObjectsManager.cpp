#include "LogicObjectsManager.h"
#include "xml/XMLTreeNode.h"
#include "MeshInstance.h"
#include "MirrorObject.h"
#include "SpawnPointObject.h"
#include "TutorialObject.h"
#include "DoorObject.h"
#include "FanObject.h"
#include "ElevatorObject.h"
#include "Lasers.h"
#include "LifeRecover.h"
#include "EnergyRecover.h"
#include "MirrorPieceObject.h"
#include "BrokenMirrorObject.h"
#include "Logger.h"
#include "Exception.h"
#include "Base.h"


CLogicObjectsManager::CLogicObjectsManager()
	: m_fTime(0.0f)
{
}


CLogicObjectsManager::~CLogicObjectsManager()
{
	Destroy();
}

void CLogicObjectsManager::Update(float ElapsedTime)
{
	for (size_t i=0; i < m_ResourcesVector.size(); ++i)
	{
		m_ResourcesVector[i]->Update(ElapsedTime);
	}
}

bool CLogicObjectsManager::Load (const std::string &FileName)
{
	CXMLTreeNode parser;
	if (!parser.LoadFile(FileName.c_str()))
	{
		std::string msg_error = "CLogicObjectsManager::Load->Error al intentar leer el archivo: " + FileName;
		LOGGER->AddNewLog(ELOG_LEVEL::ELL_ERROR, msg_error.c_str());
		throw CException(__FILE__, __LINE__, msg_error);
	}
	m_FileName = FileName;

	// Obtenemos el nodo "Logic_Objects"
	CXMLTreeNode  l_LogicObjectsNode = parser["Logic_Objects"];
	if (l_LogicObjectsNode.Exists())
	{
		// Obtenemos los datos para todos los objetos logicos
		int l_count_meshes = l_LogicObjectsNode.GetNumChildren();
		for (int i = 0; i < l_count_meshes; ++i)
		{
			if (!l_LogicObjectsNode(i).IsComment())
			{
				CLogicObject* l_LogicObject = NULL;
				
				// Obtenemos tipo de logica aplicada al objeto 
				std::string l_type = l_LogicObjectsNode(i).GetPszProperty("logic_type","");
				std::string l_name = l_LogicObjectsNode(i).GetPszProperty("name","");

				if (l_type=="Mirror") 
				{
					l_LogicObject = new CMirrorObject(l_LogicObjectsNode(i));
				}
				else if (l_type=="Spawn Point")
				{
					l_LogicObject = new CSpawnPointObject(l_LogicObjectsNode(i));
				}
				else if (l_type=="Tutorial")
				{
					l_LogicObject = new CTutorialObject(l_LogicObjectsNode(i));
				}				
				else if (l_type=="Door")
				{
					l_LogicObject = new CDoorObject(l_LogicObjectsNode(i));
				}	
				else if (l_type=="Elevator")
				{
					l_LogicObject = new CElevatorObject(l_LogicObjectsNode(i));
				}	
				else if (l_type=="Fan")
				{
					l_LogicObject = new CFanObject(l_LogicObjectsNode(i));
				}
				else if (l_type=="Lasers")
				{
					l_LogicObject = new CLasers();
				}
				else if (l_type=="Life")
				{
					l_LogicObject = new CLifeRecover(l_LogicObjectsNode(i));
				}
				else if (l_type=="Energy")
				{
					l_LogicObject = new CEnergyRecover(l_LogicObjectsNode(i));
				}
				else if (l_type=="Mirror piece")
				{
					l_LogicObject = new CMirrorPieceObject(l_LogicObjectsNode(i));
				}
				else if (l_type=="Broken mirror")
				{
					l_LogicObject = new CBrokenMirrorObject(l_LogicObjectsNode(i));
				}

				if (l_LogicObject != NULL)
				{
					AddResource(l_name, l_LogicObject);

					std::string msg_info = "CLogicObjectsManager::Load()-> Logic object " + l_name + " tipo " + l_type + " cargado ok!\n";
					LOGGER->AddNewLog(ELOG_LEVEL::ELL_INFORMATION, msg_info.c_str());
				}
				else
				{
					std::string msg_info = "CLogicObjectsManager::Load()-> Tipo de logica " + l_type + " desconocida en objeto " + l_name + "!\n";
					LOGGER->AddNewLog(ELOG_LEVEL::ELL_INFORMATION, msg_info.c_str());
				}
			}
		}
	}
	return true;
}

bool CLogicObjectsManager::Reload ()
{
	if (m_FileName != "")
	{
		Destroy();
		return Load (m_FileName);
	}

	return false;
}

void CLogicObjectsManager::SetObjectEnabled(const std::string& Name, bool state)
{
	for (size_t i=0; i<m_ResourcesVector.size(); ++i)
	{
		CLogicObject* l_Obj = m_ResourcesVector[i];
		if (l_Obj->GetName()==Name)
		{
			l_Obj->SetEnabled(state);
			return;
		}
	}
}

void CLogicObjectsManager::SetAllObjectsEnabled(CLogicObject::TLogicType logic, bool state)
{
	for (size_t i=0; i<m_ResourcesVector.size(); ++i)
	{
		CLogicObject* l_Obj = m_ResourcesVector[i];
		if (l_Obj->GetType()==logic)
		{
			l_Obj->SetEnabled(state);
		}
	}
}

void CLogicObjectsManager::SetMirrorRenderActive(const std::string& Name, bool state)
{
	for (size_t i=0; i<m_ResourcesVector.size(); ++i)
	{
		CLogicObject* l_Obj = m_ResourcesVector[i];
		if (l_Obj->GetType()==CLogicObject::MIRROR && l_Obj->GetName()==Name)
		{
			(((CMirrorObject*)l_Obj)->SetMirrorActive(state));
			return;
		}
	}
}

bool CLogicObjectsManager::GetMirrorRenderActive(const std::string& Name)
{
	for (size_t i=0; i<m_ResourcesVector.size(); ++i)
	{
		CLogicObject* l_Obj = m_ResourcesVector[i];
		if (l_Obj->GetType()==CLogicObject::MIRROR && l_Obj->GetName()==Name)
		{
			return (((CMirrorObject*)l_Obj)->GetMirrorActive());
		}
	}
	
	return false;
}

void CLogicObjectsManager::SetMirrorON(const std::string& Name, bool on)
{
	for (size_t i=0; i<m_ResourcesVector.size(); ++i)
	{
		CLogicObject* l_Obj = m_ResourcesVector[i];
		if (l_Obj->GetType()==CLogicObject::MIRROR && l_Obj->GetName()==Name)
		{
			(((CMirrorObject*)l_Obj)->SetMirrorON(on));
			(((CMirrorObject*)l_Obj)->GetRenderableObject()->SetVisible(on));
			return;
		}
	}
}

void CLogicObjectsManager::CloseDoor(const std::string& Name)
{
	for (size_t i=0; i<m_ResourcesVector.size(); ++i)
	{
		CLogicObject* l_Obj = m_ResourcesVector[i];
		if (l_Obj->GetType()==CLogicObject::DOOR)
			if (l_Obj->GetName()==Name)
			{
				if (((CDoorObject*)l_Obj)->IsOpen())
					((CDoorObject*)l_Obj)->SetMustClose(true);
				return;
			}
	}
}

bool CLogicObjectsManager::GetTutorialActive()
{
	for (size_t i=0; i<m_ResourcesVector.size(); ++i)
	{
		CLogicObject* l_Obj = m_ResourcesVector[i];
		if (l_Obj->GetType()==CLogicObject::TUTORIAL)
		{
			if (((CTutorialObject*)l_Obj)->IsActive())
			{
				return true;	// tutorial activo
			}
		}
		else if (l_Obj->GetType()==CLogicObject::MIRROR_PIECE)
		{
			if (((CMirrorPieceObject*)l_Obj)->IsActive())
			{
				return true;	// ventana trozo espejo conseguido activa
			}
		}
		else if (l_Obj->GetType()==CLogicObject::BROKEN_MIRROR)
		{
			if (((CBrokenMirrorObject*)l_Obj)->IsActive())
			{
				return true;	// ventana te faltan trozos de espejo activa
			}
		}
	}

	return false;
}

void CLogicObjectsManager::GenerateMirrors()
{
	for (size_t i=0; i<m_ResourcesVector.size(); ++i)
	{
		CLogicObject* l_Obj = m_ResourcesVector[i];
		if (l_Obj->GetType()==CLogicObject::MIRROR)
		{
			((CMirrorObject *)l_Obj)->Generate();
		}
	}
}

void CLogicObjectsManager::RestartAllObjects()
{
	for (size_t i=0; i<m_ResourcesVector.size(); ++i)
	{
		CLogicObject* l_Obj = m_ResourcesVector[i];
		l_Obj->Restart();
	}
}