#include "AnimatedCoreModel.h"
#include "xml/XMLTreeNode.h"
#include "Logger.h"
#include "Exception.h"
#include "cal3d\error.h"
#include "VertexType.h"
#include "RenderableVertexs.h"
#include "IndexedVertexs.h"
#include "StaticMesh.h"
#include "EffectManager.h"
#include "Base.h"


CAnimatedCoreModel::CAnimatedCoreModel()
	: m_CalCoreModel(NULL), m_CalHardwareModel(NULL), m_RenderableVertexs(NULL), m_Name(""), m_SkeletonFilename(""), m_Path("")
{
	CalLoader::setLoadingMode(LOADER_ROTATE_X_AXIS);
	//Technique por defecto para modelos animados
	std::string name=CORE->GetEffectManager()->GetTechniqueEffectNameByVertexDefault(CAL3D_HW_VERTEX::GetVertexType());
	SetDeffaultEffectTechnique(CORE->GetEffectManager()->GetEffectTechnique(name));
}

CAnimatedCoreModel::~CAnimatedCoreModel() 
{ 
	Release(); 
}

void CAnimatedCoreModel::Release()
{
	CHECKED_DELETE(m_CalCoreModel);
	CHECKED_DELETE(m_RenderableVertexs);
	CHECKED_DELETE(m_CalHardwareModel);

	m_vMeshFilename.clear();
	m_TextureFilenameList.clear();
}

void CAnimatedCoreModel::Load(const std::string &Path)
{
	CXMLTreeNode parser;
	if (!parser.LoadFile((Path+"\\actor.xml").c_str()))
	{
		std::string msg_error = "CAnimatedCoreModel::Load->Error al intentar leer el archivo: " + Path;
		LOGGER->AddNewLog(ELL_ERROR, msg_error.c_str());
		throw CException(__FILE__, __LINE__, msg_error);
	}
	m_Path = Path;

	// Obtenemos el nodo "Animated_Model"
	CXMLTreeNode  l_AnimatedModelNode = parser["animated_model"];
	m_Name = l_AnimatedModelNode.GetPszProperty("name","");

	if (l_AnimatedModelNode.Exists())
	{
		// Creamos instancia de CalCoreModel
		m_CalCoreModel = new CalCoreModel(m_Name);

		int l_count_model = l_AnimatedModelNode.GetNumChildren();
		for (int i = 0; i < l_count_model; ++i)
		{
			if (!l_AnimatedModelNode(i).IsComment())
			{
				std::string l_NodeName = l_AnimatedModelNode(i).GetName();

				if (strcmp("skeleton", l_NodeName.c_str())==0) 
				{
					m_SkeletonFilename = l_AnimatedModelNode(i).GetPszProperty("filename",""); 
					LoadSkeleton();

					// Calculate animated model bounding box
					m_CalCoreModel->getCoreSkeleton()->calculateBoundingBoxes(m_CalCoreModel);

				}
				else if (strcmp("mesh", l_NodeName.c_str())==0) 
				{
					std::string l_MeshName = l_AnimatedModelNode(i).GetPszProperty("filename",""); 
					m_vMeshFilename.push_back(l_MeshName);
					LoadMesh(l_MeshName);
				} 
				else if (strcmp("animation", l_NodeName.c_str())==0) 
				{
					std::string l_AnimationName = l_AnimatedModelNode(i).GetPszProperty("name","");
					std::string l_AnimationFileName = l_AnimatedModelNode(i).GetPszProperty("filename","");
					LoadAnimation(l_AnimationName, l_AnimationFileName);
				}
				else if (strcmp("texture", l_NodeName.c_str())==0) 
				{
					std::string l_TextureFileName = l_AnimatedModelNode(i).GetPszProperty("filename","");
					m_TextureFilenameList.push_back(l_TextureFileName);
				}
			}
		}
		
		CalcAnimatedCoreModelBB();
	}
}


bool CAnimatedCoreModel::LoadMesh(const std::string &MeshFileName)
{
	if(m_CalCoreModel->loadCoreMesh(m_Path+MeshFileName) == -1)
	{
		LOGGER->AddNewLog(ELL_WARNING, "CAnimatedCoreModel::LoadMesh()-> Error: \"%s\"", CalError::getLastErrorText().c_str());
		return false;
	}
	return true;
}

bool CAnimatedCoreModel::LoadSkeleton()
{
	if(!m_CalCoreModel->loadCoreSkeleton(m_Path+m_SkeletonFilename))
	{
		LOGGER->AddNewLog(ELL_WARNING, "CAnimatedCoreModel::LoadSkeleton()-> Error: \"%s\"", CalError::getLastErrorText().c_str());
		return false;
	}

	return true;
}

bool CAnimatedCoreModel::LoadAnimation(const std::string &Name, const std::string &Filename)
{
	if(m_CalCoreModel->loadCoreAnimation(m_Path+Filename, Name) == -1)
	{
		LOGGER->AddNewLog(ELL_WARNING, "CAnimatedCoreModel::LoadAnimation()-> Error: \"%s\"", CalError::getLastErrorText().c_str());
		return false;
	}

	return true;
}

const std::string & CAnimatedCoreModel::GetTextureName( size_t id )
{
	return m_TextureFilenameList.at(id);
}


