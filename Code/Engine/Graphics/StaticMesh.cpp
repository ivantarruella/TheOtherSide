#include "Texture.h"
#include "IndexedVertexs.h"
#include "TextureManager.h"
#include "StaticMesh.h"
#include "VertexType.h"
#include "Core.h"
#include "RenderManager.h"
#include "Matrix44.h"
#include "EffectManager.h"
#include "Renderer\RenderableObjectsTechniqueManager.h"
#include "LightManager.h"
#include "Logger.h"
#include "Exception.h"
#include "Base.h"

#define STATIC_MESH_FILE_HEADER 0xcaca
#define STATIC_MESH_FILE_FOOTER 0xacac

bool CStaticMesh::Load (const std::string &FileName)
{
	FILE *l_File = NULL;
	fopen_s(&l_File, FileName.c_str(),"rb");
	m_FileName=FileName;
	if(l_File !=NULL )
	{
		// Leemos el header del fichero
		uint16 l_FileHeader; 
		fread(&l_FileHeader,sizeof(uint16),1,l_File);               
		if(l_FileHeader != STATIC_MESH_FILE_HEADER)
		{
			fclose(l_File);
			return false;
		}

		// Leemos número de RenderableVertexs que contiene el fichero
		uint16 l_NumRenderableVertexs;
		fread(&l_NumRenderableVertexs,sizeof(uint16),1,l_File);               
		m_Textures.resize(l_NumRenderableVertexs);
		uint16 l_OffsetVtxPhysic=0;

		for(size_t RV=0;RV<l_NumRenderableVertexs;++RV)
		{
			// Leemos tipo de vertices que contiene el fichero
			uint16 l_VertexType;
			fread(&l_VertexType,sizeof(uint16),1,l_File);               

			// Leemos materiales 
			int l_NumMaterials=-1;
			
			if (l_VertexType==TNORMAL_COLORED_VERTEX::GetVertexType())
				l_NumMaterials=0;	// solo geometria, normal i difuso (sin materiales)
			else if (l_VertexType==TNORMAL_TEXTURE1_VERTEX::GetVertexType())
				l_NumMaterials=1;
			else if (l_VertexType==TNORMAL_TEXTURE1_TEXTURE2_VERTEX::GetVertexType())
				l_NumMaterials=2;
			else if (l_VertexType==TNORMAL_TANGENT_BINORMAL_TEXTURED_VERTEX::GetVertexType() )
				l_NumMaterials=2;
			else if (l_VertexType==TNORMAL_TANGENT_BINORMAL_TEXTURE1_TEXTURE2_VERTEX::GetVertexType() )
				l_NumMaterials=3;
			else if (l_VertexType==TNORMAL_TANGENT_BINORMAL_TEXTURE1_TEXTURE2_VERTEX::GetVertexType() )
				l_NumMaterials=3;		
			else if (l_VertexType==TNORMAL_TANGENT_BINORMAL_TEXTURE1_TEXTURE2_RNM_SPECULAR_VERTEX::GetVertexType() )
				l_NumMaterials=6;	

			if (l_NumMaterials >= 0)
			{
				for (int i=0;i<l_NumMaterials;++i)
				{
					// Leemos longitud del nombre del material
					uint16 l_TextureNameSize;
					fread(&l_TextureNameSize,sizeof(uint16),1,l_File);

					// Leemos nombre del material
					std::string l_TextureName;
					l_TextureName.resize(l_TextureNameSize+1);
					fread(&l_TextureName[0],sizeof(char),l_TextureNameSize+1,l_File);

					if (l_TextureName != "")
					{
						// Cargamos material
						CTexture *l_Texture=CORE->GetTextureManager()->GetTexture(l_TextureName);
						m_Textures[RV].push_back(l_Texture);
					}
					else
					{
						std::string msg_error = "CStaticMesh::Load-> Falta textura en " + FileName + " !\n";
						LOGGER->AddNewLog(ELL_ERROR, msg_error.c_str());
					}
				}

				CRenderableVertexs* l_RV=NULL;

				// Leemos número de vertices que contiene el fichero
				uint16 l_NumVertexs;
				fread(&l_NumVertexs,sizeof(uint16),1,l_File);               

				// Cargamos datos de los vertices
				void *l_Vtxs=NULL;
				size_t l_SizeOfVertexType=0;
				if (l_VertexType==TNORMAL_TEXTURE1_VERTEX::GetVertexType())
					l_SizeOfVertexType = sizeof(TNORMAL_TEXTURE1_VERTEX);
				else if (l_VertexType==TNORMAL_COLORED_VERTEX::GetVertexType())
					l_SizeOfVertexType = sizeof(TNORMAL_COLORED_VERTEX);			
				else if (l_VertexType==TNORMAL_TEXTURE1_TEXTURE2_VERTEX::GetVertexType())
					l_SizeOfVertexType = sizeof(TNORMAL_TEXTURE1_TEXTURE2_VERTEX);
				else if (l_VertexType==TNORMAL_COLORED_VERTEX::GetVertexType())
					l_SizeOfVertexType = sizeof(TNORMAL_COLORED_VERTEX);
				else if (l_VertexType==TNORMAL_TANGENT_BINORMAL_TEXTURED_VERTEX::GetVertexType())
					l_SizeOfVertexType = sizeof(TNORMAL_TANGENT_BINORMAL_TEXTURED_VERTEX);
				else if (l_VertexType==TNORMAL_TANGENT_BINORMAL_TEXTURE1_TEXTURE2_VERTEX::GetVertexType())
					l_SizeOfVertexType = sizeof(TNORMAL_TANGENT_BINORMAL_TEXTURE1_TEXTURE2_VERTEX);
				else if (l_VertexType==TNORMAL_TANGENT_BINORMAL_TEXTURE1_TEXTURE2_RNM_SPECULAR_VERTEX::GetVertexType())
					l_SizeOfVertexType = sizeof(TNORMAL_TANGENT_BINORMAL_TEXTURE1_TEXTURE2_RNM_SPECULAR_VERTEX);

				if (l_SizeOfVertexType!=0)
				{
					l_Vtxs = malloc(l_NumVertexs * l_SizeOfVertexType);
					fread(l_Vtxs, l_SizeOfVertexType, l_NumVertexs, l_File);
				}
				
				unsigned char *l_VtxAddress=(unsigned char *)l_Vtxs;
				for(int i =0;i<l_NumVertexs;i++)
				{
					Vect3f *l_VtxPointer=(Vect3f *)l_VtxAddress;
					Vect3f l_Vtx=*l_VtxPointer;
					m_VB.push_back(l_Vtx);
					l_VtxAddress+=l_SizeOfVertexType;
				}

				// Leemos número de indices que contiene el fichero
				void* l_Idxs=NULL;
				uint16 l_NumIndexs;
				fread(&l_NumIndexs,sizeof(uint16),1,l_File);               

				// Cargamos datos de los indices
				l_Idxs = malloc(l_NumIndexs * sizeof(uint16));
				fread(l_Idxs, sizeof(uint16), l_NumIndexs, l_File);
				uint16* l_Indexs=(uint16*)(l_Idxs);
				//Vector para physx
				for(int i =0;i<l_NumIndexs;i++)
				{
					uint32 l_Idx=l_Indexs[i]+l_OffsetVtxPhysic;
					m_IB.push_back(l_Idx);
				}
				l_OffsetVtxPhysic=l_OffsetVtxPhysic+l_NumVertexs;
				m_NumFaces=l_NumIndexs/3;
				m_NumVertexs=l_NumIndexs;

				//Optimitzar l'ordre
				//VertexCacheOptimisation( l_Vtxs, l_Indexs, l_NumVertexs, l_NumIndexs, l_SizeOfVertexType );

				// Creamos el objeto de la clase RenderableVertex y lo metemos en el array m_RVs
				if(l_VertexType==TNORMAL_TEXTURE1_VERTEX::GetVertexType())
					l_RV = new CIndexedVertexs<TNORMAL_TEXTURE1_VERTEX>(CORE->GetRenderManager(), l_Vtxs, l_Idxs, l_NumVertexs, l_NumIndexs);
				else if (l_VertexType==TNORMAL_COLORED_VERTEX::GetVertexType())
					l_RV = new CIndexedVertexs<TNORMAL_COLORED_VERTEX>(CORE->GetRenderManager(), l_Vtxs, l_Idxs, l_NumVertexs, l_NumIndexs);
				else if(l_VertexType==TNORMAL_TEXTURE1_TEXTURE2_VERTEX::GetVertexType())
					l_RV = new CIndexedVertexs<TNORMAL_TEXTURE1_TEXTURE2_VERTEX>(CORE->GetRenderManager(), l_Vtxs, l_Idxs, l_NumVertexs, l_NumIndexs);
				else if(l_VertexType==TNORMAL_COLORED_VERTEX::GetVertexType())
					l_RV = new CIndexedVertexs<TNORMAL_COLORED_VERTEX>(CORE->GetRenderManager(), l_Vtxs, l_Idxs, l_NumVertexs, l_NumIndexs);
				else if(l_VertexType==TNORMAL_TANGENT_BINORMAL_TEXTURED_VERTEX::GetVertexType())
				{
					CalcTangentsAndBinormals(l_Vtxs, (unsigned short *)l_Idxs, l_NumVertexs, l_NumIndexs,sizeof(TNORMAL_TANGENT_BINORMAL_TEXTURED_VERTEX), 0, 12, 28, 44, 60);		
					l_RV = new CIndexedVertexs<TNORMAL_TANGENT_BINORMAL_TEXTURED_VERTEX>(CORE->GetRenderManager(), l_Vtxs, l_Idxs, l_NumVertexs, l_NumIndexs);
				}
				else if(l_VertexType==TNORMAL_TANGENT_BINORMAL_TEXTURE1_TEXTURE2_VERTEX::GetVertexType())
				{
					CalcTangentsAndBinormals(l_Vtxs, (unsigned short *)l_Idxs, l_NumVertexs, l_NumIndexs,sizeof(TNORMAL_TANGENT_BINORMAL_TEXTURE1_TEXTURE2_VERTEX), 0, 12, 28, 44, 60);		
					l_RV = new CIndexedVertexs<TNORMAL_TANGENT_BINORMAL_TEXTURE1_TEXTURE2_VERTEX>(CORE->GetRenderManager(), l_Vtxs, l_Idxs, l_NumVertexs, l_NumIndexs);
				}
				else if(l_VertexType==TNORMAL_TANGENT_BINORMAL_TEXTURE1_TEXTURE2_RNM_SPECULAR_VERTEX::GetVertexType())
				{
					CalcTangentsAndBinormals(l_Vtxs, (unsigned short *)l_Idxs, l_NumVertexs, l_NumIndexs,sizeof(TNORMAL_TANGENT_BINORMAL_TEXTURE1_TEXTURE2_RNM_SPECULAR_VERTEX), 0, 12, 28, 44, 60);		
					l_RV = new CIndexedVertexs<TNORMAL_TANGENT_BINORMAL_TEXTURE1_TEXTURE2_RNM_SPECULAR_VERTEX>(CORE->GetRenderManager(), l_Vtxs, l_Idxs, l_NumVertexs, l_NumIndexs);
				}

				// Añadimos el nuevo renderable object al vector de renderable objects y cargamos technique por defecto segun tipo de vertice
				if (l_RV != NULL)
				{
					m_RVs.push_back(l_RV);
					m_VertexTypes.push_back(l_VertexType);
					m_RenderableObjectTechniqueName.push_back(CORE->GetEffectManager()->GetTechniqueEffectNameByVertexDefault(l_VertexType));		
				}

				if (l_Vtxs!=NULL)
					free(l_Vtxs);
				if (l_Idxs!=NULL)	
					free(l_Idxs);
			}
			else
			{
				std::string msg_error = "CStaticMesh::Load-> Tipo de vertice desconocido en " + FileName + " !\n";
				LOGGER->AddNewLog(ELL_ERROR, msg_error.c_str());
			}
		}

		GetRenderableObjectTechnique();

		// Leemos vertices minimo, maximo, centro y radio de la esfera contenedora del mesh
		fread(&m_vminPoint.x,sizeof(Vect3f), 1, l_File);
		fread(&m_vmaxPoint.x,sizeof(Vect3f), 1, l_File);
		fread(&m_vcenter.x,sizeof(Vect3f), 1, l_File);
		fread(&m_fradius,sizeof(float), 1, l_File);

		// Leemos el footer del fichero
		uint16 l_FileFooter;
		fread(&l_FileFooter,sizeof(uint16),1,l_File);  
		if(l_FileFooter!=STATIC_MESH_FILE_FOOTER)
		{
			fclose(l_File);
			return false;
		}
		fclose(l_File);


		return true;		
	}

	return false;
}

