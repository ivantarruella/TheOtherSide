#include "d3dx9.h"
#include "VertexType.h"
#include "cal3d\model.h"
#include "cal3d\renderer.h"
#include "Texture.h"
#include "cal3d\cal3d.h"
#include "TextureManager.h"
#include "AnimatedInstanceModel.h"
#include "AnimatedModelsManager.h"
#include "StaticMeshManager.h"
#include "Matrix44.h"
#include "IndexedVertexs.h"
#include "EffectManager.h"
#include "RenderableVertexs.h"
#include "Renderer\RenderableObjectsTechniqueManager.h"
#include "Light.h"
#include "Logger.h"
#include "Math\MathUtils.h"
#include "Camera\Camera.h"
#include "Base.h"

CAnimatedInstanceModel::CAnimatedInstanceModel(const std::string &Name)
	: m_CalModel(NULL), m_AnimatedCoreModel(NULL)//, m_pVB(NULL), m_pIB(NULL), m_NumVtxs(0), m_NumFaces(0)
{
	SetName(Name);
}

CAnimatedInstanceModel::CAnimatedInstanceModel(const CXMLTreeNode& XmlData)
	: m_CalModel(NULL), m_AnimatedCoreModel(NULL)//, m_pVB(NULL), m_pIB(NULL), m_NumVtxs(0), m_NumFaces(0)
{
	SetYaw(XmlData.GetFloatProperty("yaw",0.0f));
	SetPitch(XmlData.GetFloatProperty("pitch",0.0f));
	SetRoll(XmlData.GetFloatProperty("roll",0.0f));
	SetPosition(XmlData.GetVect3fProperty("pos", Vect3f(0.0f, 0.0f, 0.0f),true));
	SetName(XmlData.GetPszProperty("name", ""));
	SetCastShadows(XmlData.GetBoolProperty("cast_shadows",false));
	SetScale(XmlData.GetVect3fProperty("scale",Vect3f(1.0f, 1.0f, 1.0f),true));
	SetCreatePhysics(XmlData.GetBoolProperty("generate_physics",false));
	m_PhysicElement=NULL;

}

CAnimatedInstanceModel::CAnimatedInstanceModel(const Vect3f &pos, const std::string &CoreName, const std::string &Name)
	: m_CalModel(NULL), m_AnimatedCoreModel(NULL)//, m_pVB(NULL), m_pIB(NULL), m_NumVtxs(0), m_NumFaces(0)
{
	SetYaw(-3.14159f);
	SetPitch(0.0f);
	SetRoll(0.0f);
	SetPosition(pos);
	SetName(Name);
	SetCastShadows(true);
	SetScale(Vect3f(1.0f, 1.0f, 1.0f));
	SetCreatePhysics(true);
	m_PhysicElement=NULL;

}

CAnimatedInstanceModel::~CAnimatedInstanceModel() 
{ 
	Destroy(); 
	CHECKED_DELETE(m_PhysicElement);
}

void CAnimatedInstanceModel::PositionAnimatedModel(CRenderManager *RM)
{
	Mat44f mat, mat1;
	mat.SetIdentity();
	mat1.SetIdentity();
	
	mat.RotByAngleY(CObject3D::m_fYaw);
	mat.RotByAngleX(CObject3D::m_fPitch);
	mat.RotByAngleZ(CObject3D::m_fRoll);
	mat.Translate(CObject3D::m_Position);
	mat1.Scale(CObject3D::m_Scale.x, CObject3D::m_Scale.y, CObject3D::m_Scale.z);	
}

void CAnimatedInstanceModel::DrawAnimatedModel(CRenderManager *RM, const CFrustum* Frustum, bool forwardRender)
{
	CCamera* l_Cam = CORE->GetCamera();
	if (l_Cam==NULL)
		return;

	Vect3f l_Center = m_AnimatedCoreModel->GetVtxCenter() + GetPosition();
    float l_Radius = m_AnimatedCoreModel->GetRadius();
	float l_Dist = l_Cam->GetEye().Distance(GetPosition());
	if(Frustum->SphereVisible(l_Center, l_Radius) && l_Dist<MAX_DIST_RENDER) {
		PositionAnimatedModel(RM);
		RenderModelByHardware(RM, Frustum, forwardRender);
	}
}

void CAnimatedInstanceModel::DrawAnimatedModelShadow(CRenderManager *RM, const CFrustum* Frustum, bool forwardRender)
{
	Vect3f l_Center = m_AnimatedCoreModel->GetVtxCenter() + GetPosition();
	float l_Radius = m_AnimatedCoreModel->GetRadius();
	
	bool bInLightFrustum = Frustum->SphereVisible(l_Center, l_Radius);
	if(bInLightFrustum) {
		PositionAnimatedModel(RM);
		RenderModelByHardware(RM, Frustum, forwardRender);
	}
}

