#include "RenderManager.h"
#include <cassert>
#include "Color.h"
#include "Logger.h"
#include "Exception.h"
#include "Math\MathUtils.h"
#include "Matrix44.h"
#include "Camera\Camera.h"
#include "EffectManager.h"
#include "Texture.h"
#include "Core.h"
#include "VertexType.h"
#include <assert.h>
#include "Base.h"

void CRenderManager::Done()
{
	if (m_bIsOk)
	{
		Release();
		m_bIsOk=false;
	}
}

void CRenderManager::Release()
{
	CHECKED_RELEASE(m_pD3D);
	CHECKED_RELEASE(m_pD3DDevice);
}

bool CRenderManager::Init(HWND hWnd, const SInitParams& params)
{
	// Create the D3D object.
	m_pD3D = Direct3DCreate9( D3D_SDK_VERSION );
	assert(m_pD3D);

	m_bIsOk = m_pD3D != NULL;

	if (m_bIsOk)
	{
		// Set up the structure used to create the D3DDevice
		D3DPRESENT_PARAMETERS d3dpp;
		ZeroMemory( &d3dpp, sizeof(d3dpp) );

		if(params.fullscreen)
		{
			d3dpp.Windowed          = FALSE;
			d3dpp.BackBufferWidth   = params.w;
			d3dpp.BackBufferHeight  = params.h;
			d3dpp.BackBufferFormat  = D3DFMT_A8R8G8B8;
			d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
		}
		else
		{
			d3dpp.Windowed          = TRUE;
			//d3dpp.BackBufferFormat	= D3DFMT_UNKNOWN;
			d3dpp.BackBufferFormat  = D3DFMT_A8R8G8B8; 
		}

		d3dpp.SwapEffect				= D3DSWAPEFFECT_FLIP;
		d3dpp.EnableAutoDepthStencil	= TRUE;
		d3dpp.AutoDepthStencilFormat	= D3DFMT_D24S8;
		d3dpp.Flags						= D3DPRESENTFLAG_DISCARD_DEPTHSTENCIL;
		if (params.vsync)
			d3dpp.PresentationInterval		= D3DPRESENT_INTERVAL_ONE;
		else
			d3dpp.PresentationInterval		= D3DPRESENT_INTERVAL_IMMEDIATE;

		// Create the D3DDevice
#if MULTITHREADED_LOAD		
		m_bIsOk = !FAILED(    m_pD3D->CreateDevice(    D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd,
			D3DCREATE_HARDWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED, &d3dpp, &m_pD3DDevice ) );
#else
		m_bIsOk = !FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd,
			D3DCREATE_HARDWARE_VERTEXPROCESSING, &d3dpp, &m_pD3DDevice));
#endif

		if (!m_bIsOk)
		{
			m_bIsOk = !FAILED(    m_pD3D->CreateDevice(    D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd,
				D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &m_pD3DDevice ) );

			if (m_bIsOk)
			{
				LOGGER->AddNewLog(ELOG_LEVEL::ELL_INFORMATION, "RenderManager:: D3DCREATE_SOFTWARE_VERTEXPROCESSING");
			}
		}
		else
		{
			LOGGER->AddNewLog(ELOG_LEVEL::ELL_INFORMATION, "RenderManager:: D3DCREATE_HARDWARE_VERTEXPROCESSING");
		}

		if (m_bIsOk)
		{
			// Turn off culling, so we see the front and back of the triangle
			m_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
			m_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
			m_pD3DDevice->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);
			m_pD3DDevice->SetSamplerState(1, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
			m_pD3DDevice->SetSamplerState(1, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
			m_pD3DDevice->SetSamplerState(1, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

			m_pD3DDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_CCW );
			m_pD3DDevice->SetRenderState( D3DRS_ZENABLE, TRUE );
			m_pD3DDevice->SetRenderState( D3DRS_ZWRITEENABLE, TRUE );
			m_pD3DDevice->SetRenderState( D3DRS_ZFUNC, D3DCMP_LESSEQUAL );


			//m_pD3DDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_CCW);
			//m_pD3DDevice->SetRenderState( D3DRS_ZENABLE,D3DZB_TRUE);
			//m_pD3DDevice->SetRenderState( D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
			//m_pD3DDevice->SetRenderState( D3DRS_ZWRITEENABLE, TRUE);

			// Turn off D3D lighting, since we are providing our own vertex colors
			m_pD3DDevice->SetRenderState( D3DRS_LIGHTING, FALSE );

			if (params.fullscreen)
			{
				m_uWidth    = params.w;
				m_uHeight    = params.h;
			}
			else
			{
				GetWindowRect(hWnd);
			}
			LOGGER->AddNewLog(ELOG_LEVEL::ELL_INFORMATION, "RenderManager:: La resolucion de pantalla es (%dx%d)",m_uWidth,m_uHeight);

		}
	}

	if (!m_bIsOk)
	{
		std::string msg_error = "Rendermanager::Init-> Error al inicializar Direct3D";
		LOGGER->AddNewLog(ELOG_LEVEL::ELL_ERROR, msg_error.c_str());
		Release();
		throw CException(__FILE__, __LINE__, msg_error);
	}

	return m_bIsOk;
}