bool CStaticMesh::ReLoad ()
{
	if (m_FileName != "")
	{
		Release();
		return Load (m_FileName);
	}

	return false;
}


void CStaticMesh::Render (CRenderManager *RM, const Vect3f& position, const CFrustum* Frustum, bool forwardRender) const
{
	CEffectTechnique* l_EffectTechnique = NULL;
	for(size_t i=0;i<m_RVs.size();++i)
	{
		for (size_t t=0;t<m_Textures[i].size();++t)
		{
			CTexture* l_Text = m_Textures[i][t];
			if (l_Text!=NULL)
			{
				l_Text->Activate(t);
			}
		}

		if (m_RenderableObjectTechniques.size())
		{
			// Render usando effects, si hay techniques cargadas
			CRenderableObjectTechnique *l_Technique = m_RenderableObjectTechniques[i];
			if (l_Technique != NULL)
			{
				l_EffectTechnique = l_Technique->GetEffectTechnique();
				if ( l_EffectTechnique != NULL)
				{
					if (forwardRender)
					{
						Vect3f l_Center = position + m_vcenter;
						l_EffectTechnique->GetEffect()->SetLights(MAX_LIGHTS_BY_SHADER, l_Center, Frustum);		
					}

					l_EffectTechnique->BeginRender();
					m_RVs[i]->Render(RM, l_EffectTechnique);
				}
			}
		}
		else
		{
			// Render sin usar effects efecto
			m_RVs[i]->Render(RM);		
		}
	}
}