void CAnimatedInstanceModel::Render(CRenderManager *RM, const CFrustum* Frustum, bool forwardRender)
{
	if (!m_bEnabled || !m_Visible)
		return;		// no pintamos los modelos animados inactivos o ocultos
			
	DrawAnimatedModel(RM, Frustum, forwardRender);
}

void CAnimatedInstanceModel::RenderShadow(CRenderManager *RM, CLight* Light)
{	
	if (!m_bEnabled || !m_Visible || !m_CastShadows)
		return;		// no generamos shadow map de los modelos animados que no generan sombra

	const CFrustum& l_LightFrustum = Light->GetLightFrustum();
	DrawAnimatedModelShadow(RM, &l_LightFrustum, true);
}

void CAnimatedInstanceModel::RenderDebug(CRenderManager *RM)
{	
}

/*
void CAnimatedInstanceModel::RenderModelBySoftware(CRenderManager *RM)
{
	// get the renderer of the model
	CalRenderer *pCalRenderer;
	pCalRenderer = m_CalModel->getRenderer();

	// begin the rendering loop
	if(!pCalRenderer->beginRendering()) return;

	m_NumVtxs=0;
	m_NumFaces=0;

	DWORD dwVBLockFlags=D3DLOCK_NOOVERWRITE;
	DWORD dwIBLockFlags=D3DLOCK_NOOVERWRITE;

	RM->GetDevice()->SetStreamSource( 0, m_pVB, 0,  sizeof(TNORMAL_TEXTURE1_VERTEX) );
	RM->GetDevice()->SetFVF(TNORMAL_TEXTURE1_VERTEX::GetFVF());
	RM->GetDevice()->SetIndices(m_pIB);

	// get the number of meshes
	int meshCount;
	meshCount = pCalRenderer->getMeshCount();

	// render all meshes of the model
	int meshId;
	for(meshId = 0; meshId < meshCount; meshId++)
	{
		// get the number of submeshes
		int submeshCount;
		submeshCount = pCalRenderer->getSubmeshCount(meshId);

		// render all submeshes of the mesh
		int submeshId;
		for(submeshId = 0; submeshId < submeshCount; submeshId++)
		{
			// select mesh and submesh for further data access
			if(pCalRenderer->selectMeshSubmesh(meshId, submeshId))
			{
				if(m_NumVtxs+ pCalRenderer->getVertexCount()>=30000)
				{
					m_NumVtxs=0;
					dwVBLockFlags=D3DLOCK_DISCARD;
				}

				if(m_NumFaces + pCalRenderer->getFaceCount()>=50000)
				{
					m_NumFaces=0;
					dwIBLockFlags=D3DLOCK_DISCARD;

				}


				// Get vertexbuffer from the model		  		  
				TNORMAL_TEXTURE1_VERTEX *pVertices;

				m_pVB->Lock(m_NumVtxs*sizeof(TNORMAL_TEXTURE1_VERTEX), pCalRenderer->getVertexCount()*sizeof(TNORMAL_TEXTURE1_VERTEX), (void**)&pVertices, dwVBLockFlags);

				int vertexCount = pCalRenderer->getVerticesNormalsAndTexCoords(&pVertices->x);
				m_pVB->Unlock();

				CalIndex *meshFaces;

				int faceCount;

				m_pIB->Lock(m_NumFaces* 3*sizeof(CalIndex), pCalRenderer->getFaceCount()*3* sizeof(CalIndex), (void**)&meshFaces,dwIBLockFlags);

				faceCount = pCalRenderer->getFaces(meshFaces);
				m_pIB->Unlock();


				RM->GetDevice()->SetRenderState(D3DRS_CULLMODE,D3DCULL_NONE);
				//RM->GetDevice()->SetTexture(0,(LPDIRECT3DTEXTURE9)pCalRenderer->getMapUserData(0));
				CTexture* l_Text = m_TextureList[meshId];
				if (l_Text!=NULL)
					l_Text->Activate(0);


				RM->GetDevice()->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, m_NumVtxs, 0, vertexCount, m_NumFaces*3, faceCount);

				m_NumVtxs+=vertexCount;
				m_NumFaces+=faceCount;


				dwIBLockFlags=D3DLOCK_NOOVERWRITE;
				dwVBLockFlags=D3DLOCK_NOOVERWRITE;

			}
		}
	}

	// end the rendering
	pCalRenderer->endRendering();
}


bool CAnimatedInstanceModel::LoadVertexBuffer(CRenderManager *RM)
{
  // Create vertex buffer
	if(FAILED(RM->GetDevice()->CreateVertexBuffer(30000*sizeof(TNORMAL_TEXTURE1_VERTEX),
		D3DUSAGE_WRITEONLY|D3DUSAGE_DYNAMIC, TNORMAL_TEXTURE1_VERTEX::GetFVF(), 
	  D3DPOOL_DEFAULT , &m_pVB, NULL
	  )))
	  return false;

  // Create index buffer
  if(sizeof(CalIndex)==2)
  {
	  if(FAILED(
         RM->GetDevice()->CreateIndexBuffer(50000*3*sizeof(CalIndex),
			  D3DUSAGE_WRITEONLY|D3DUSAGE_DYNAMIC,D3DFMT_INDEX16,
			  D3DPOOL_DEFAULT ,&m_pIB, NULL)))
			  return false;
  }
  else
  {
	  if(FAILED(
      RM->GetDevice()->CreateIndexBuffer(50000*3*sizeof(CalIndex),
			  D3DUSAGE_WRITEONLY|D3DUSAGE_DYNAMIC,D3DFMT_INDEX32,
			  D3DPOOL_DEFAULT ,&m_pIB, NULL)))
			  return false;
  }

  return true;
}
*/

