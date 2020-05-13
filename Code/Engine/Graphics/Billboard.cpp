#include "Billboard.h"
#include "Camera\Camera.h"
#include "VertexType.h"
#include "Color.h"
#include "RenderManager.h"
#include "Camera\Frustum.h"
#include "Texture.h"
#include "Core.h"
#include "XML\XMLTreeNode.h"
#include "TextureManager.h"
#include "Base.h"


CBillboard::CBillboard(CXMLTreeNode &parser)
{
	SetColor(CColor(parser.GetVect3fProperty("color",Vect3f(1.0f))));
	m_fHeight=parser.GetFloatProperty("height",2.0f);
	m_fWidth=parser.GetFloatProperty("width",2.0f);

	std::string l_TexturePath=parser.GetPszProperty("tex","");
	CTexture *l_Texture = CORE->GetTextureManager()->GetTexture(l_TexturePath);

	SetTexture(l_Texture);
	SetPos(Vect3f(0.0f,0.0f,0.0f));
}

CBillboard::CBillboard(const CBillboard & other): m_Texture(NULL),m_Pos(Vect3f()),m_Rot(Mat33f())
{
	m_Texture = other.m_Texture;
	m_fWidth = other.m_fWidth;
	m_fHeight = other.m_fHeight;

	m_Rot.SetIdentity();
	m_Color = other.m_Color;
}


void CBillboard::Render (CRenderManager * RenderManager, const CFrustum* Frustum)
{
	float l_radius = m_PointA.Distance(m_Pos);
	if(!Frustum->SphereVisible(m_Pos, l_radius))
		return;

	TTEXTURE1_COLORED_VERTEX l_Points[4];

   	 unsigned short l_Indexes[6]={0,1,2, 1,3,2}; //A,C,B  B,C,D
	 unsigned long l_Color=D3DCOLOR_COLORVALUE(m_Color.GetRed(), m_Color.GetGreen(), m_Color.GetBlue(), m_Color.GetAlpha());

   	 l_Points[0].x=m_PointA.x;
   	 l_Points[0].y=m_PointA.y;
   	 l_Points[0].z=m_PointA.z;
   	 l_Points[0].u1=0.0f;
   	 l_Points[0].v1=0.0f;
	 l_Points[0].color=l_Color;
   	 
   	 l_Points[1].x=m_PointB.x;
   	 l_Points[1].y=m_PointB.y;
   	 l_Points[1].z=m_PointB.z;
   	 l_Points[1].u1=1.0f;
   	 l_Points[1].v1=0.0f;
   	 l_Points[1].color=l_Color;

   	 l_Points[2].x=m_PointC.x;
   	 l_Points[2].y=m_PointC.y;
   	 l_Points[2].z=m_PointC.z;
   	 l_Points[2].u1=0.0f;
   	 l_Points[2].v1=1.0f;
   	 l_Points[2].color=l_Color;

   	 l_Points[3].x=m_PointD.x;
   	 l_Points[3].y=m_PointD.y;
   	 l_Points[3].z=m_PointD.z;
   	 l_Points[3].u1=1.0f;
   	 l_Points[3].v1=1.0f;   	 
   	 l_Points[3].color=l_Color;
	 
	 RenderManager->GetDevice()->SetTexture(0,m_Texture->GetTexture());
	 RenderManager->GetDevice()->SetFVF(TTEXTURE1_COLORED_VERTEX::GetFVF());
   	 RenderManager->GetDevice()->DrawIndexedPrimitiveUP(D3DPT_TRIANGLELIST,0,6,2,l_Indexes,D3DFMT_INDEX16,l_Points,sizeof(TTEXTURE1_COLORED_VERTEX));
}

