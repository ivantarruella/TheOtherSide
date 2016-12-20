#pragma once

#include "Vector3.h"
#include "Math\Matrix33.h"


#include "Texture.h"
#include "Color.h"

class CRenderManager;
class CCamera;
class CXMLTreeNode;
class CFrustum;

#define GROUND_HEIGHT 0.2f
#define CEILING_HEIGHT 2.8f



class CBillboard
{
public:
	CBillboard(void): m_Texture(NULL),
		m_Pos(Vect3f()),
		m_fWidth(4.0),
		m_fHeight(4.0),
		m_Rot(Mat33f()),
		m_Color(colWHITE)
	{

			
			m_Rot.SetIdentity();

	}

	CBillboard(const CBillboard & other);
	CBillboard(CXMLTreeNode &parser);
	~CBillboard(void){}

	//Puede tener una textura de atributo o la pasamos cada vez
	// que tenga x,y,z y color
	virtual void Render (CRenderManager * RenderManager, const CFrustum* Frustum);
	//calcular los 4 vertices segun la altura de la camra 
	void Update ();//,Mat33f & Rot);
	void UpdateAxisBillboard(Vect3f Forward);
	void UpdateImpactBillboard();
	void SetPos (Vect3f &Pos);
	void SetFirstPos (Vect3f &Pos);
	Vect3f& GetPos(){ return m_Pos;}
	Vect3f& GetFirstPos(){ return m_FirstPos;}
	void SetTexture ( CTexture * Texture);
	CTexture * GetTexture(){ return m_Texture;}
	void SetWidth(float Width){ m_fWidth=Width;}
	float GetWidth() {return m_fWidth;}
	void SetHeight(float Height){ m_fHeight=Height;}
	float GetHeight() {return m_fHeight;}
	Mat33f& GetRot(){return m_Rot;}
	void SetRot(Mat33f &Rotation) { m_Rot=Rotation;}
	void SetColor(CColor& Color) { m_Color=Color;}
	CColor& GetColor(){ return m_Color;}

private:
	Vect3f m_Pos,m_PointA,m_PointB,m_PointC,m_PointD;
	Vect3f m_FirstPos;
	float m_fWidth,m_fHeight;
	CTexture * m_Texture;
	//Rotacion para mover el up segun el direction
	Mat33f m_Rot;
	CColor m_Color;

	//A----------B
	//|			 |
	//|			 |
	//|			 |
	//C----------D

};

