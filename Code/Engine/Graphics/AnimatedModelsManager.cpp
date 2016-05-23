#include "AnimatedModelsManager.h"
#include "xml/XMLTreeNode.h"
#include "Logger.h"
#include "Exception.h"
#include "Monster.h"
#include "Soldier.h"
#include "Player.h"
#include "ElevatorAnimatedPhysicModel.h"
#include "EnemyManager.h"
#include "Core.h"

#include "Base.h"

CAnimatedModelsManager::CAnimatedModelsManager()
{
}


CAnimatedModelsManager::~CAnimatedModelsManager()
{
	Destroy();
}

CAnimatedCoreModel * CAnimatedModelsManager::GetCore(const std::string &Name, const std::string &Path)
{
	CAnimatedCoreModel* l_pAnimatedCoreModel = CMapManager<CAnimatedCoreModel>::GetResource(Name);
	if(l_pAnimatedCoreModel==NULL)
	{
		l_pAnimatedCoreModel = new CAnimatedCoreModel();
		l_pAnimatedCoreModel->Load(Path);
		
		AddResource(Name, l_pAnimatedCoreModel);
	}

	return l_pAnimatedCoreModel;
}

CAnimatedInstanceModel * CAnimatedModelsManager::CreateInstance(const std::string &Name)
{
	CAnimatedCoreModel* l_pCoreModel = CMapManager<CAnimatedCoreModel>::GetResource(Name);
	if(l_pCoreModel!=NULL)
	{
		CAnimatedInstanceModel* l_pAnimatedInstanceModel = new CAnimatedInstanceModel(Name);
		l_pAnimatedInstanceModel->Initialize(l_pCoreModel);

		return l_pAnimatedInstanceModel;
	}
	return NULL;	
}

CAnimatedInstanceModel * CAnimatedModelsManager::CreateInstance(const CXMLTreeNode& XmlData)
{
	std::string l_Name = XmlData.GetPszProperty("core","");
	CAnimatedCoreModel* l_pCoreModel = CMapManager<CAnimatedCoreModel>::GetResource(l_Name);
	CAnimatedInstanceModel* l_pAnimatedInstanceModel = NULL;
  	if(l_pCoreModel!=NULL)
	{
		if(l_Name=="Monster")
		{			
			CMonster* l_Monster = new CMonster(XmlData);
			l_pAnimatedInstanceModel=l_Monster;
			std::string l_name = l_pAnimatedInstanceModel->GetName();
			CORE->GetEnemyManager()->AddMonster(l_name, l_Monster);
		}
		else if(l_Name=="Soldier")
		{
			CSoldier* l_Soldier = new CSoldier(XmlData);
			l_pAnimatedInstanceModel=l_Soldier;
			std::string l_name = l_pAnimatedInstanceModel->GetName();
			CORE->GetEnemyManager()->AddSoldier(l_name, l_Soldier);
		}
		else if(l_Name=="Player")
		{
			l_pAnimatedInstanceModel=new CPlayer(XmlData);
		}
		else if(l_Name=="Elevator")
		{
			CElevatorAnimatedPhysicModel* l_Elevator = new CElevatorAnimatedPhysicModel(XmlData, l_pCoreModel);
			l_pAnimatedInstanceModel=l_Elevator;
		}
		else
		{
			std::string msg_error = "CAnimatedModelsManager::Load->No se encuentra el modelo animado: " + l_Name;
			LOGGER->AddNewLog(ELL_WARNING, msg_error.c_str());
		}
	}

	return l_pAnimatedInstanceModel;	
}

CAnimatedInstanceModel * CAnimatedModelsManager::CreateInstance(const std::string &CoreName, const std::string &Name, const Vect3f &pos)
{
	std::string l_Name = CoreName;
	CAnimatedCoreModel* l_pCoreModel = CMapManager<CAnimatedCoreModel>::GetResource(l_Name);
	CAnimatedInstanceModel* l_pAnimatedInstanceModel = NULL;
  	if(l_pCoreModel!=NULL)
	{
		if(l_Name=="Monster")
		{			
			CMonster* l_Monster = new CMonster(pos, CoreName, Name);
			l_pAnimatedInstanceModel=l_Monster;
			std::string l_name = l_pAnimatedInstanceModel->GetName();
			CORE->GetEnemyManager()->AddMonster(l_name, l_Monster);
		}
		else if(l_Name=="Soldier")
		{
			CSoldier* l_Soldier = new CSoldier(pos, CoreName, Name);
			l_pAnimatedInstanceModel=l_Soldier;
			std::string l_name = l_pAnimatedInstanceModel->GetName();
			CORE->GetEnemyManager()->AddSoldier(l_name, l_Soldier);
		}
		else
		{
			std::string msg_error = "CAnimatedModelsManager::Load->No se encuentra el modelo animado: " + l_Name;
			LOGGER->AddNewLog(ELL_WARNING, msg_error.c_str());
		}
	}

	return l_pAnimatedInstanceModel;	
}

bool CAnimatedModelsManager::Load(const std::string& FileName)
{
	CXMLTreeNode parser;
	if (!parser.LoadFile(FileName.c_str()))
	{
		std::string msg_error = "CAnimatedModelsManager::Load->Error al intentar leer el archivo: " + FileName;
		LOGGER->AddNewLog(ELL_ERROR, msg_error.c_str());
		throw CException(__FILE__, __LINE__, msg_error);
	}
	m_FileName=FileName;
	// Obtenemos el nodo "Animated_Models"
	CXMLTreeNode  l_AnimatedModelsNode = parser["Animated_Models"];
	if (l_AnimatedModelsNode.Exists())
	{
		int l_count_animatedmodels = l_AnimatedModelsNode.GetNumChildren();
		for (int i = 0; i < l_count_animatedmodels; ++i)
		{
			if (!l_AnimatedModelsNode(i).IsComment())
			{
				// Obtenemos los datos de cada animated model
				const char* l_name = l_AnimatedModelsNode(i).GetPszProperty("name","");
				const char* l_path= l_AnimatedModelsNode(i).GetPszProperty("path","");
				GetCore(l_name, l_path);
			}
		}
	}
	
	return true;
}

void CAnimatedModelsManager::Reload()
{
	if (m_FileName != "")
	{
		Destroy();
		Load (m_FileName);
	}
}