#if 0
void CRenderManager::BeginRendering()
{
#ifdef _DEBUG // Clear the backbuffer to a blue color in a Debug mode
	uint32 red   	 = (uint32) (m_BackbufferColor_debug.GetRed() * 255);		
	uint32 green     = (uint32) (m_BackbufferColor_debug.GetGreen() * 255);
	uint32 blue   	 = (uint32) (m_BackbufferColor_debug.GetBlue()* 255);
	m_pD3DDevice->Clear( 0, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(red, green, blue), 1.0f, 0 );
#else // Clear the backbuffer to a black color in a Release mode
	uint32 red   	 = (uint32) (m_BackbufferColor_release.GetRed() * 255);
	uint32 green    = (uint32) (m_BackbufferColor_release.GetGreen() * 255);
	uint32 blue   	 = (uint32) (m_BackbufferColor_release.GetBlue()* 255);
	m_pD3DDevice->Clear( 0, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(red, green, blue), 1.0f, 0 );
#endif

	// Begin the scene
	HRESULT hr=m_pD3DDevice->BeginScene();
	assert( SUCCEEDED( hr ) );
	m_pD3DDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_CCW);
	m_pD3DDevice->SetRenderState( D3DRS_ZENABLE,D3DZB_TRUE);
	m_pD3DDevice->SetRenderState( D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
	m_pD3DDevice->SetRenderState( D3DRS_ZWRITEENABLE, TRUE);
	m_pD3DDevice->SetRenderState( D3DRS_LIGHTING, FALSE );
	m_pD3DDevice->SetRenderState( D3DRS_DITHERENABLE, TRUE );
	m_pD3DDevice->SetRenderState( D3DRS_SPECULARENABLE, FALSE );

	if(m_bPaintSolid)
	{
		m_pD3DDevice->SetRenderState( D3DRS_FILLMODE, D3DFILL_SOLID  );
	}
	else
	{
		m_pD3DDevice->SetRenderState( D3DRS_FILLMODE, D3DFILL_WIREFRAME  );
	}
}

void CRenderManager::EndRendering()
{
	m_pD3DDevice->EndScene();
	// Present the backbuffer contents to the display
	m_pD3DDevice->Present( NULL, NULL, NULL, NULL );
}
#endif

void CRenderManager::Clear(bool m_Color, bool m_Depth, bool m_Stencil, DWORD Col)
{
	DWORD m_ClearMask= (m_Color ? D3DCLEAR_TARGET : 0) | 
		(m_Depth ? D3DCLEAR_ZBUFFER : 0) |
		(m_Stencil ? D3DCLEAR_STENCIL : 0);

	m_pD3DDevice->Clear( 0, NULL, m_ClearMask, D3DCOLOR(Col), 1.0f, 0 );
}

