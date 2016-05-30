#include "CubeTexture.h"
#include "RenderManager.h"
#include "Logger.h"
#include "Exception.h"
#include "Core.h"
#include "Base.h"


void CCubeTexture::Activate(size_t StageId)
{
	CORE->GetRenderManager()->GetDevice()->SetTexture(StageId, m_CubeTexture);
}

void CCubeTexture::Deactivate(size_t StageId)
{
	CORE->GetRenderManager()->GetDevice()->SetTexture((DWORD)StageId,NULL);
}

void CCubeTexture::Release()
{
	CHECKED_RELEASE(m_CubeTexture);
	m_CubeTexture=NULL;
	CHECKED_RELEASE(cubeFacePX);
	cubeFacePX=NULL;
	CHECKED_RELEASE(cubeFacePY);
	cubeFacePY=NULL;
	CHECKED_RELEASE(cubeFacePZ);
	cubeFacePZ=NULL;
	CHECKED_RELEASE(cubeFaceNX);
	cubeFaceNX=NULL;
	CHECKED_RELEASE(cubeFaceNX);
	cubeFaceNX=NULL;
	CHECKED_RELEASE(cubeFaceNX);
	cubeFaceNX=NULL;
}

bool CCubeTexture::SetAsRenderTarget(D3DCUBEMAP_FACES face, size_t IdStage, bool viewportStencil)
{
	LPDIRECT3DDEVICE9 l_Device= CORE->GetRenderManager()->GetDevice();
	l_Device->GetRenderTarget((DWORD)IdStage, &m_OldRenderTarget);

	if(FAILED( m_CubeTexture->GetCubeMapSurface(face, 0, &m_RenderTargetTexture )) )
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

void CCubeTexture::UnsetAsRenderTarget(size_t IdStage)
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

CCubeTexture::TFormatType CCubeTexture::GetFormatTypeFromString(const std::string &FormatType)
{
	if(FormatType=="R32F")
		return CCubeTexture::R32F;
	else if(FormatType=="RG32F")
		return CCubeTexture::RG32F;
	else if(FormatType=="A16B16G16R16F")
		return CCubeTexture::A16B16G16R16F;
	else if(FormatType=="A8R8G8B8")
		return CCubeTexture::A8R8G8B8;
	else if(FormatType=="R8G8B8")
		return CCubeTexture::R8G8B8;
	else if(FormatType=="X8R8G8B8")
		return CCubeTexture::X8R8G8B8;
	else
	{
		//Info("Format Type '%s' not recognized", FormatType.c_str());
		std::string msg_error = "CTexture::FormatType->Error, tipo de formato no reconocido: " + FormatType;
		LOGGER->AddNewLog(ELL_ERROR, msg_error.c_str());
		return CCubeTexture::A8R8G8B8;
		//throw CException(__FILE__, __LINE__, msg_error);
	}
}

bool CCubeTexture::Create(const std::string &Name, unsigned int Size, unsigned int MipMaps, TUsageType UsageType, TPoolType PoolType, TFormatType FormatType)
{
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
	
	// creating depth cube
	if (FAILED(D3DXCreateCubeTexture(CORE->GetRenderManager()->GetDevice(), Size, 1, l_UsageType, l_Format, l_Pool, &m_CubeTexture)))
		return false;
	
	if(l_CreateDepthStencilSurface)
	{
		CORE->GetRenderManager()->GetDevice()->CreateDepthStencilSurface(Size, Size, D3DFMT_D24S8,D3DMULTISAMPLE_NONE, 0, TRUE,&m_DepthStencilRenderTargetTexture, NULL);
		assert(m_DepthStencilRenderTargetTexture!=NULL);
	}

	// getting each six surfaces of the cubic depth map
	if (FAILED(m_CubeTexture->GetCubeMapSurface(D3DCUBEMAP_FACE_POSITIVE_X, 0, &cubeFacePX)))
		return false;
	if (FAILED(m_CubeTexture->GetCubeMapSurface(D3DCUBEMAP_FACE_POSITIVE_Y, 0, &cubeFacePY)))
		return false;
	if (FAILED(m_CubeTexture->GetCubeMapSurface(D3DCUBEMAP_FACE_POSITIVE_Z, 0, &cubeFacePZ)))
		return false;
	if (FAILED(m_CubeTexture->GetCubeMapSurface(D3DCUBEMAP_FACE_NEGATIVE_X, 0, &cubeFaceNX)))
		return false;
	if (FAILED(m_CubeTexture->GetCubeMapSurface(D3DCUBEMAP_FACE_NEGATIVE_Y, 0, &cubeFaceNY)))
		return false;
	if (FAILED(m_CubeTexture->GetCubeMapSurface(D3DCUBEMAP_FACE_NEGATIVE_Z, 0, &cubeFaceNZ)))
		return false;

	return true;
}



