#include "ParticleEmitter.h"
#include "Random.h"
#include "XML\XMLTreeNode.h"
#include "TextureManager.h"
#include "Texture.h"
#include <ctime>

#include "Base.h"

void CParticleEmitter::Load(CXMLTreeNode &parser)
{
	m_vColors.clear();
	m_vDirections.clear();
	m_vAcelerations.clear();
	m_vSizes.clear();

	//m_Type = parser.GetPszProperty("type", "point");
	m_bGravity = parser.GetBoolProperty("gravity");
	m_fMinTimeLife = parser.GetFloatProperty("life_time_min", 0.0f);
	m_fMaxTimeLife = parser.GetFloatProperty("life_time_max", 0.0f);
	m_fMinEmitRate = parser.GetFloatProperty("emit_rate_min", 1.0f);
	m_fMaxEmitRate = parser.GetFloatProperty("emit_rate_max", 1.0f);
	std::string l_texturePath = parser.GetPszProperty("tex", "");
	m_fAngle = parser.GetFloatProperty("angle", 0.0f);

	m_sTexture = CORE->GetTextureManager()->GetTexture(l_texturePath);

	//Variación de los colores
	CXMLTreeNode l_Node = parser["color"];
	m_vColors.push_back(SColor());
	//SColor l_color = new SColor();
	m_vColors[m_vColors.size()-1].time = l_Node.GetFloatProperty("time", 0.0f);
	m_vColors[m_vColors.size()-1].col1.SetArgb(l_Node.GetVect4fProperty("color1",Vect4f(1.0f,1.0f,1.0f,1.0f)));
	m_vColors[m_vColors.size()-1].col2.SetArgb(l_Node.GetVect4fProperty("color2",Vect4f(1.0f,1.0f,1.0f,1.0f)));
	//m_vColors.push_back(l_color);

	for(int i=0;i<l_Node.GetNumChildren();i++){
		m_vColors.push_back(SColor());
		m_vColors[m_vColors.size()-1].time = l_Node(i).GetFloatProperty("time", 0.0f);
		m_vColors[m_vColors.size()-1].col1.SetArgb(l_Node(i).GetVect4fProperty("color1",Vect4f(1.0f,1.0f,1.0f,1.0f)));
		m_vColors[m_vColors.size()-1].col2.SetArgb(l_Node(i).GetVect4fProperty("color2",Vect4f(1.0f,1.0f,1.0f,1.0f)));
	}

	//Variación de los vectores directores
	l_Node = parser["direction"];
	m_vDirections.push_back(SDirection());
	//SDirection* l_direction = new SDirection();
	m_vDirections[m_vDirections.size()-1].time = l_Node.GetFloatProperty("time", 0.0f);
	m_vDirections[m_vDirections.size()-1].dir1 = l_Node.GetVect3fProperty("dir1", Vect3f(0.0f,0.0f,0.0f));
	m_vDirections[m_vDirections.size()-1].dir2 = l_Node.GetVect3fProperty("dir2", Vect3f(0.0f,0.0f,0.0f));
	//m_vDirections.push_back(l_direction);

	for(int i=0;i<l_Node.GetNumChildren();i++){
		m_vDirections.push_back(SDirection());
		m_vDirections[m_vDirections.size()-1].time = l_Node(i).GetFloatProperty("time", 0.0f);
		m_vDirections[m_vDirections.size()-1].dir1 = l_Node(i).GetVect3fProperty("dir1",Vect3f(0.0f,0.0f,0.0f));
		m_vDirections[m_vDirections.size()-1].dir2 = l_Node(i).GetVect3fProperty("dir2",Vect3f(0.0f,0.0f,0.0f));
	}


	//Variación de los tamaños
	l_Node = parser["size"];
	m_vSizes.push_back(SSize());
	//SSize* l_size = new SSize();
	m_vSizes[m_vSizes.size()-1].time = l_Node.GetFloatProperty("time", 0.0f);
	m_vSizes[m_vSizes.size()-1].size1 = l_Node.GetFloatProperty("mins", 1.0f);
	m_vSizes[m_vSizes.size()-1].size2 = l_Node.GetFloatProperty("maxs", 1.0f);
	//m_vSizes.push_back(l_size);

	for(int i=0;i<l_Node.GetNumChildren();i++){
		m_vSizes.push_back(SSize());
		m_vSizes[m_vSizes.size()-1].time = l_Node(i).GetFloatProperty("time", 0.0f);
		m_vSizes[m_vSizes.size()-1].size1 = l_Node(i).GetFloatProperty("mins",1.0f);
		m_vSizes[m_vSizes.size()-1].size2 = l_Node(i).GetFloatProperty("maxs",1.0f);
	}

	//Variación de las aceleraciones
	l_Node = parser["aceleration"];
	m_vAcelerations.push_back(SAceleration());
	//SAceleration* l_aceleration = new SAceleration();
	m_vAcelerations[m_vAcelerations.size()-1].time = l_Node.GetFloatProperty("time", 0.0f);
	m_vAcelerations[m_vAcelerations.size()-1].acel1 = l_Node.GetVect3fProperty("acel1", Vect3f(0.0f,0.0f,0.0f));
	m_vAcelerations[m_vAcelerations.size()-1].acel2 = l_Node.GetVect3fProperty("acel2", Vect3f(0.0f,0.0f,0.0f));
	//m_vAcelerations.push_back(l_aceleration);

	for(int i=0;i<l_Node.GetNumChildren();i++){
		m_vAcelerations.push_back(SAceleration());
		m_vAcelerations[m_vAcelerations.size()-1].time = l_Node(i).GetFloatProperty("time", 0.0f);
		m_vAcelerations[m_vAcelerations.size()-1].acel1 = l_Node(i).GetVect3fProperty("acel1", Vect3f(0.0f,0.0f,0.0f));
		m_vAcelerations[m_vAcelerations.size()-1].acel1 = l_Node(i).GetVect3fProperty("acel2", Vect3f(0.0f,0.0f,0.0f));
	}

}