//-----------------------------------------------------------------------------
// Name: SetupMatrices()
// Desc: Sets up the world, view, and projection transform matrices.
//-----------------------------------------------------------------------------
void CRenderManager::SetupMatrices(CCamera* camera)
{
	D3DXMATRIX m_matView;
	D3DXMATRIX m_matProject;

	if(!camera)
	{
		//Set default view and projection matrix

		//Setup Matrix projection
		D3DXMatrixPerspectiveFovLH( &m_matProject, 45.0f * D3DX_PI / 180.0f, (float)m_uWidth/(float)m_uHeight, 1.0f, 100.0f );

		//Setup Matrix view
		//D3DXVECTOR3 l_Eye(5.0f,5.0f,5.0f), l_LookAt(0.0f,0.0f,0.0f), l_VUP(0.0f,1.0f,0.0f);
		D3DXVECTOR3 l_Eye(-10.0f,3.0f,0.0f), l_LookAt(0.0f,0.0f,0.0f), l_VUP(0.0f,1.0f,0.0f);
		D3DXMatrixLookAtLH( &m_matView, &l_Eye, &l_LookAt, &l_VUP);
	}
	else{
		Vect3f eye = camera->GetEye();
		D3DXVECTOR3 l_Eye(eye.x, eye.y, eye.z);
		Vect3f lookat = camera->GetLookAt();
		D3DXVECTOR3 l_LookAt(lookat.x, lookat.y, lookat.z);
		Vect3f vup = camera->GetVecUp();
		D3DXVECTOR3 l_VUP(vup.x, vup.y, vup.z);
		//Setup Matrix view
		D3DXMatrixLookAtLH( &m_matView, &l_Eye, &l_LookAt, &l_VUP);
		//Setup Matrix projection
		D3DXMatrixPerspectiveFovLH(	&m_matProject, camera->GetFov(), camera->GetAspectRatio(), camera->GetZn(), camera->GetZf());
	}

	//Setup Matrix projection
	m_Frustum.Update( m_matView * m_matProject );
	m_pD3DDevice->SetTransform( D3DTS_VIEW, &m_matView );
	m_pD3DDevice->SetTransform( D3DTS_PROJECTION, &m_matProject );

	CORE->GetEffectManager()->ActivateCamera(Mat44f(m_matView), Mat44f(m_matProject),camera->GetEye());
}

void CRenderManager::SetTransform	(D3DXMATRIX& matrix)
{
	CORE->GetEffectManager()->SetWorldMatrix(Mat44f(matrix));
	m_pD3DDevice->SetTransform(D3DTS_WORLD, &matrix);
}

void CRenderManager::SetTransform	(Mat44f& m)
{
	D3DXMATRIX aux(	m.m00, m.m10, m.m20, m.m30	, m.m01, m.m11, m.m21, m.m31
		, m.m02, m.m12, m.m22, m.m32, m.m03, m.m13, m.m23, m.m33);
	m_pD3DDevice->SetTransform(D3DTS_WORLD, &aux);
	CORE->GetEffectManager()->SetWorldMatrix(m);
}

void CRenderManager::GetWindowRect(HWND hwnd)
{
	RECT rec_window;
	GetClientRect(    hwnd, &rec_window);
	m_uWidth = rec_window.right - rec_window.left;
	m_uHeight = rec_window.bottom - rec_window.top;
}

void CRenderManager::DrawLine ( const Vect3f &PosA, const Vect3f &PosB, CColor Color)
{
	DWORD color_aux = Color.GetUint32Argb();

	CUSTOMVERTEX v[2] =
	{
		{PosA.x, PosA.y, PosA.z, color_aux},
		{PosB.x, PosB.y, PosB.z, color_aux},
	};

	m_pD3DDevice->SetTexture(0,NULL);
	m_pD3DDevice->SetFVF(CUSTOMVERTEX::getFlags());
	m_pD3DDevice->DrawPrimitiveUP( D3DPT_LINELIST,1, v,sizeof(CUSTOMVERTEX));
}

void CRenderManager::DrawAxis ( float size)
{
	DrawLine(Vect3f(0,0,0), Vect3f(0,size,0), colGREEN);
	DrawLine(Vect3f(0,0,0), Vect3f(size,0,0), colRED);
	DrawLine(Vect3f(0,0,0), Vect3f(0,0,size), colBLUE);
}

void CRenderManager::DrawGrid (float sizeV, float sizeH, uint16 num)
{
	for(int i=0; i<num; i++)
	{
		DrawLine(Vect3f( -sizeV/2 + (sizeV*i)/(num-1) , 0, -sizeH/2), Vect3f(-sizeV/2 + (sizeV*i)/(num-1) , 0, sizeH/2), colGRAY);
	}

	for(int i=0; i<num; i++)
	{
		DrawLine(Vect3f( -sizeV/2, 0, -sizeH/2 + (sizeH*i)/(num-1)), Vect3f( sizeV/2, 0, -sizeH/2 + (sizeH*i)/(num-1)), colGRAY);
	}
}