void CStaticMesh::RenderShadow (CRenderManager *RM, const Vect3f& position, const CFrustum* Frustum, CLight* Light) const
{
	CEffectTechnique* l_EffectTechnique = NULL;
	for(size_t i=0;i<m_RVs.size();++i)
	{
		for (size_t t=0;t<m_Textures[i].size();++t)
		{
			CTexture* l_Text = m_Textures[i][t];
			if (l_Text!=NULL)
			{
				l_Text->Activate(t);
			}
		}

		if (m_RenderableObjectTechniques.size())
		{
			// Render usando effects, si hay techniques cargadas
			CRenderableObjectTechnique *l_Technique = m_RenderableObjectTechniques[i];
			if (l_Technique != NULL)
			{
				l_EffectTechnique = l_Technique->GetEffectTechnique();
				if ( l_EffectTechnique != NULL)
				{
					l_EffectTechnique->GetEffect()->SetLight(0, Light);
					l_EffectTechnique->BeginRender();
					m_RVs[i]->Render(RM, l_EffectTechnique);
				}
			}
		}
		else
		{
			// Render sin usar effects efecto
			m_RVs[i]->Render(RM);		
		}
	}
}

void CStaticMesh::Release()
{
	for(size_t i=0;i<m_RVs.size();++i)
	{
		CHECKED_DELETE(m_RVs[i]);
	}
	m_RVs.clear();
	
	m_VB.clear();
	m_IB.clear();
	m_Textures.clear();
}