bool CAnimatedCoreModel::LoadVertexBuffer()
{
	int l_NumVtxs=0;
	int l_NumFaces=0;

	int l_MeshCount = m_CalCoreModel->getCoreMeshCount();
	for(int i = 0; i < l_MeshCount; ++i)
	{
		CalCoreMesh* l_CoreMesh = m_CalCoreModel->getCoreMesh(i);
		for(int j = 0; j < l_CoreMesh->getCoreSubmeshCount(); ++j)
		{
			l_NumVtxs += l_CoreMesh->getCoreSubmesh(j)->getVertexCount();
			l_NumFaces += l_CoreMesh->getCoreSubmesh(j)->getFaceCount();
		}
	}

	CHECKED_DELETE(m_CalHardwareModel);
	m_CalHardwareModel = new CalHardwareModel(m_CalCoreModel);

	//Cogemos el doble de vértices necesarios porque al crear el model de hardware
	//puede necesitar más vértices que el modelo por software
	CAL3D_HW_VERTEX *l_Vtxs = new CAL3D_HW_VERTEX[l_NumVtxs*2]; 
	CalIndex *l_Idxs = new CalIndex[l_NumFaces*3];
	
	m_CalHardwareModel->setVertexBuffer((char*) l_Vtxs,sizeof(CAL3D_HW_VERTEX));
	m_CalHardwareModel->setWeightBuffer(((char*)l_Vtxs) + 12,sizeof(CAL3D_HW_VERTEX));
	m_CalHardwareModel->setMatrixIndexBuffer(((char*)l_Vtxs) + 28,sizeof(CAL3D_HW_VERTEX));
	m_CalHardwareModel->setNormalBuffer(((char*)l_Vtxs)+44,sizeof(CAL3D_HW_VERTEX));
	m_CalHardwareModel->setTextureCoordNum(1);
	m_CalHardwareModel->setTextureCoordBuffer(0,((char*)l_Vtxs)+92,sizeof(CAL3D_HW_VERTEX));
	m_CalHardwareModel->setIndexBuffer(l_Idxs);
	
	if (m_CalHardwareModel->load( 0, 0, MAXBONES)==NULL) 
		return false;
	
	l_NumVtxs = m_CalHardwareModel->getTotalVertexCount();
	l_NumFaces = m_CalHardwareModel->getTotalFaceCount();
	CalcTangentsAndBinormals(l_Vtxs, (unsigned short *)l_Idxs, l_NumVtxs, l_NumFaces*3,sizeof(CAL3D_HW_VERTEX),0, 44, 60, 76, 92);		

	CHECKED_DELETE(m_RenderableVertexs);
	m_RenderableVertexs=new CIndexedVertexs<CAL3D_HW_VERTEX>(CORE->GetRenderManager(), l_Vtxs, l_Idxs,l_NumVtxs, l_NumFaces*3);
	
	delete []l_Vtxs;
	delete []l_Idxs;
	
	return true;
}

void CAnimatedCoreModel::CalcAnimatedCoreModelBB()
{
	Vect3f l_VtxMin, l_VtxMax;

	for(int i = 0; i < m_CalCoreModel->getCoreMeshCount(); ++i)
	{
		CalCoreMesh* l_Mesh = m_CalCoreModel->getCoreMesh(i);
		for(int j = 0; j < 1/*l_Mesh->getCoreSubmeshCount()*/; ++j)
		{
			CalCoreSubmesh* l_SubMesh = l_Mesh->getCoreSubmesh(j);

			if (l_SubMesh!=NULL)
			{
				const std::vector<CalCoreSubmesh::Vertex> l_VertexVector  = l_SubMesh->getVectorVertex();
				std::vector<CalCoreSubmesh::Vertex>::const_iterator cit  = l_VertexVector.cbegin();
				std::vector<CalCoreSubmesh::Vertex>::const_iterator cit_end = l_VertexVector.cend();
      
				l_VtxMin.x = cit->position.x;
				l_VtxMin.y = cit->position.y;
				l_VtxMin.z = cit->position.z;
				l_VtxMax = l_VtxMin;

				for(; cit != cit_end; ++cit)
				{
					l_VtxMin.x = (l_VtxMin.x < cit->position.x) ? l_VtxMin.x : cit->position.x;
					l_VtxMin.y = (l_VtxMin.y < cit->position.y) ? l_VtxMin.y : cit->position.y;
					l_VtxMin.z = (l_VtxMin.z < cit->position.z) ? l_VtxMin.z : cit->position.z;
          
					l_VtxMax.x = (l_VtxMax.x > cit->position.x) ? l_VtxMax.x : cit->position.x;
					l_VtxMax.y = (l_VtxMax.y > cit->position.y) ? l_VtxMax.y : cit->position.y;
					l_VtxMax.z = (l_VtxMax.z > cit->position.z) ? l_VtxMax.z : cit->position.z;
				}
			}
			else
			{
				std::string msg_error = "CAnimatedCoreModel::CalcAnimatedCoreModelBB->SubMesh no encontrada para " + l_Mesh->getName() + "!\n";
				LOGGER->AddNewLog(ELL_ERROR, msg_error.c_str());
			}
		}
	}

	m_VtxMin.x = l_VtxMin.x;
	m_VtxMin.y = l_VtxMin.y;
	m_VtxMin.z = l_VtxMin.z;

	m_VtxMax.x = l_VtxMax.x;
	m_VtxMax.y = l_VtxMax.y;
	m_VtxMax.z = l_VtxMax.z;
	
	m_VtxCenter = Vect3f((m_VtxMin.x + l_VtxMax.x)/2,(m_VtxMin.y + l_VtxMax.y)/2,(m_VtxMin.z + l_VtxMax.z)/2);
	m_fRadius = m_VtxCenter.SqDistance(l_VtxMax);
	m_VtxBBSize = Vect3f(abs(m_VtxMax.x-m_VtxMin.x),abs(m_VtxMax.y-m_VtxMin.y),abs(m_VtxMax.z-m_VtxMin.z));
}


const Vect3f& CAnimatedCoreModel::GetBoxSize(){

	return m_VtxBBSize;

}