void CRenderManager::DrawPlane(float size, const Vect3f& normal, float distance, CColor Color, int GridX, int GridZ )
{
	// assert(IsOk());
	D3DXMATRIX matrix;
	D3DXMatrixIdentity(&matrix);
	m_pD3DDevice->SetTransform(D3DTS_WORLD, &matrix);

	//Ax + By + Cz + D = 0
	//if (x,y) = (0,0) then z = -D/C
	//if (x,y) = (1,1) then z = (-D-A-B)/C
	//With two points we have a vector-->

	float A,B,C,D;
	A = normal.x;
	B = normal.y;
	C = normal.z;
	D = distance;
	//float pointAz;
	//float pointBz;

	Vect3f pointA, pointB;
	if( C!= 0)
	{
		pointA = Vect3f(0.f,0.f, -D/C);
		pointB = Vect3f(1.f,1.f, (D-A-B)/C);
	}
	else if( B!= 0)
	{
		pointA = Vect3f(0.f,-D/B, 0.f);
		pointB = Vect3f(1.f,(-D-A-C)/B,1.f);
	}
	else if( A != 0)
	{
		pointA = Vect3f(-D/A,0.f, 0.f);
		pointB = Vect3f((-D-B-C)/A,1.f,1.f);
	}
	else
	{
		//error.
	}


	Vect3f vectorA = pointB - pointA;
	vectorA.Normalize();
	Vect3f vectorB;
	vectorB = normal^vectorA;
	vectorB.Normalize();
	Vect3f initPoint = normal*distance;

	assert(GridX>0);
	assert(GridZ>0);
	//LINEAS EN Z
	Vect3f initPointA = initPoint - vectorB*size*0.5;
	for(int b=0;b<=GridX;++b)
	{   	 
		DrawLine(initPointA + vectorA*size*0.5, initPointA - vectorA*size*0.5, Color );

		initPointA += vectorB*size/(float)GridX;
	}
	initPointA = initPoint - vectorA*size*0.5;
	for(int b=0;b<=GridX;++b)
	{
		DrawLine(initPointA + vectorB*size*0.5, initPointA - vectorB*size*0.5, Color);
		initPointA += vectorA*size/(float)GridX;
	}
}

void CRenderManager::DrawCube ( const Vect3f &size, CColor Color)
{
	DrawLine(Vect3f(size.x/2, -size.y/2, size.z/2), Vect3f(size.x/2, size.y/2, size.z/2), Color);
	DrawLine(Vect3f(-size.x/2, -size.y/2, size.z/2), Vect3f(-size.x/2, size.y/2, size.z/2), Color);
	DrawLine(Vect3f(size.x/2, -size.y/2, -size.z/2), Vect3f(size.x/2, size.y/2, -size.z/2), Color);
	DrawLine(Vect3f(-size.x/2, -size.y/2, -size.z/2), Vect3f(-size.x/2, size.y/2, -size.z/2), Color);

	DrawLine(Vect3f(size.x/2, -size.y/2, size.z/2), Vect3f(size.x/2, -size.y/2, -size.z/2), Color);
	DrawLine(Vect3f(size.x/2, -size.y/2, -size.z/2), Vect3f(-size.x/2, -size.y/2, -size.z/2), Color);
	DrawLine(Vect3f(-size.x/2, -size.y/2, -size.z/2), Vect3f(-size.x/2, -size.y/2, size.z/2), Color);
	DrawLine(Vect3f(-size.x/2, -size.y/2, size.z/2), Vect3f(size.x/2, -size.y/2, size.z/2), Color);

	DrawLine(Vect3f(size.x/2, size.y/2, size.z/2), Vect3f(size.x/2, size.y/2, -size.z/2), Color);
	DrawLine(Vect3f(size.x/2, size.y/2, -size.z/2), Vect3f(-size.x/2, size.y/2, -size.z/2), Color);
	DrawLine(Vect3f(-size.x/2, size.y/2, -size.z/2), Vect3f(-size.x/2, size.y/2, size.z/2), Color);
	DrawLine(Vect3f(-size.x/2, size.y/2, size.z/2), Vect3f(size.x/2, size.y/2, size.z/2), Color);
}

void CRenderManager::DrawCircle(float Radius, float coordY, CColor Color, uint16 num)
{
	for(int i=0; i<num; i++)
	{
		DrawLine(Vect3f((float)(Radius*(cos(i*e2PIf/num))), (float)coordY, (float)(Radius*(sin(i*e2PIf/num)))), Vect3f((float)(Radius*(cos((i+1)*e2PIf/num))), (float)coordY, (float)(Radius*(sin((i+1)*e2PIf/num)))), Color);
	}
}