void CParticleEmitter::Copy(CParticleEmitter &other)
{
	m_vPos = other.GetPos();
	m_Type = other.GetType();

	m_fMinTimeLife = other.GetMinTimeLife();
	m_fMaxTimeLife = other.GetMaxTimeLife();

	m_fMinEmitRate = other.GetMinRate();
	m_fMaxEmitRate = other.GetMaxRate();
	m_bGravity = other.GetGravity();
	m_fAngle = other.GetAngle();

	std::vector<SColor> l_ColorVector = other.GetColorVector();
	for(unsigned int i=0; i<l_ColorVector.size(); i++)
	{
		m_vColors.push_back(SColor());
		m_vColors[m_vColors.size()-1].time = l_ColorVector[i].time;
		m_vColors[m_vColors.size()-1].col1 = l_ColorVector[i].col1;
		m_vColors[m_vColors.size()-1].col2 = l_ColorVector[i].col2;
	}

	std::vector<SDirection> l_DirectionVector = other.GetDirectionVector();
	for(unsigned int i=0; i<l_DirectionVector.size(); i++)
	{
		m_vDirections.push_back(SDirection());
		m_vDirections[m_vDirections.size()-1].time = l_DirectionVector[i].time;
		m_vDirections[m_vDirections.size()-1].dir1 = l_DirectionVector[i].dir1;
		m_vDirections[m_vDirections.size()-1].dir2 = l_DirectionVector[i].dir2;
	}

	std::vector<SSize> l_SizeVector = other.GetSizeVector();
	for(unsigned int i=0; i<l_SizeVector.size(); i++)
	{
		m_vSizes.push_back(SSize());
		m_vSizes[m_vSizes.size()-1].time = l_SizeVector[i].time;
		m_vSizes[m_vSizes.size()-1].size1 = l_SizeVector[i].size1;
		m_vSizes[m_vSizes.size()-1].size2 = l_SizeVector[i].size2;
	}

	std::vector<SAceleration> l_AcelVector = other.GetAcelerationVector();
	for(unsigned int i=0; i<l_AcelVector.size(); i++)
	{
		m_vAcelerations.push_back(SAceleration());
		m_vAcelerations[m_vAcelerations.size()-1].time = l_AcelVector[i].time;
		m_vAcelerations[m_vAcelerations.size()-1].acel1 = l_AcelVector[i].acel1;
		m_vAcelerations[m_vAcelerations.size()-1].acel2 = l_AcelVector[i].acel2;
	}

	m_fNumNewPartsExcess = other.GetNumNewPartsExcess();
	m_sTexture = other.GetTexture();
}

void CParticleEmitter::Update(float deltaTime, int world)
{
	if (!m_bEmitterEnabled || m_World!=world)
		return;

	CreateParticle(deltaTime);

	for(unsigned int i=0; i<m_Particles.GetTotalElements(); i++)
	{
		CParticle * l_Particle = m_Particles.GetAt(i);
		if(l_Particle!=NULL && !m_Particles.IsFree(i))
		{
			if(!l_Particle->Update(deltaTime))
			{
				m_Particles.Free(i);
			}
		}
	}
}