bool CStaticMesh::GetRenderableObjectTechnique()
{
	CRenderableObjectsTechniqueManager *l_ROTM= CORE->GetRenderableObjectsTechniqueManager();
	bool l_Ok=true;
	for(size_t i=0;i<m_VertexTypes.size();++i)
	{
		if(m_RenderableObjectTechniqueName[i]=="")
			m_RenderableObjectTechniqueName[i]=l_ROTM->GetRenderableObjectTechniqueNameByVertexType(m_VertexTypes[i]);

		CRenderableObjectTechnique *l_ROT=l_ROTM->GetResource(m_RenderableObjectTechniqueName[i],"");	

		m_RenderableObjectTechniques.push_back(l_ROT);		
		if(l_ROT==NULL  ||l_ROT->GetEffectTechnique() == NULL)
		{
			/* comentar con Jordi, esta mierda llena el logger!
			std::string msg_error = "Error trying to GetRenderableObjectTechnique " +m_RenderableObjectTechniqueName[i]+ " on CStaticMesh";
			LOGGER->AddNewLog(ELL_WARNING, msg_error.c_str());
			*/
			l_Ok = false;
		}

		if (m_VertexTypes[i]==TNORMAL_TEXTURE1_TEXTURE2_VERTEX::GetVertexType())
		{
			if(l_ROT!=NULL && l_ROT->GetEffectTechnique() != NULL)
			{
				l_ROT->GetEffectTechnique()->setLightMap(*m_Textures[i][1]);
			}
		}

		l_Ok=l_Ok && l_ROT!=NULL;
	}
	return l_Ok;
}

void CStaticMesh::SetTexture(CTexture* Texture, size_t stage)
{
	if (m_Textures.size() > stage)
	{
		m_Textures[stage][0] = Texture;
	}
}

CTexture* CStaticMesh::GetTexture(size_t stage)
{
	return m_Textures[stage][0];
}