void CRenderManager::DrawElipse(float a, float b, float coordY, CColor Color, uint16 num)
{
	for(int i=0; i<=num;++i)
	{
		float O_0 = 2*ePIf*((float)i/(float)num);
		float d_0 = (float)sqrt( ((cos(O_0)*cos(O_0))/(a*a)) + ((sin(O_0)*sin(O_0))/(b*b)));
		float O_1 = 2*ePIf*((float)(i+1)/(float)num);
		float d_1 = (float)sqrt( ((cos(O_1)*cos(O_1))/(a*a)) + ((sin(O_1)*sin(O_1))/(b*b)));

		DrawLine(Vect3f((float)cos(O_0) / (float)d_0, (float)coordY, (float)sin(O_0) / d_0), Vect3f((float)cos(O_1) / (float)d_1, (float)coordY, (float)sin(O_1) / (float)d_1), Color);
	}
}

void CRenderManager::DrawSphere(float radius, uint32 edges, CColor color )
{
	for(int t=0;t<static_cast<int>(edges);++t)
	{
		float l_radiusRing= (float)(radius*sin(mathUtils::Deg2Rad<float>(180.0f*((float)t))/((float)edges)));
		for(int b=0;b<static_cast<int>(edges);++b)
		{   	    
			Vect3f l_PosA((float)(l_radiusRing*cos(mathUtils::Deg2Rad<float>((float)(360.0f*(float)b)/((float)edges)))), (float)(radius*cos(mathUtils::Deg2Rad<float>(180.0f*((float)t))/((float)edges))), (float)(l_radiusRing*sin(mathUtils::Deg2Rad<float>((float)(360.0f*(float)b)/((float)edges)))));
			Vect3f l_PosB((float)(l_radiusRing*cos(mathUtils::Deg2Rad<float>((float)(360.0f*(float)(b+1))/((float)edges)))), (float)(radius*cos(mathUtils::Deg2Rad<float>(180.0f*((float)t))/((float)edges))),(float)(l_radiusRing*sin(mathUtils::Deg2Rad<float>((float)(360.0f*(float)(b+1))/((float)edges)))));
			DrawLine(l_PosA,l_PosB,color);

			float l_radiusNextRing= (float)(radius*sin(mathUtils::Deg2Rad<float>(180.0f*((float)(t+1)))/((float)edges)));

			Vect3f l_PosC((float)(l_radiusRing*cos(mathUtils::Deg2Rad<float>((float)(360.0f*(float)b)/((float)edges)))), (float)(radius*cos(mathUtils::Deg2Rad<float>(180.0f*((float)t))/((float)edges))), (float)(l_radiusRing*sin(mathUtils::Deg2Rad<float>((float)(360.0f*(float)b)/((float)edges)))));
			Vect3f l_PosD((float)(l_radiusNextRing*cos(mathUtils::Deg2Rad<float>((float)(360.0f*(float)b)/((float)edges)))), (float)(radius*cos(mathUtils::Deg2Rad<float>(180.0f*((float)(t+1)))/((float)edges))), (float)(l_radiusNextRing*sin(mathUtils::Deg2Rad<float>((float)(360.0f*(float)b)/((float)edges)))));
			DrawLine(l_PosC,l_PosD,color);
		}
	}
}

void CRenderManager::DrawCone (float radius , float h, CColor color , uint16 num){
	float rad;
	Vect3f ExtremoSup(0.0,h,0.0);
	for(int i=0; i<=num;++i){
		rad=(num-i)*(radius/num);
		DrawCircle(rad,i*h/num,color,30);


	}
	num=2*num;
	for(int i=0; i<=num;++i){
		Vect3f ExtremoInf((float)(radius*cos(mathUtils::Deg2Rad<float>((float)(360.0f)*(float)i/((float)num)))), (float)0.0f, (float)(radius*sin(mathUtils::Deg2Rad<float>((float)(360.0f)*(float)i/((float)num)))));
		DrawLine(ExtremoSup,ExtremoInf,color);
	}
}