void CAnimatedInstanceModel::Update(float ElapsedTime)
{
	m_CalModel->update(ElapsedTime);
}

void CAnimatedInstanceModel::Initialize(CAnimatedCoreModel *AnimatedCoreModel)
{
	m_AnimatedCoreModel = AnimatedCoreModel;
	if (m_AnimatedCoreModel!=NULL && m_AnimatedCoreModel->GetCoreModel()!=NULL)
	{
		InitD3D(CORE->GetRenderManager());	

		m_CalModel = new CalModel(m_AnimatedCoreModel->GetCoreModel());

		// attach all meshes to the model
		for (int i=0; i<m_AnimatedCoreModel->GetCoreModel()->getCoreMeshCount(); ++i)
		{
			m_CalModel->attachMesh(i);	
		}

		// cargamos texturas
		for (size_t i=0; i<m_AnimatedCoreModel->GetNumTextures(); ++i)
		{
			std::string l_TexturePath = m_AnimatedCoreModel->GetCoreModelPath() + m_AnimatedCoreModel->GetTextureName(i);
			CTexture* l_Texture = CORE->GetTextureManager()->GetTexture(l_TexturePath); 
			m_TextureList.push_back(l_Texture);
		}

		// set initial animation state
		ClearAllAnims();

		// guardamos tipos de vertices
		m_VertexTypes.push_back(m_AnimatedCoreModel->GetRenderableVertexs()->GetVertexType());
		m_RenderableObjectTechniqueName = CORE->GetEffectManager()->GetTechniqueEffectNameByVertexDefault(m_VertexTypes[0]);		

		GetRenderableObjectTechnique();
	}
}

void CAnimatedInstanceModel::InitD3D(CRenderManager *RM)
{
	//LoadVertexBuffer(RM);		// usa RenderModelBySoftware
	m_AnimatedCoreModel->LoadVertexBuffer();		// usa RenderModelByHardware
}

void CAnimatedInstanceModel::Destroy()
{
	CHECKED_DELETE(m_CalModel);

	//CHECKED_RELEASE(m_pVB);
	//CHECKED_RELEASE(m_pIB);

	m_TextureList.clear();
	m_NormalTextureList.clear();
}

void CAnimatedInstanceModel::ClearAllAnims()
{
	// set initial animation state
	for (int i=0; i<m_AnimatedCoreModel->GetCoreModel()->getCoreAnimationCount(); ++i)
	{
		BlendCycle(i, 1.0f);
		ClearCycle(i, 0.f);
	}	

	BlendCycle(0, 1.0f);
}

void CAnimatedInstanceModel::ExecuteAction(int Id, float Time, float TimeOut)
{
	m_CalModel->getMixer()->removeAction(Id);
	m_CalModel->getMixer()->executeAction(Id, Time, TimeOut);
}

void CAnimatedInstanceModel::RemoveAction(int Id)
{
	m_CalModel->getMixer()->removeAction(Id);
}

void CAnimatedInstanceModel::BlendCycle(int Id, float Time, float weight)
{
	m_CalModel->getMixer()->blendCycle(Id, weight, Time);
}

void CAnimatedInstanceModel::ClearCycle(int Id, float Time)
{
	m_CalModel->getMixer()->clearCycle(Id, Time);
}

float CAnimatedInstanceModel::GetAnimationDuration()
{
	return m_CalModel->getMixer()->getAnimationDuration();
}

std::vector<CalAnimation *> & CAnimatedInstanceModel::GetAnimationVector()
{
	return m_CalModel->getMixer()->getAnimationVector();
}

