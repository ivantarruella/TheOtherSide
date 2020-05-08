#pragma once
#ifndef __CLASS_INDEXEDVERTEXS_H__
#include "VertexType.h"
#include "RenderManager.h"
#include "RenderableVertexs.h"
#include "EffectTechnique.h"

template<class T>
class CIndexedVertexs : public CRenderableVertexs
{
protected:
	inline size_t GetVertexSize() const {return sizeof(T);}
	inline size_t GetIndexSize() const {return sizeof(unsigned short);}

public:
	CIndexedVertexs(CRenderManager *RM, void *VertexAddress, void *IndexAddres, size_t VertexCount, size_t IndexCount)
	{
		m_VertexCount = VertexCount;
		m_IndexCount = IndexCount;

		if (RM->GetDevice()->CreateVertexBuffer(GetVertexSize()*m_VertexCount, D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY, T::GetFVF(), D3DPOOL_DEFAULT, &m_VB, NULL)==D3D_OK)
		{
			void *l_Data=NULL;
			m_VB->Lock(0, GetVertexSize()*m_VertexCount, &l_Data, 0);
			memcpy(l_Data, VertexAddress, GetVertexSize()*m_VertexCount);		
			m_VB->Unlock();
		}

		if (RM->GetDevice()->CreateIndexBuffer(GetIndexSize()*m_IndexCount, D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, D3DPOOL_DEFAULT, &m_IB, NULL)==D3D_OK)
		{
			void *l_Data=NULL;
			m_IB->Lock(0, GetIndexSize()*m_IndexCount, &l_Data, 0);
			memcpy(l_Data, IndexAddres, GetIndexSize()*m_IndexCount);		
			m_IB->Unlock();
		}		
	}

	~CIndexedVertexs()
	{
		m_IndexCount=0;
		m_VertexCount=0;
		CHECKED_RELEASE(m_IB);
		CHECKED_RELEASE(m_VB);
	}

	virtual bool Render(CRenderManager *RM) const
	{

		RM->GetDevice()->SetStreamSource(0, m_VB, 0, GetVertexSize());
		RM->GetDevice()->SetIndices(m_IB);
		RM->GetDevice()->SetFVF(T::GetFVF());
		HRESULT l_HR=RM->GetDevice()->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, m_VertexCount, 0, m_IndexCount/3);

		return (l_HR==D3D_OK);
	}
	
	virtual bool Render(CRenderManager *RM, CEffectTechnique *EffectTechnique) const
	{
		return Render(RM, EffectTechnique, 0, 0, (uint16)m_VertexCount, 0, (uint16) m_IndexCount/3);
	}
	
	virtual bool Render(CRenderManager *RM, CEffectTechnique *EffectTechnique, int BaseVertexIndex, uint16 MinVertexIndex, uint16 NumVertices, uint16 startIndex, uint16 FaceCount) const
	{
		 
		LPDIRECT3DDEVICE9 l_Device=RM->GetDevice();
		UINT l_NumPasses;
		if(EffectTechnique==NULL || EffectTechnique->GetD3DTechnique()==NULL || EffectTechnique->GetEffect()==NULL || EffectTechnique->GetEffect()->GetD3DEffect()==NULL)
			return false;
		LPD3DXEFFECT l_Effect=EffectTechnique->GetEffect()->GetD3DEffect();
		l_Effect->SetTechnique(EffectTechnique->GetD3DTechnique());
		if(SUCCEEDED(l_Effect->Begin(&l_NumPasses,0)))
		{
			l_Device->SetVertexDeclaration(T::GetVertexDeclaration());
			l_Device->SetStreamSource(0,m_VB,0,sizeof(T));
			l_Device->SetIndices(m_IB);
			for (UINT b=0;b<l_NumPasses;++b)
			{
				l_Effect->BeginPass(b);
				l_Device->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, BaseVertexIndex, MinVertexIndex, NumVertices, startIndex, FaceCount);
				l_Effect->EndPass();
			}
			l_Effect->End();
		}
		return true;
	}

	virtual inline unsigned short GetVertexType() const {return T::GetVertexType();}
};

#endif