void CRenderManager::DrawConeInv (float radius , float h, CColor color , uint16 num){
	float rad;
	Vect3f Vertice(0.0,0.0,0.0);
	for(int i=0; i<=num;++i){
		rad=(num-i)*(radius/num);
		DrawCircle(rad,i*(-h)/num +h,color,30);


	}
	num=2*num;
	for(int i=0; i<=num;++i){
		Vect3f ExtremoInf((float)(radius*cos(mathUtils::Deg2Rad<float>((float)(360.0f)*(float)i/((float)num)))), (float)h, (float)(radius*sin(mathUtils::Deg2Rad<float>((float)(360.0f)*(float)i/((float)num)))));
		DrawLine(Vertice,ExtremoInf,color);
	}
}

void CRenderManager::DrawCilinder (float radius , float h, CColor color , uint16 num){

	for(int i=0; i<=num;++i){
		DrawCircle(radius,i*(-h)/num +h,color,30);		
	}

	num=2*num;
	for(int i=0; i<=num;++i){
		Vect3f ExtremoSup((float)(radius*cos(mathUtils::Deg2Rad<float>((float)(360.0f)*(float)i/((float)num)))), (float)h, (float)(radius*sin(mathUtils::Deg2Rad<float>((float)(360.0f)*(float)i/((float)num)))));
		Vect3f ExtremoInf(ExtremoSup[0],0.0,ExtremoSup[2]);
		DrawLine(ExtremoInf,ExtremoSup,color);
	}
}


void CRenderManager::DrawQuad2D (const Vect2i& pos, uint32 w, uint32 h, ETypeAlignment alignment, CColor color)
{
	Vect2i finalPos = pos;
	CalculateAlignment(w, h, alignment, finalPos);

	// finalPos = [0]
	//
	//  [0]------[2]
	//   |	      |
	//   |	      |
	//   |	      |
	//  [1]------[3]


	unsigned short indices[6]={0,2,1,1,2,3};
	SCREEN_COLOR_VERTEX v[4] =
	{
		{ (float)finalPos.x,		(float)finalPos.y,		0,1, D3DCOLOR_COLORVALUE(color.GetRed(), color.GetGreen(), color.GetBlue(), color.GetAlpha())} //(x,y) sup_esq.
		,	{ (float)finalPos.x,		(float)finalPos.y+h,	0,1, D3DCOLOR_COLORVALUE(color.GetRed(), color.GetGreen(), color.GetBlue(), color.GetAlpha())} //(x,y) inf_esq.
		, { (float)finalPos.x+w,	(float)finalPos.y,		0,1, D3DCOLOR_COLORVALUE(color.GetRed(), color.GetGreen(), color.GetBlue(), color.GetAlpha())} //(x,y) sup_dr.
		,	{ (float)finalPos.x+w,	(float)finalPos.y+h,	0,1, D3DCOLOR_COLORVALUE(color.GetRed(), color.GetGreen(), color.GetBlue(), color.GetAlpha())} //(x,y) inf_dr.
	};

	m_pD3DDevice->SetFVF( SCREEN_COLOR_VERTEX::getFlags() );
	m_pD3DDevice->SetTexture(0, NULL);
	m_pD3DDevice->DrawIndexedPrimitiveUP(D3DPT_TRIANGLELIST,0,4,2,indices,D3DFMT_INDEX16,v,sizeof( SCREEN_COLOR_VERTEX ) );
}

void CRenderManager::DrawColoredQuad2DTexturedInPixels(RECT Rect, CColor Color, CTexture * Texture, float U0, float V0, float U1, float V1)
{

	//  [0]------[2]
	//   |	      |
	//   |	      |
	//   |        |
	//  [1]------[3]

	SCREEN_TEXTURE_COLORED_VERTEX v[4] =
	{
		{ (float)Rect.left,		(float)Rect.top,		0,1, D3DCOLOR_COLORVALUE(Color.GetRed(), Color.GetGreen(), Color.GetBlue(), Color.GetAlpha()), U0, V0} //(x,y) sup_esq.
		, { (float)Rect.right,	(float)Rect.top,		0,1, D3DCOLOR_COLORVALUE(Color.GetRed(), Color.GetGreen(), Color.GetBlue(), Color.GetAlpha()), U1, V0} //(x,y) sup_dr.
		,	{ (float)Rect.left,		(float)Rect.bottom,		0,1, D3DCOLOR_COLORVALUE(Color.GetRed(), Color.GetGreen(), Color.GetBlue(), Color.GetAlpha()), U0, V1} //(x,y) inf_esq.
		,	{ (float)Rect.right,	(float)Rect.bottom,	0,1, D3DCOLOR_COLORVALUE(Color.GetRed(), Color.GetGreen(), Color.GetBlue(), Color.GetAlpha()), U1, V1} //(x,y) inf_dr.
	};

	m_pD3DDevice->SetFVF( SCREEN_TEXTURE_COLORED_VERTEX::GetFVF() );
	if (Texture != NULL)
	{
		m_pD3DDevice->SetTexture(0, Texture->GetTexture());
	}
	
	m_pD3DDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP,2,v,sizeof( SCREEN_TEXTURE_COLORED_VERTEX ) );
}