void CalcTangentsAndBinormals(void *VtxsData, unsigned short *IdxsData, size_t VtxCount,
	size_t IdxCount, size_t VertexStride, size_t GeometryStride, size_t NormalStride,
	size_t TangentStride, size_t BiNormalStride, size_t TextureCoordsStride)
{
	D3DXVECTOR3 *tan1 = new D3DXVECTOR3[VtxCount * 2];
	D3DXVECTOR3 *tan2 = tan1 + VtxCount;
	ZeroMemory(tan1, VtxCount * sizeof(D3DXVECTOR3) * 2);
	unsigned char *l_VtxAddress=(unsigned char *)VtxsData;


	for(size_t b=0;b<IdxCount;b+=3)
	{
		unsigned short i1=IdxsData[b];
		unsigned short i2=IdxsData[b+1];
		unsigned short i3=IdxsData[b+2];
		D3DXVECTOR3 *v1=(D3DXVECTOR3 *)
			&l_VtxAddress[i1*VertexStride+GeometryStride];
		D3DXVECTOR3 *v2=(D3DXVECTOR3 *)
			&l_VtxAddress[i2*VertexStride+GeometryStride];
		D3DXVECTOR3 *v3=(D3DXVECTOR3 *)
			&l_VtxAddress[i3*VertexStride+GeometryStride];
		D3DXVECTOR2 *w1=(D3DXVECTOR2 *)
			&l_VtxAddress[i1*VertexStride+TextureCoordsStride];
		D3DXVECTOR2 *w2=(D3DXVECTOR2 *)
			&l_VtxAddress[i2*VertexStride+TextureCoordsStride];
		D3DXVECTOR2 *w3=(D3DXVECTOR2 *)
			&l_VtxAddress[i3*VertexStride+TextureCoordsStride];
		float x1=v2->x-v1->x;
		float x2=v3->x-v1->x;
		float y1=v2->y-v1->y;
		float y2=v3->y-v1->y;
		float z1=v2->z-v1->z;
		float z2=v3->z-v1->z;
		float s1=w2->x-w1->x;
		float s2=w3->x-w1->x;
		float t1=w2->y-w1->y;
		float t2=w3->y-w1->y;
		float r = 1.0F / (s1 * t2 - s2 * t1);
		D3DXVECTOR3 sdir((t2 * x1 - t1 * x2) * r, (t2 * y1 - t1 * y2) * r,
			(t2 * z1 - t1 * z2) * r);
		D3DXVECTOR3 tdir((s1 * x2 - s2 * x1) * r, (s1 * y2 - s2 * y1) * r,
			(s1 * z2 - s2 * z1) * r);
		assert(i1<VtxCount);
		assert(i2<VtxCount);
		assert(i3<VtxCount);
		tan1[i1] += sdir;
		tan1[i2] += sdir;
		tan1[i3] += sdir;
		tan2[i1] += tdir;
		tan2[i2] += tdir;
		tan2[i3] += tdir;
	}
	for (size_t b=0;b<VtxCount;++b)
	{
		D3DXVECTOR3 *l_NormalVtx=(D3DXVECTOR3 *)&l_VtxAddress[b*VertexStride+NormalStride];
		D3DXVECTOR3 *l_TangentVtx=(D3DXVECTOR3 *)&l_VtxAddress[b*VertexStride+TangentStride];
		D3DXVECTOR4 *l_TangentVtx4=(D3DXVECTOR4 *)&l_VtxAddress[b*VertexStride+TangentStride];
		D3DXVECTOR3 *l_BiNormalVtx=(D3DXVECTOR3 *)&l_VtxAddress[b*VertexStride+BiNormalStride];
		const D3DXVECTOR3& t=tan1[b];
		// Gram-Schmidt orthogonalize
		D3DXVECTOR3 l_VAl=t-(*l_NormalVtx)*D3DXVec3Dot(l_NormalVtx, &t);
		D3DXVec3Normalize(l_TangentVtx,&l_VAl);
		//tangent[a] = (t - n * Dot(n, t)).Normalize();
		// Calculate handedness
		D3DXVECTOR3 l_Cross;
		D3DXVec3Cross(&l_Cross,l_NormalVtx,l_TangentVtx);
		l_TangentVtx4->w=(D3DXVec3Dot(&l_Cross,&tan2[b])< 0.0f ) ? -1.0f : 1.0f;
		//tangent[a].w = (Dot(Cross(n, t), tan2[a]) < 0.0F) ? -1.0F : 1.0F;
		D3DXVec3Cross(l_BiNormalVtx,l_NormalVtx,l_TangentVtx);
	}
	delete[] tan1;
}

#if 0
// ------------------------------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------------------------------
// Vertex Cache Optimization ----------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------------------------------

struct SVCO_Triangle {
  bool added;
  float score;
  uint16 vertexs[3];
};

struct SVCO_Vertex {
  int cachePos;
  float score;
  int useInTris;
  int unusedTris;
  std::vector<uint16> tris;
};