void CAnimatedInstanceModel::RenderModelByHardware(CRenderManager *RM, const CFrustum* Frustum, bool forwardRender)
{
	CEffectManager *l_EffectManager=CORE->GetEffectManager();
	
	if (m_RenderableObjectTechniques.size()==0 || m_RenderableObjectTechniques[0]==NULL)
		return;

	CEffectTechnique *l_EffectTechnique = m_RenderableObjectTechniques[0]->GetEffectTechnique();
	if(l_EffectTechnique==NULL)
		return;

	l_EffectManager->SetWorldMatrix(GetTransform());
	CEffect *m_Effect=l_EffectTechnique->GetEffect();
	if(m_Effect==NULL)
		return;

	LPD3DXEFFECT l_Effect=m_Effect->GetD3DEffect();
	if(l_Effect)
	{
		if (forwardRender)		// animated models forward rendering
		{
			l_EffectTechnique->GetEffect()->SetLights(MAX_LIGHTS_BY_SHADER, GetPosition(), Frustum);		
		}

		l_EffectTechnique->BeginRender();
		CalHardwareModel *l_CalHardwareModel=m_AnimatedCoreModel->GetCalHardwareModel();
		D3DXMATRIX transformation[MAXBONES];
		memset(transformation, 0, sizeof(transformation));
		int l_CalHardwareModelCount = l_CalHardwareModel->getHardwareMeshCount();
		for(int hardwareMeshId=0;hardwareMeshId<l_CalHardwareModelCount; hardwareMeshId++)
		{
			l_CalHardwareModel->selectHardwareMesh(hardwareMeshId);
			int l_boneCount = l_CalHardwareModel->getBoneCount();
			for(int boneId = 0; boneId < l_boneCount; boneId++)
			{
				D3DXMatrixRotationQuaternion(&transformation[boneId],(CONST	D3DXQUATERNION*)&l_CalHardwareModel->getRotationBoneSpace(boneId, m_CalModel->getSkeleton()));
				CalVector translationBoneSpace =l_CalHardwareModel->getTranslationBoneSpace(boneId,m_CalModel->getSkeleton());
				transformation[boneId]._14 =translationBoneSpace.x;
				transformation[boneId]._24 =translationBoneSpace.y;
				transformation[boneId]._34 =translationBoneSpace.z;
			}
			float l_Matrix[MAXBONES*3*4];
			memset(l_Matrix, 0, sizeof(l_Matrix));
			for(int b=0;b<l_boneCount;++b)
			{
				memcpy(&l_Matrix[b*3*4], &transformation[b], sizeof(float)*3*4);
			}
			l_Effect->SetFloatArray(m_Effect->GetBonesParameter(),(float *)l_Matrix,(l_CalHardwareModel->getBoneCount())*3*4);
			
			m_TextureList[0]->Activate(0);
			m_TextureList[1]->Activate(1);		// normal map
			
			m_AnimatedCoreModel->GetRenderableVertexs()->Render(RM,l_EffectTechnique, l_CalHardwareModel->getBaseVertexIndex(), 0, l_CalHardwareModel->getVertexCount(), l_CalHardwareModel->getStartIndex(),l_CalHardwareModel->getFaceCount());
		}
	}
}

bool CAnimatedInstanceModel::GetRenderableObjectTechnique()
{
	CRenderableObjectsTechniqueManager *l_ROTM= CORE->GetRenderableObjectsTechniqueManager();
	bool l_Ok=true;
	for(size_t i=0;i<m_VertexTypes.size();++i)
	{
		if(m_RenderableObjectTechniqueName=="")
			m_RenderableObjectTechniqueName=l_ROTM->GetRenderableObjectTechniqueNameByVertexType(m_VertexTypes[i]);

		CRenderableObjectTechnique *l_ROT=l_ROTM->GetResource(m_RenderableObjectTechniqueName,"");	

		m_RenderableObjectTechniques.push_back(l_ROT);	

		/* comentar con Jordi, esta mierda llena el logger!
		if(l_ROT==NULL)
		{
			std::string msg_error = "Error trying to GetRenderableObjectTechnique" +m_RenderableObjectTechniqueName+" on CAnimatedInstanceModel";
			LOGGER->AddNewLog(ELL_WARNING, msg_error.c_str());
		}
		*/
		l_Ok=l_Ok && l_ROT!=NULL;
	}
	return l_Ok;
}

void CAnimatedInstanceModel::RenderReflected(CRenderManager *RM, CRenderableObject* Mesh, const CFrustum* Frustum)
{	
	CCamera* l_Cam = CORE->GetCamera();
	if (l_Cam==NULL)
		return;

	Vect3f l_Center = m_AnimatedCoreModel->GetVtxCenter() + GetPosition();
    float l_Radius = m_AnimatedCoreModel->GetRadius();
    
	float l_Dist = l_Cam->GetEye().SqDistance(GetPosition());
	if(Frustum->SphereVisible(l_Center, l_Radius) && l_Dist<MAX_DIST_RENDER*2)
		RenderModelByHardware(RM, Frustum, true);
}