void CRenderManager::CalculateAlignment (uint32 w, uint32 h, ETypeAlignment alignment, Vect2i & finalPos)
{
	switch (alignment)
	{
	case CENTER:
		{
			finalPos.x -= (uint32)(w*0.5f);
			finalPos.y -= (uint32)(h*0.5f);
		}
		break;
	case UPPER_LEFT:
		{
			//Por defecto ya est alienado de esta manera :)
		}
		break;
	case UPPER_RIGHT:
		{
			finalPos.x -= w;
		}
		break;
	case LOWER_RIGHT:
		{
			finalPos.x -= w;
			finalPos.y -= h;
		}
		break;
	case LOWER_LEFT:
		{
			finalPos.y -= h;
		}
		break;
	default:
		{
			LOGGER->AddNewLog(ELOG_LEVEL::ELL_ERROR, "RenderManager:: Se está intentado renderizar un quad2d con una alineacion desconocida");
		}
		break;
	}	
}

void CRenderManager::DrawRectangle2D (	const Vect2i& pos, uint32 w, uint32 h, CColor& backGroundColor,	uint32 edge_w, uint32 edge_h, CColor& edgeColor )
{
	//Draw background quad2D:
	DrawQuad2D(pos, w, h, UPPER_LEFT, backGroundColor);

	//Draw the four edges:

	//2 Horizontal:
	Vect2i pos_aux = pos;
	pos_aux.y -= edge_h;
	DrawQuad2D(pos_aux, w, edge_h, UPPER_LEFT, edgeColor);
	pos_aux = pos;
	pos_aux.y += h;
	DrawQuad2D(pos_aux, w, edge_h, UPPER_LEFT, edgeColor);

	//2 Vertical:
	pos_aux = pos;
	pos_aux.x -= edge_w;
	pos_aux.y -= edge_h;
	DrawQuad2D(pos_aux, edge_w, h + (2*edge_w), UPPER_LEFT, edgeColor);
	pos_aux.x = pos.x + w;
	DrawQuad2D(pos_aux, edge_w, h + (2*edge_w), UPPER_LEFT, edgeColor);	
}

void CRenderManager::DrawTexturedQuad2D (const Vect2i& pos, uint32 w, uint32 h, ETypeAlignment alignment, CTexture *Texture, CColor color)
{
  Vect2i finalPos = pos;
  CalculateAlignment(w, h, alignment, finalPos);
  color = CColor(1.0f, 1.0f, 1.0f, 1.0f);

  // finalPos = [0]
  //
  //  [0]------[2]
  //   |    	|
  //   |    	|
  //   |    	|
  //  [1]------[3]

  unsigned short indices[6]={0,2,1,1,2,3};
  SCREEN_TEXTURE_COLORED_VERTEX v[4] =
  {
	{ (float)finalPos.x,(float)finalPos.y,0,1, D3DCOLOR_COLORVALUE(color.GetRed(), color.GetGreen(), color.GetBlue(), color.GetAlpha()), 0.0f, 0.0f} //(x,y) sup_esq.
	,{ (float)finalPos.x,(float)finalPos.y+h,0,1, D3DCOLOR_COLORVALUE(color.GetRed(), color.GetGreen(), color.GetBlue(), color.GetAlpha()), 0.0f, 1.0f} //(x,y) inf_esq.
	,{ (float)finalPos.x+w,(float)finalPos.y,0,1, D3DCOLOR_COLORVALUE(color.GetRed(), color.GetGreen(), color.GetBlue(), color.GetAlpha()),1.0f, 0.0f} //(x,y) sup_dr.
	,{ (float)finalPos.x+w,(float)finalPos.y+h,0,1, D3DCOLOR_COLORVALUE(color.GetRed(), color.GetGreen(), color.GetBlue(), color.GetAlpha()),1.0f, 1.0f} //(x,y) inf_dr.
  };

  m_pD3DDevice->SetFVF( SCREEN_TEXTURE_COLORED_VERTEX::GetFVF());
  Texture->Activate(0);
 
  m_pD3DDevice->DrawIndexedPrimitiveUP( D3DPT_TRIANGLELIST,0, 4, 2,indices,D3DFMT_INDEX16, v, sizeof( SCREEN_TEXTURE_COLORED_VERTEX ) );
}