#define FVS_CacheDecayPower     1.5f
#define FVS_LastTriScore        0.75f
#define FVS_ValenceBoostScale   2.0f
#define FVS_ValenceBoostPower   0.5f
#define FVS_MaxSizeVertexCache  32
float FindVertexScore ( const SVCO_Vertex &_VData )
{
  if ( _VData.unusedTris == 0 )
  {
    // No tri needs this vertex!
    return -1.0f;
  }
  float l_fScore = 0.0f;
  int l_iCachePosition = _VData.cachePos;
  if ( l_iCachePosition < 0 )
  {
    // Vertex is not in FIFO cache - no score.
  }
  else
  {
    if ( l_iCachePosition < 3 )
    {
      // This vertex was used in the last triangle,
      // so it has a fixed score, whichever of the three
      // it's in. Otherwise, you can get very different
      // answers depending on whether you add
      // the triangle 1,2,3 or 3,1,2 - which is silly.
      l_fScore = FVS_LastTriScore;
    }
    else
    {
      assert ( l_iCachePosition < FVS_MaxSizeVertexCache );
      // Points for being high in the cache.
      const float Scaler = 1.0f / ( FVS_MaxSizeVertexCache - 3 );
      l_fScore = 1.0f - ( l_iCachePosition - 3 ) * Scaler;
      l_fScore = powf ( l_fScore, FVS_CacheDecayPower );
    }
  }

  // Bonus points for having a low number of tris still to
  // use the vert, so we get rid of lone verts quickly.
  float l_fValenceBoost = powf ( (float)_VData.unusedTris, -FVS_ValenceBoostPower );
  l_fScore += FVS_ValenceBoostScale * l_fValenceBoost;
  return l_fScore;
}

float FindTriangleScore ( const SVCO_Triangle& _VTriangle, SVCO_Vertex *_VData )
{
  return _VData[_VTriangle.vertexs[0]].score + _VData[_VTriangle.vertexs[1]].score + _VData[_VTriangle.vertexs[2]].score;
}


void CopyVertex(void* _pDstBuffer, void* _pSrcBuffer, size_t _iVStride, uint32 _iDstIndex, uint32 _iSrcIndex)
{
  memcpy((void*)&(((char*)_pDstBuffer)[_iDstIndex * _iVStride]), (void*)&(((char*)_pSrcBuffer)[_iSrcIndex * _iVStride]), _iVStride);
}

struct DiscartedVertices {
  int a, b, c;
};

DiscartedVertices UpdateCache(int* _Cache, uint32 _V1, uint32 _V2, uint32 _V3)
{
  DiscartedVertices l_dv;
  l_dv.a = _V1;
  l_dv.b = _V2;
  l_dv.c = _V3;

  uint16 l_iReplaced = 0;
  for(uint16 i = 0; i < FVS_MaxSizeVertexCache; ++i)
  {
    if(_V1 == _Cache[i] || _V2 == _Cache[i] || _V3 == _Cache[i])
    {
      l_iReplaced++;
      _Cache[i] = l_dv.a;
      l_dv.a = l_dv.b;
      l_dv.b = l_dv.c;
      l_dv.c = -1;
    }
    else
    {
      int aux = _Cache[i];
      _Cache[i] = l_dv.a;
      l_dv.a = l_dv.b;
      l_dv.b = l_dv.c;
      l_dv.c = aux;
    }
  }
  return l_dv;
}

