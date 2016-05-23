#include "DeferredShadingSceneRendererCommand.h"
#include "RenderManager.h"
#include "TextureManager.h"
#include "DrawQuadRendererCommand.h"
#include "RenderableObjectsTechniqueManager.h"
#include "EffectManager.h"
#include "LightManager.h"
#include "VertexType.h"
#include "SpotLight.h"
#include "Camera\Camera.h"
#include "Color.h"
#include "Process.h"
#include "Base.h"

CDeferredShadingSceneRendererCommand::CDeferredShadingSceneRendererCommand(CXMLTreeNode &atts)
	: CStagedTexturedRendererCommand(atts), m_RenderableObjectTechnique(NULL)
{
	int l_count_textures = atts.GetNumChildren();
	for (int i=0; i<l_count_textures; ++i)
	{
		int l_StageId = atts(i).GetIntProperty("stage_id", 0);
		std::string l_File = atts(i).GetPszProperty("file","");

		CTexture* l_Texture = CORE->GetTextureManager()->GetTexture(l_File);
		if (l_Texture != NULL)
		{
			CStagedTexturedRendererCommand::AddStageTexture(l_StageId, l_Texture);
		}
	}

	m_RenderableObjectTechnique = CORE->GetRenderableObjectsTechniqueManager()->GetResource(CORE->GetRenderableObjectsTechniqueManager()->GetRenderableObjectTechniqueNameByVertexType(SCREEN_TEXTURE_COLORED_VERTEX::GetVertexType()),"");

	uint32 w,h;
	CORE->GetRenderManager()->GetWidthAndHeight(w,h);
	m_Rect.top=m_Rect.left=(long)0.0;
	m_Rect.right=w;
	m_Rect.bottom=h;
}

void CDeferredShadingSceneRendererCommand::Execute(CRenderManager &RM)
{
	if (!CORE->GetProcess()->GetStartGame())
		return;		// evita pintado de luces en GUI

	if (m_RenderableObjectTechnique == NULL)
		return;

	CEffectTechnique* l_EffectTechnique = m_RenderableObjectTechnique->GetEffectTechnique();
	CEffect* l_Effect = l_EffectTechnique->GetEffect();

	if(l_Effect!=NULL)
	{
		LPDIRECT3DDEVICE9 l_Device = RM.GetDevice();
		CLightManager* LM = CORE->GetLightManager();
		const CFrustum& l_Frustum = RM.GetFrustum();
		const  std::vector<CLight*>& l_vLights = LM->GetLights();

		ActivateTextures();

		for(size_t i=0;i<LM->getNumLights();i++)
		{
			CLight* l_Light = l_vLights[i];
			if (l_Light->GetEnabled() && !l_Light->isAmbient())
			{	
				if	(l_Light->isVisible(RM, &l_Frustum))
				{
					l_Effect->SetLight(0, l_Light);	
					l_Light->BeginRenderEffectManagerShadowMap(l_Effect);
					l_Device->SetRenderState(D3DRS_SCISSORTESTENABLE, TRUE);
					RECT rect = DetermineClipRect(l_Light);
					l_Device->SetScissorRect(&rect);
					DrawColoredQuad2DTexturedInPixelsByEffectTechnique(&RM, l_EffectTechnique, m_Rect, colWHITE, NULL);
					l_Device->SetRenderState( D3DRS_SCISSORTESTENABLE, FALSE );
				}
				else if (l_Light->GetCastShadows())
					l_Light->DeleteShadowMap(l_Effect);		// destruimos shadowmap de las luces dinamicas que generan sombras apagadas
			}
		}
	}
}