void CParticleEmitter::CreateParticle(float fTimeDelta)
{
	CRandom * l_Random = CORE->GetRandom();

	float l_TimeToLive = l_Random->getRandFloat(m_fMinTimeLife, m_fMaxTimeLife);

	float fEmitRateThisFrame = l_Random->getRandFloat(m_fMinEmitRate, m_fMaxEmitRate);
	int iNumNewParts = (int)(fEmitRateThisFrame * fTimeDelta);
	m_fNumNewPartsExcess += (float)(fEmitRateThisFrame * fTimeDelta)-iNumNewParts;

	if (m_fNumNewPartsExcess > 1.0f)
	{
		iNumNewParts += (int)m_fNumNewPartsExcess;
		m_fNumNewPartsExcess -= (int)m_fNumNewPartsExcess;
	}

	for (int q=0; q < iNumNewParts; q++)
	{
		// Si hay espacio para una nueva partícula:
		if(m_Particles.GetNumFreeElements() <= 0)
		{
			return;
		}
		CParticle *part = m_Particles.New();
		part->SetTimeToLive(l_TimeToLive);
		part->SetGravity(m_bGravity);
		part->SetAngle(m_fAngle);
		part->SetTexture(m_sTexture);

		//Función virtual pura que calcula la posición de la partícula según el tipo de emisor
		GenerateParticlePosition(part);
		
		//determinar los colores según time
		for(unsigned int i=0; i<m_vColors.size(); i++)
		{
			SColor l_color;
			l_color.time = m_vColors[i].time;
			l_color.col1 = l_Random->getRandColor(m_vColors[i].col1, m_vColors[i].col2);
			part->AddColor(l_color);
		}

		//determinar la direccion segun time
		for(unsigned int i=0; i<m_vDirections.size(); i++)
		{
			SDirection l_direction;
			l_direction.time = m_vDirections[i].time;
			l_direction.dir1 = l_Random->getRandVect3f(m_vDirections[i].dir1, m_vDirections[i].dir2);
			part->AddDirection(l_direction);
		}

		//determinar el tamaño segun time
		for(unsigned int i=0; i<m_vSizes.size(); i++)
		{
			SSize l_size;
			l_size.time = m_vSizes[i].time;
			l_size.size1 = l_Random->getRandFloat(m_vSizes[i].size1, m_vSizes[i].size2);
			part->AddSize(l_size);
		}

		//determinar la aceleracion segun time
		for(unsigned int i=0; i<m_vAcelerations.size(); i++)
		{
			SAceleration l_acel;
			l_acel.time = m_vAcelerations[i].time;
			l_acel.acel1 = l_Random->getRandVect3f(m_vAcelerations[i].acel1, m_vAcelerations[i].acel2);
			part->AddAceleration(l_acel);
		}

	}
}

void CParticleEmitter::SetPos(const Vect3f& pos)
{
	m_vPos=pos;
}

void CParticleEmitter::SetMinRate(float min)
{
	m_fMinEmitRate=min;
}

void CParticleEmitter::SetMaxRate(float max)
{
	m_fMaxEmitRate=max;
}

void CParticleEmitter::SetNumNewPartsExcess(float num)
{
	m_fNumNewPartsExcess=num;
}

void CParticleEmitter::SetTexture(CTexture* texture)
{
	m_sTexture=texture;
}


const Vect3f& CParticleEmitter::GetPos()
{
	return m_vPos;
}

float CParticleEmitter::GetMinRate()
{
	return m_fMinEmitRate;
}

float CParticleEmitter::GetMaxRate()
{
	return m_fMaxEmitRate;
}

float CParticleEmitter::GetNumNewPartsExcess()
{
	return m_fNumNewPartsExcess;
}

CTexture* CParticleEmitter::GetTexture()
{
	return m_sTexture;
}

void CParticleEmitter::SetWorld(int world)
{
	m_World = world;
}


void CParticleEmitter::Render(CRenderManager * RM, int world, const CFrustum* Frustum) 
{
	if (!m_bEmitterEnabled || m_World!=world)
		return;

	for(unsigned int i=0; i<m_Particles.GetTotalElements(); ++i)
	{
		CParticle * l_Particle;
		l_Particle=m_Particles.GetAt(i);
		if(l_Particle!=NULL && !m_Particles.IsFree(i))
		{
			l_Particle->Render(RM, Frustum);
		}
	}

}