int RecomputeScores(int* _Cache, const DiscartedVertices& _dv, SVCO_Vertex* _vVertex, SVCO_Triangle* _vTriangles, uint16 _iNumTriangles)
{
  //Recompute Vertex scores
  for(uint16 i = 0; i < FVS_MaxSizeVertexCache; ++i)
  {
    if(_Cache[i] < 0) break; //No hi ha res més a la cache

    SVCO_Vertex* l_pVertex = _vVertex + _Cache[i];
    l_pVertex->cachePos = i;
    l_pVertex->score = FindVertexScore(*l_pVertex);
  }
  if(_dv.a >= 0)
  {
    SVCO_Vertex* l_pVertex = _vVertex + _dv.a;
    l_pVertex->cachePos = -1;
    l_pVertex->score = FindVertexScore(*l_pVertex);
  }
  if(_dv.b >= 0)
  {
    SVCO_Vertex* l_pVertex = _vVertex + _dv.b;
    l_pVertex->cachePos = -1;
    l_pVertex->score = FindVertexScore(*l_pVertex);
  }
  if(_dv.c >= 0)
  {
    SVCO_Vertex* l_pVertex = _vVertex + _dv.c;
    l_pVertex->cachePos = -1;
    l_pVertex->score = FindVertexScore(*l_pVertex);
  }

  //Recompute Triangle Scores
  int l_iBestTriangle = -1;
  for(uint16 i = 0; i < FVS_MaxSizeVertexCache; ++i)
  {
    if(_Cache[i] < 0) break; //No hi ha res més a la cache
    SVCO_Vertex* l_pVertex = _vVertex + _Cache[i];
    for(uint16 j = 0; j < l_pVertex->tris.size(); ++j)
    {
      SVCO_Triangle* l_pTriangle = _vTriangles + l_pVertex->tris[j];
      if(!l_pTriangle->added)
      {
        l_pTriangle->score = FindTriangleScore(*l_pTriangle, _vVertex);
        if(l_iBestTriangle < 0 || l_pTriangle->score > _vTriangles[l_iBestTriangle].score)
        {
          l_iBestTriangle = l_pVertex->tris[j];
        }
      }
    }
  }
  if(_dv.a >= 0)
  {
    SVCO_Vertex* l_pVertex = _vVertex + _dv.a;
    for(uint16 j = 0; j < l_pVertex->tris.size(); ++j)
    {
      SVCO_Triangle* l_pTriangle = _vTriangles + l_pVertex->tris[j];
      if(!l_pTriangle->added)
      {
        l_pTriangle->score = FindTriangleScore(*l_pTriangle, _vVertex);
        if(l_iBestTriangle < 0 || l_pTriangle->score > _vTriangles[l_iBestTriangle].score)
        {
          l_iBestTriangle = l_pVertex->tris[j];
        }
      }
    }
  }
  if(_dv.b >= 0)
  {
    SVCO_Vertex* l_pVertex = _vVertex + _dv.b;
    for(uint16 j = 0; j < l_pVertex->tris.size(); ++j)
    {
      SVCO_Triangle* l_pTriangle = _vTriangles + l_pVertex->tris[j];
      if(!l_pTriangle->added)
      {
        l_pTriangle->score = FindTriangleScore(*l_pTriangle, _vVertex);
        if(l_iBestTriangle < 0 || l_pTriangle->score > _vTriangles[l_iBestTriangle].score)
        {
          l_iBestTriangle = l_pVertex->tris[j];
        }
      }
    }
  }
  if(_dv.c >= 0)
  {
    SVCO_Vertex* l_pVertex = _vVertex + _dv.c;
    for(uint16 j = 0; j < l_pVertex->tris.size(); ++j)
    {
      SVCO_Triangle* l_pTriangle = _vTriangles + l_pVertex->tris[j];
      if(!l_pTriangle->added)
      {
        l_pTriangle->score = FindTriangleScore(*l_pTriangle, _vVertex);
        if(l_iBestTriangle < 0 || l_pTriangle->score > _vTriangles[l_iBestTriangle].score)
        {
          l_iBestTriangle = l_pVertex->tris[j];
        }
      }
    }
  }

  if(l_iBestTriangle < 0)
  {
    for(uint16 i = 0; i < _iNumTriangles; ++i)
    {
      if(!_vTriangles[i].added && (l_iBestTriangle < 0 || _vTriangles[i].score > _vTriangles[l_iBestTriangle].score))
      {
        l_iBestTriangle = i;
      }
    }
  }

  return l_iBestTriangle;
}

