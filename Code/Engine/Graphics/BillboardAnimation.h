#pragma once
#include <vector>
#include "Billboard.h"


class CCamera;
class CXMLTreeNode;

class CBillboardAnimation :
	public CBillboard
{
public:
	CBillboardAnimation(void):m_Index(0)
		,m_TimePerImage(0.5)
		,m_Textures (NULL)
		,m_fTime(0.0f)
		,m_Loop(false)	{}

	CBillboardAnimation(CXMLTreeNode &parser);
	CBillboardAnimation(CBillboardAnimation &other);
	~CBillboardAnimation(void);
	void Init();
	virtual void Update (float dt);
	void SetTimePerImage(float Time){ m_TimePerImage=Time;}
	float GetTimePerImage(){ return m_TimePerImage;}
	void SetLoop(bool Loop){ m_Loop=Loop;}
	bool GetLoop(){ return m_Loop;}
	void SetImageIndex(uint16 Index){m_Index=Index;}
	uint16 GetImageIndex(){return m_Index;}
	std::vector <CTexture*> GetTextureVector(){return m_Textures;}
	

private:
	std::vector <CTexture*> m_Textures;
	float m_TimePerImage;
	float m_fTime;
	uint16 m_Index;
	bool m_Loop;



};

