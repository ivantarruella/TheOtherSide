#include "BillboardManager.h"
#include "xml/XMLTreeNode.h"
#include "Logger.h"
#include "Exception.h"
#include "RenderManager.h"
#include "Utils/MapManager.h"
#include <map>
#include <iterator>
#include "Base.h"
CBillboardManager::CBillboardManager(void): m_Path("")
{
}



CBillboardManager::~CBillboardManager(void)
{
	Destroy();
}

void CBillboardManager::Destroy(){


	for(uint16 i = 0; i<this->size();i++){

		CHECKED_DELETE(CBillboardManager::at(i));
	}
	this->clear();
	m_BillboardCores.Destroy();
	m_BillboardAnimationCores.Destroy();
}

void CBillboardManager::Reload()
{
	if (m_Path!="")
	{
		Destroy();
		Load(m_Path);
	}
}

void CBillboardManager::Load(const std::string &FileName)
{

	CXMLTreeNode parser;
	if (!parser.LoadFile(FileName.c_str()))
	{
		std::string msg_error = "CBillboardManager::Load->Error al intentar leer el archivo: " + FileName;
		LOGGER->AddNewLog(ELL_ERROR, msg_error.c_str());
		throw CException(__FILE__, __LINE__, msg_error);
	}


	m_Path = FileName;
	int l_count_objects;

	// Obtenemos el nodo "Billboards"
	CXMLTreeNode  l_BillboardNode = parser["Billboards"];
	if (l_BillboardNode.Exists())
	{
		std::string l_Name;
		CBillboard* l_Billboard = NULL;
		if (l_BillboardNode.Exists())
		{
			l_count_objects = l_BillboardNode.GetNumChildren();
			for (int i = 0; i < l_count_objects; ++i){
				//Animation Node
				l_Billboard=new CBillboard(l_BillboardNode(i));
				l_Name=l_BillboardNode(i).GetPszProperty("name","");
				m_BillboardCores.AddResource(l_Name,l_Billboard);
			}
		}


	}


	// Obtenemos el nodo "BillboardAnimation"
	CXMLTreeNode  l_BillboardAnimationNode = parser["BillboardAnimation"];
	if (l_BillboardAnimationNode.Exists())
	{
		std::string l_Name;
		CBillboardAnimation* l_BillboardAnimation;
		CXMLTreeNode  l_NodeBillboard = parser["Billboard"];
		if (l_NodeBillboard.Exists()){

			l_count_objects = l_NodeBillboard.GetNumChildren();
			for (int i = 0; i < l_count_objects; ++i){
				//Animation Node
				l_BillboardAnimation=new CBillboardAnimation(l_NodeBillboard(i));
				l_Name=l_NodeBillboard(i).GetPszProperty("name","");
				m_BillboardAnimationCores.AddResource(l_Name,l_BillboardAnimation);
			}
			CXMLTreeNode l_NodeInstances = parser["Instances"];
			if (l_NodeInstances.Exists()){

				l_count_objects = l_NodeInstances.GetNumChildren();
				for (int i = 0; i < l_count_objects; ++i){
					//Instance Node
					l_Name=l_NodeInstances(i).GetPszProperty("id","");
					l_BillboardAnimation=m_BillboardAnimationCores.GetResource(l_Name);
					CBillboardAnimation* l_Copy=new CBillboardAnimation(*l_BillboardAnimation);
					Vect3f l_Pos=l_NodeInstances(i).GetVect3fProperty("pos",Vect3f(0.0f));
					l_Copy->SetPos(l_Pos);
					this->push_back(l_Copy);

				}

			}


		}
	}

}


void CBillboardManager::Render(CRenderManager * RM, const CFrustum* Frustum)
{
	//PreRender
	//Activa el alpha
	LPDIRECT3DDEVICE9 l_Device=RM->GetDevice();
	l_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	l_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	l_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	l_Device->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	//End PreRender

	for(uint16 i = 0; i<this->size();i++){

		this->at(i)->Render(RM, Frustum);
	}


	//PostRender
	//Desactiva el alpha
	l_Device->SetRenderState(D3DRS_POINTSPRITEENABLE, FALSE);
	l_Device->SetRenderState(D3DRS_POINTSCALEENABLE, FALSE);
	l_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	l_Device->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	//end PostRender
}

void CBillboardManager::Update(float ElapsedTime)
{
	for(uint16 i = 0; i<this->size();i++){

		this->at(i)->Update(ElapsedTime);
	}


}

CBillboard* CBillboardManager::GetBillboardCore(std::string BillboardName)
{
	return m_BillboardCores.GetResource(BillboardName);

}