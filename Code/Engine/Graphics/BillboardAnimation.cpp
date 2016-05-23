#include "BillboardAnimation.h"
#include "Texture.h"
#include "Camera\Camera.h"
#include "XML\XMLTreeNode.h"
#include "TextureManager.h"
#include "Base.h"

CBillboardAnimation::CBillboardAnimation(CXMLTreeNode &parser){
	SetColor(CColor(parser.GetVect3fProperty("color",Vect3f(1.0f))));
	float size=parser.GetFloatProperty("size",2.0f);
	SetHeight(size);
	SetWidth(size);
	m_TimePerImage=parser.GetFloatProperty("time",0.2f);
	m_Loop=parser.GetBoolProperty("loop",false);
	CXMLTreeNode l_Node = parser["Textures"];
	for(int i=0;i<l_Node.GetNumChildren();i++){
		
		std::string l_TexturePath=l_Node(i).GetPszProperty("tex","");
		CTexture *l_Texture = CORE->GetTextureManager()->GetTexture(l_TexturePath);
		m_Textures.push_back(l_Texture);
	}
	m_Index=0;
	m_fTime=0.0f;
	SetTexture(m_Textures[m_Index]);
	SetPos(Vect3f(0.0f,0.0f,0.0f));
	
}

CBillboardAnimation::CBillboardAnimation(CBillboardAnimation &other){
	
	SetColor(other.GetColor());
	SetHeight(other.GetHeight());
	SetWidth(other.GetWidth());
	SetPos(Vect3f(0.0f,0.0f,0.0f));
	m_TimePerImage=other.GetTimePerImage();
	m_Loop=other.GetLoop();
	m_fTime=0.0f;
	m_Index=0;

	std::vector<CTexture*> l_TextVector = other.GetTextureVector();
	for( uint16 i=0;i<l_TextVector.size();i++){
		m_Textures.push_back(l_TextVector.at(i));
	}
	
	SetTexture(m_Textures[m_Index]);


}

CBillboardAnimation::~CBillboardAnimation(void)
{
	
	m_Textures.clear();
}


void CBillboardAnimation:: Update(float dt){

	m_fTime+=dt;
	CBillboard::Update();
	if(m_fTime>m_TimePerImage){
		m_Index= (m_Index+1)%m_Textures.size();
		m_fTime=0.0;
		SetTexture(m_Textures[m_Index]);
		
	}
	
}



//Afegim textures
void CBillboardAnimation::Init()
{
	/*
	CTexture * m_Texture = new CTexture();
	m_Texture->Load("data\\Textures\\flame1.tga");
	m_Textures.push_back(m_Texture);

	m_Texture = new CTexture();
	m_Texture->Load("data\\Textures\\flame2.tga");
	m_Textures.push_back(m_Texture);

	m_Texture = new CTexture();
	m_Texture->Load("data\\Textures\\flame3.tga");
	m_Textures.push_back(m_Texture);

	m_Texture = new CTexture();
	m_Texture->Load("data\\Textures\\flame4.tga");
	m_Textures.push_back(m_Texture);

	m_Texture = new CTexture();
	m_Texture->Load("data\\Textures\\flame5.tga");
	m_Textures.push_back(m_Texture);

	m_Texture = new CTexture();
	m_Texture->Load("data\\Textures\\flame6.tga");
	m_Textures.push_back(m_Texture);

	m_Texture = new CTexture();
	m_Texture->Load("data\\Textures\\flame7.tga");
	m_Textures.push_back(m_Texture);

	m_Texture = new CTexture();
	m_Texture->Load("data\\Textures\\flame8.tga");
	m_Textures.push_back(m_Texture);


	SetTexture(m_Textures[m_Index]);
	SetPos(Vect3f(0.0f,0.0f,0.0f));
	*/

}