#include "Texture.h"
#include "RenderManager.h"
#include "Logger.h"
#include "Exception.h"
#include "Core.h"
#include "Base.h"


const std::string & CTexture::GetFileName() const
{
	return m_FileName;
}

bool CTexture::Load(const std::string &FileName)
{
	m_FileName = FileName;
	return LoadFile();
}

bool CTexture::Reload()
{
	if (m_OldRenderTarget != NULL)		// Recarga texturas creadas de forma dinamica para G-Buffer peta
	{
		Unload();
		return LoadFile();
	}
	return false;
}

void CTexture::Activate(size_t StageId)
{
	CORE->GetRenderManager()->GetDevice()->SetTexture(StageId, m_Texture);
}

void CTexture::Deactivate(size_t StageId)
{
	CORE->GetRenderManager()->GetDevice()->SetTexture((DWORD)StageId,NULL);
}

void CTexture::Unload()
{
	CHECKED_RELEASE(m_Texture);
	m_Texture=NULL;
}

bool CTexture::LoadFile()
{
	//HRESULT l_HR = D3DXCreateTextureFromFile(CORE->GetRenderManager()->GetDevice(), m_FileName.c_str(), &m_Texture);
	HRESULT l_HR = D3DXCreateTextureFromFileEx(CORE->GetRenderManager()->GetDevice(), m_FileName.c_str(), D3DX_DEFAULT_NONPOW2, D3DX_DEFAULT_NONPOW2, D3DX_FROM_FILE, 0, D3DFMT_FROM_FILE, D3DPOOL_MANAGED, D3DX_FILTER_NONE, D3DX_FILTER_NONE, 0, NULL, NULL, &m_Texture);
	return l_HR==S_OK;
}

LPDIRECT3DTEXTURE9 CTexture::GetTexture()
{
	return m_Texture;
}


bool CTexture::SetAsRenderTarget(size_t IdStage, bool viewportStencil)
{
	LPDIRECT3DDEVICE9 l_Device= CORE->GetRenderManager()->GetDevice();
	l_Device->GetRenderTarget((DWORD)IdStage, &m_OldRenderTarget);

	if(FAILED( m_Texture->GetSurfaceLevel( 0, &m_RenderTargetTexture )) )
		return false;

	l_Device->SetRenderTarget( (DWORD)IdStage, m_RenderTargetTexture );

	CHECKED_RELEASE(m_RenderTargetTexture);

	m_ViewportStencil=viewportStencil;
	if(!m_ViewportStencil)
	{
		if(FAILED( l_Device->GetDepthStencilSurface(&m_OldDepthStencilRenderTarget ) ) )
			return false;
		
		l_Device->SetDepthStencilSurface(m_DepthStencilRenderTargetTexture );
	}
	return true;
}

void CTexture::UnsetAsRenderTarget(size_t IdStage)
{
	LPDIRECT3DDEVICE9 l_Device=CORE->GetRenderManager()->GetDevice();

	if(!m_ViewportStencil)
	{
		l_Device->SetDepthStencilSurface(m_OldDepthStencilRenderTarget);
		CHECKED_RELEASE(m_OldDepthStencilRenderTarget);
	}
	l_Device->SetRenderTarget(IdStage, m_OldRenderTarget);
	CHECKED_RELEASE(m_OldRenderTarget);
}

CTexture::TFormatType CTexture::GetFormatTypeFromString(const std::string &FormatType)
{
	if(FormatType=="R32F")
		return CTexture::R32F;
	else if(FormatType=="RG32F")
		return CTexture::RG32F;
	else if(FormatType=="A16B16G16R16F")
		return CTexture::A16B16G16R16F;
	else if(FormatType=="A8R8G8B8")
		return CTexture::A8R8G8B8;
	else if(FormatType=="R8G8B8")
		return CTexture::R8G8B8;
	else if(FormatType=="X8R8G8B8")
		return CTexture::X8R8G8B8;
	else
	{
		//Info("Format Type '%s' not recognized", FormatType.c_str());
		std::string msg_error = "CTexture::FormatType->Error, tipo de formato no reconocido: " + FormatType;
		LOGGER->AddNewLog(ELOG_LEVEL::ELL_ERROR, msg_error.c_str());
		return CTexture::A8R8G8B8;
		//throw CException(__FILE__, __LINE__, msg_error);
	}
}


bool CTexture::Create(const std::string &Name, unsigned int Width, unsigned int Height, unsigned int MipMaps, TUsageType UsageType, TPoolType PoolType, TFormatType FormatType)
{
	//SetName(Name);
	//m_FileName = Name;
	D3DPOOL l_Pool=D3DPOOL_DEFAULT;
	DWORD l_UsageType=D3DUSAGE_DYNAMIC;
	D3DFORMAT l_Format=D3DFMT_A8R8G8B8;
	bool l_CreateDepthStencilSurface=false;
	
	switch(UsageType)
	{
		case DYNAMIC:
			l_UsageType=D3DUSAGE_DYNAMIC;
			break;
		case RENDERTARGET:
			l_CreateDepthStencilSurface=true;
			l_UsageType=D3DUSAGE_RENDERTARGET;
			break;
	}
	
	switch(PoolType)
	{
		case DEFAULT:
			l_Pool=D3DPOOL_DEFAULT;
			break;
		case SYSTEMMEM:
			l_Pool=D3DPOOL_SYSTEMMEM;
			break;
	}

	switch(FormatType)
	{
		case A8R8G8B8:
			l_Format=D3DFMT_A8R8G8B8;
			break;
		case A16B16G16R16F:
			l_Format=D3DFMT_A16B16G16R16F;
			break;
		case R8G8B8:
			l_Format=D3DFMT_R8G8B8;
			break;
		case X8R8G8B8:
			l_Format=D3DFMT_X8R8G8B8;
			break;
		case R32F:
			l_Format=D3DFMT_R32F;
			break;
		case RG32F:
			l_Format=D3DFMT_G32R32F;
			break;
	}
	HRESULT hr=CORE->GetRenderManager()->GetDevice()->CreateTexture(Width, Height, MipMaps, l_UsageType, l_Format, l_Pool,&m_Texture,NULL);
	if(l_CreateDepthStencilSurface)
	{
		CORE->GetRenderManager()->GetDevice()->CreateDepthStencilSurface(Width, Height, D3DFMT_D24S8,D3DMULTISAMPLE_NONE, 0, TRUE,&m_DepthStencilRenderTargetTexture, NULL);
		assert(m_DepthStencilRenderTargetTexture!=NULL);
	}
	
	SetName(Name);
	assert(m_Texture!=NULL);
	assert(hr==D3D_OK);
	m_Width=Width;
	m_Height=Height;

	return hr!=D3D_OK;
}


void CTexture::CaptureFrameBuffer(size_t IdStage)
{
	LPDIRECT3DDEVICE9 l_Device=CORE->GetRenderManager()->GetDevice();
	LPDIRECT3DSURFACE9 l_RenderTarget, l_Surface;
	
	m_Texture->GetSurfaceLevel(0,&l_Surface);
	l_Device->GetRenderTarget(IdStage,&l_RenderTarget);
	if (l_RenderTarget!=NULL) {
		l_Device->StretchRect(l_RenderTarget,NULL, l_Surface,NULL,D3DTEXF_NONE);
		l_RenderTarget->Release();
	}
}