RECT CDeferredShadingSceneRendererCommand::DetermineClipRect(CLight* Light) 
{
	CEffectManager* l_EM = CORE->GetEffectManager();
	uint32 w,h;
	CORE->GetRenderManager()->GetWidthAndHeight(w,h);
	RECT bbox2D;
	bbox2D.top = 0;
	bbox2D.bottom = h;
	bbox2D.left = 0;
	bbox2D.right = w;

	if (CORE->GetCamera() == NULL)
		return bbox2D;

	if (Light->GetType()==CLight::DIRECTIONAL)
		return bbox2D;

	Vect3f	position(0.0f, 0.0f, 0.0f);
	float range = 0.0f;

	if (Light->GetType()==CLight::OMNI) {
		position = Light->GetPosition();
		range = Light->GetRadius();
	}
	else if (Light->GetType()==CLight::SPOT) {
		CSpotLight* spot = dynamic_cast<CSpotLight*>(Light);
		range = Light->GetRadius();
		position = spot->GetPosition()+spot->GetDirection() * range;
	}

	if (position.SqDistance(CORE->GetCamera()->GetEye()) < range * range * 1.3f)
		return bbox2D;	// si camara dentro del volumen de la luz, no nos complicamos la vida, pintamos full screen quad

	//compute 3D bounding box of light in world space
	D3DXVECTOR3 bbox3D[8];
	bbox3D[0].x = position.x - range; bbox3D[0].y = position.y + range;	bbox3D[0].z = position.z - range;
	bbox3D[1].x = position.x + range; bbox3D[1].y = position.y + range;	bbox3D[1].z = position.z - range;
	bbox3D[2].x = position.x - range; bbox3D[2].y = position.y - range;	bbox3D[2].z = position.z - range;
	bbox3D[3].x = position.x + range; bbox3D[3].y = position.y - range;	bbox3D[3].z = position.z - range;
	bbox3D[4].x = position.x - range; bbox3D[4].y = position.y + range;	bbox3D[4].z = position.z + range;
	bbox3D[5].x = position.x + range; bbox3D[5].y = position.y + range;	bbox3D[5].z = position.z + range;
	bbox3D[6].x = position.x - range; bbox3D[6].y = position.y - range;	bbox3D[6].z = position.z + range;
	bbox3D[7].x = position.x + range; bbox3D[7].y = position.y - range;	bbox3D[7].z = position.z + range;


	//project coordinates
	D3DXMATRIX viewProjMat = l_EM->GetViewProjectionMatrix().GetD3DXMatrix();
	D3DXVECTOR2 projBox[8];
	for(int i = 0; i < 8; ++i) 
	{
		D3DXVECTOR4 projPoint;
		D3DXVec3Transform(&projPoint, &bbox3D[i], &viewProjMat);
		projBox[i].x = projPoint.x / projPoint.w;
		projBox[i].y = projPoint.y / projPoint.w;
		//clip to extents
		if(projBox[i].x < -1.0f)
			projBox[i].x = -1.0f;
		else if(projBox[i].x > 1.0f) 
			projBox[i].x = 1.0f;
		if(projBox[i].y < -1.0f)
			projBox[i].y = -1.0f;
		else if(projBox[i].y > 1.0f) 
			projBox[i].y = 1.0f;
		//go to pixel coordinates
		projBox[i].x = ((projBox[i].x + 1.0f) / 2.0f) * w;
		projBox[i].y = ((-projBox[i].y + 1.0f) / 2.0f) * h;
	}
	
	//compute 2D bounding box of projected coordinates
	unsigned int minX = 0xFFFFFFFF; 
	unsigned int maxX = 0x00000000; 
	unsigned int minY = 0xFFFFFFFF; 
	unsigned int maxY = 0x00000000; 
	for(int i = 0; i < 8; ++i) 
	{
		unsigned int x =static_cast<unsigned int>(projBox[i].x); 
		unsigned int y =static_cast<unsigned int>(projBox[i].y); 
		if(x < minX)minX = x;
		if(x > maxX)maxX = x;
		if(y < minY)minY = y;
		if(y > maxY)maxY = y;
	}

	bbox2D.top = minY;
	bbox2D.bottom = maxY;
	bbox2D.left = minX;
	bbox2D.right = maxX;

	//DEBUG
	/*CColor edgeColor;
	if (Light->GetType()==CLight::OMNI)
		edgeColor.SetArgb(Vect4f(0.8f,0.f,0.f,0.0f));
	else if (Light->GetType()==CLight::SPOT)
		edgeColor.SetArgb(Vect4f(0.f,0.8f,0.f,0.0f));
	
	CORE->GetRenderManager()->DrawQuad2D(Vect2i(minX, minY), maxX-minX, maxY-minY, UPPER_LEFT, edgeColor);*/

	return bbox2D;
}