void CBillboard::Update ()
{
	CCamera * l_Camera = CORE->GetCamera();
	if (l_Camera==NULL)
		return;

	Vect3f l_Up= (l_Camera->GetVecUp());
	Vect3f l_Right = l_Up^l_Camera->GetDirection();
	l_Right.Normalize();
	
	m_PointA = m_Pos + (l_Up * (m_fHeight/2.0f)) - (l_Right * (m_fWidth/2.0f));
	m_PointB = m_Pos + (l_Up * (m_fHeight/2.0f)) + (l_Right * (m_fWidth/2.0f));
	m_PointC = m_Pos - (l_Up * (m_fHeight/2.0f)) - (l_Right * (m_fWidth/2.0f));
	m_PointD = m_Pos - (l_Up * (m_fHeight/2.0f)) + (l_Right * (m_fWidth/2.0f));
}

void CBillboard::UpdateAxisBillboard(Vect3f Forward)
{
	CCamera * l_Camera = CORE->GetCamera();
	if (l_Camera==NULL)
		return;
	
	Vect3f  l_Right= Forward^(l_Camera->GetDirection());
	l_Right.Normalize();

	Vect3f  l_Perpendicular= Forward;
	m_PointA = m_Pos + (l_Perpendicular * (m_fHeight/2.0f)) - (l_Right * (m_fWidth/2.0f));
	m_PointB = m_Pos + (l_Perpendicular * (m_fHeight/2.0f)) + (l_Right * (m_fWidth/2.0f));
	m_PointC = m_Pos - (l_Perpendicular * (m_fHeight/2.0f)) - (l_Right * (m_fWidth/2.0f));
	m_PointD = m_Pos - (l_Perpendicular * (m_fHeight/2.0f)) + (l_Right * (m_fWidth/2.0f));
}

void CBillboard::UpdateImpactBillboard()
{
	CCamera * l_Camera = CORE->GetCamera();
	if (l_Camera==NULL)
		return;

	float l_Yaw = l_Camera->GetPlayerYaw();
	Vect3f l_Right = Vect3f((float)sin(l_Yaw),0.0f,(float)cos(l_Yaw));
	l_Right.Normalize();
	Vect3f l_Up = Vect3f((float)sin(l_Yaw-FLOAT_PI_VALUE/2.f), (float)0.0f, (float)cos(l_Yaw-FLOAT_PI_VALUE/2.f));
	l_Up.Normalize();

	if (GetPos().y <GROUND_HEIGHT) 
	{
		Vect3f l_Pos = GetPos();
		l_Pos.y = 0.f;
		SetPos(l_Pos);
		m_PointA = m_Pos + (l_Up * (m_fHeight/2.0f)) - (l_Right * (m_fWidth/2.0f));
		m_PointB = m_Pos + (l_Up * (m_fHeight/2.0f)) + (l_Right * (m_fWidth/2.0f));
		m_PointC = m_Pos - (l_Up * (m_fHeight/2.0f)) - (l_Right * (m_fWidth/2.0f));
		m_PointD = m_Pos - (l_Up * (m_fHeight/2.0f)) + (l_Right * (m_fWidth/2.0f));
	}
	else
	{
		Vect3f l_Pos = GetPos();
		l_Pos.y = 2.95f;
		SetPos(l_Pos);
		m_PointC = m_Pos + (l_Up * (m_fHeight/2.0f)) - (l_Right * (m_fWidth/2.0f));
		m_PointD = m_Pos + (l_Up * (m_fHeight/2.0f)) + (l_Right * (m_fWidth/2.0f));
		m_PointA = m_Pos - (l_Up * (m_fHeight/2.0f)) - (l_Right * (m_fWidth/2.0f));
		m_PointB = m_Pos - (l_Up * (m_fHeight/2.0f)) + (l_Right * (m_fWidth/2.0f));
	}
}

void CBillboard::SetPos (Vect3f &Pos)
{
	m_Pos= Pos;
}

void CBillboard::SetFirstPos (Vect3f &Pos)
{
	m_FirstPos= Pos;
}

void CBillboard::SetTexture (CTexture* Texture)
{
	m_Texture= Texture;
}