void CRenderManager::EnableAlphaBlend()
{
	//Activa el alpha
	m_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	m_pD3DDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
}

void CRenderManager::DisableAlphaBlend()
{
	//Desactiva el alpha
	m_pD3DDevice->SetRenderState(D3DRS_POINTSPRITEENABLE, FALSE);
	m_pD3DDevice->SetRenderState(D3DRS_POINTSCALEENABLE, FALSE);
	m_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	m_pD3DDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
}

void CRenderManager::DrawQuad2D (const Vect2i& pos, uint32 w, uint32 h, ETypeAlignment alignment, CTexture* texture, ETypeFlip flip)
{
   Vect2i finalPos = pos;
   CalculateAlignment(w, h, alignment,finalPos);

   // finalPos = [0]
   //
   //  [0]------[2]
   //   |    	|
   //   |    	|
   //   |    	|
   //  [1]------[3]
   
   Vect2f coord_text[4];
   uint32 w_aux, h_aux;
   switch(flip)
   {
   case NONE_FLIP:
   	coord_text[0].x = 0.f;	coord_text[0].y = 0.f;
   	coord_text[1].x = 0.f;	coord_text[1].y = 1.f;
   	coord_text[2].x = 1.f;	coord_text[2].y = 0.f;
   	coord_text[3].x = 1.f;	coord_text[3].y = 1.f;
   	break;
   case FLIP_X:
   	//Pintamos primero el quad de la izquierda:
   	w_aux = (uint32)(w*0.5);
   	DrawQuad2D(finalPos, w_aux, h, UPPER_LEFT, texture, NONE_FLIP);
   	finalPos.x += w_aux;
   	w = w - w_aux;
   	coord_text[0].x = 0.99f;	coord_text[0].y = 0.f;
   	coord_text[1].x = 0.99f;	coord_text[1].y = 1.f;
   	coord_text[2].x = 0.f;    	coord_text[2].y = 0.f;
   	coord_text[3].x = 0.f;    	coord_text[3].y = 1.f;
   	break;
   case FLIP_Y:
   	h_aux = (uint32)(h*0.5);
   	DrawQuad2D(finalPos, w, h_aux, UPPER_LEFT, texture, NONE_FLIP);
   	finalPos.y += h_aux + 1;
   	h = h - h_aux -1;
   	coord_text[0].x = 1.f;	coord_text[0].y = 0.f;
   	coord_text[1].x = 1.f;	coord_text[1].y = 1.f;
   	coord_text[2].x = 0.f;	coord_text[2].y = 0.f;
   	coord_text[3].x = 0.f;	coord_text[3].y = 1.f;
   	break;
   default:
   	{
       	LOGGER->AddNewLog(ELOG_LEVEL::ELL_ERROR, "RenderManager:: Se está intentado renderizar un quad2d con un flip desconocido");
   	}
  	 
   }
   
   
   unsigned short indices[6]={0,2,1,1,2,3};
   SCREEN_TEXTURE_VERTEX v[4] =
   {
       	{ (float)finalPos.x,    	(float)finalPos.y,    	0,1, coord_text[0].x,	coord_text[0].y} //(x,y) sup_esq.
   	,	{ (float)finalPos.x,    	(float)finalPos.y+h,	0,1,  coord_text[1].x,	coord_text[1].y} //(x,y) inf_esq.
   	, { (float)finalPos.x+w,	(float)finalPos.y,    	0,1,  coord_text[2].x,	coord_text[2].y} //(x,y) sup_dr.
   	,	{ (float)finalPos.x+w,	(float)finalPos.y+h,	0,1,  coord_text[3].x,	coord_text[3].y} //(x,y) inf_dr.
   };


   m_pD3DDevice->SetFVF( SCREEN_TEXTURE_VERTEX::GetFVF());
   m_pD3DDevice->SetTexture(0, texture->GetTexture());
   m_pD3DDevice->DrawIndexedPrimitiveUP( D3DPT_TRIANGLELIST,0, 4, 2,indices,D3DFMT_INDEX16, v, sizeof( SCREEN_TEXTURE_VERTEX ) );

}