void VertexCacheOptimisation( void *_VData, uint16 *_IData, size_t _iVCount,
                              size_t _iICount, size_t _iVStride              )
{
  uint16         l_iNumTriangles     = _iICount / 3;
  char*          l_vData             = new char[_iVStride * _iVCount];
  SVCO_Vertex*   l_vVertex           = new SVCO_Vertex[_iVCount];
  SVCO_Triangle* l_vTriangles        = new SVCO_Triangle[l_iNumTriangles];
  uint16 *       l_iTrianglesInOrder = new uint16[l_iNumTriangles];
  int*           l_MappedIndexes     = new int[_iVCount];
  for(uint16 i = 0; i < _iVCount; ++i) l_MappedIndexes[i] = -1;


  // Initialization ------------------------------------------------------------------------------------------
  memcpy((void*)l_vData,_VData,_iVStride*_iVCount); //Copiem les dades a un buffer per separat (després les voldrem reordenar)

  // Inicialitzem els vertexos
  for(uint16 i = 0; i < _iVCount; ++i)
  {
    //CopyVertex(l_vData,_VData,_iVStride,i,i);
    l_vVertex[i].cachePos   = -1;
    l_vVertex[i].useInTris  = 0;
    l_vVertex[i].unusedTris = 0;
  }


  //Inicialitzem els triangles
  for(uint16 i = 0; i < l_iNumTriangles; ++i)
  {
    l_vTriangles[i].added      = false;
    // Per cada vertex
    for(uint16 j = 0; j < 3; ++j)
    {
      //n'agafem l'index
      uint32 l_iVertexIndex = _IData[i*3 + j];
      l_vTriangles[i].vertexs[j] = l_iVertexIndex;

      //indiquem al vertex corresponent que ès adjacent a un triangle.
      l_vVertex[l_iVertexIndex].useInTris  ++;
      l_vVertex[l_iVertexIndex].unusedTris ++;

      l_vVertex[l_iVertexIndex].tris.push_back(i);
    }
  }

  //Calculem les puntuacions per vertex...
  for(uint16 i = 0; i < _iVCount; ++i)
  {
    l_vVertex[i].score = FindVertexScore(l_vVertex[i]);
  }

  //... i per triangle (ja que hi som, agafem el millor triangle)
  int l_iBestTriangle = 0;
  for(uint16 i = 0; i < l_iNumTriangles; ++i)
  {
    l_vTriangles[i].score = FindTriangleScore(l_vTriangles[i],l_vVertex);
    if(l_vTriangles[i].score > l_vTriangles[l_iBestTriangle].score)
    {
      l_iBestTriangle = i;
    }
  }

  // Main Body ---------------------------------------------------------------------------------------------------
  int l_Cache[FVS_MaxSizeVertexCache];
  
  // inicialitzem la cache
  for(uint16 i = 0; i < FVS_MaxSizeVertexCache; ++i)
  {
    l_Cache[i] = -1;
  }

  //El cos s'ha de fer un cop per triangle (busquem sempre el millor triangle a afegim i l'afegim, mai es fa backtracking ni lookahead)
  for(uint16 i = 0; i < l_iNumTriangles; ++i)
  {
    assert(!l_vTriangles[l_iBestTriangle].added);
    l_iTrianglesInOrder[i] = l_iBestTriangle;
    if(i == l_iNumTriangles-1) break; // És l'últim triangle, no cal que recalculem res!

    // Setejem com a afegit el triangle
    l_vTriangles[l_iBestTriangle].added = true;

    // Indiquem als vertexos utilitzats que ja els fa servir un triangle menys
    l_vVertex[l_vTriangles[l_iBestTriangle].vertexs[0]].unusedTris--;
    l_vVertex[l_vTriangles[l_iBestTriangle].vertexs[1]].unusedTris--;
    l_vVertex[l_vTriangles[l_iBestTriangle].vertexs[2]].unusedTris--;

    // Actualitzem la cache amb els 3 vertexos nous (ens retorna els descartats)
    DiscartedVertices l_dv = UpdateCache(l_Cache, 
                                          l_vTriangles[l_iBestTriangle].vertexs[0],
                                          l_vTriangles[l_iBestTriangle].vertexs[1],
                                          l_vTriangles[l_iBestTriangle].vertexs[2]);

    // Actualitzem les puntuacions i agafem el millor triangle.
    l_iBestTriangle = RecomputeScores(l_Cache, l_dv, l_vVertex, l_vTriangles, l_iNumTriangles);
  }


  // Reordering pre-transform cache -------------------------------------------------------------
  uint16 l_iVertexPos = 0;
  for(uint16 i = 0; i < l_iNumTriangles; ++i)
  {
    SVCO_Triangle* l_pTriangle = l_vTriangles + l_iTrianglesInOrder[i];

    for(uint16 j = 0; j < 3; ++j)
    {
      if(l_MappedIndexes[l_pTriangle->vertexs[j]] < 0)
      {
        //No està afegit al nou vertex buffer
        // 1 - Afegim les dades
        CopyVertex(_VData,l_vData,_iVStride,l_iVertexPos,l_pTriangle->vertexs[j]);
        // 2 - Afegim el "mapejat" del vell index al nou index
        l_MappedIndexes[l_pTriangle->vertexs[j]] = l_iVertexPos;
        // 3 - Actualitzem la posició al nou buffer
        ++l_iVertexPos;
      }
      // Copiem els indexos reordenats
      _IData[i * 3 + j] = l_MappedIndexes[l_pTriangle->vertexs[j]];
    }
  }

  //netejem
  delete[] l_MappedIndexes;
  delete[] l_iTrianglesInOrder;
  delete[] l_vTriangles;
  delete[] l_vVertex;
  delete[] l_vData;
}

// ------------------------------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------------------------------
#endif