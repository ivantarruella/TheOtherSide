#include "Particle.h"
#include "Core.h"
#include "Utils\LerpAnimator3D.h"
#include "Camera\Camera.h"

#include "Base.h"

CParticle::~CParticle()
{
	m_vAcelerations.clear();
	
	m_vColors.clear();
	
	m_vDirections.clear();
	
	m_vSizes.clear();
}

void CParticle::SetDirection(const Vect3f& direction)
{
	m_Dir = direction;
}

const Vect3f& CParticle::GetDirection()
{
	return m_Dir;
}

void CParticle::SetTimeToLive(float time)
{
	m_TimeToLive = time;
}

float CParticle::GetTimeToLive()
{
	return m_TimeToLive;
}

void CParticle::UpdateParams(float deltaTime)
{
	CLerpAnimator1D l_inter;
	CLerpAnimator3D l_inter3D;

	for(unsigned int i=0; i<m_vColors.size(); i++)
	{
		if(m_Time >= m_vColors[i].time)
		{
			SetColor(m_vColors[i].col1);
		}
		else
		{
			CColor l_color;
			l_inter.SetValues(m_vColors[i-1].col1.GetRed(), m_vColors[i].col1.GetRed(),m_vColors[i].time - m_vColors[i-1].time, FUNC_INCREMENT);
			float l_aux;
			l_inter.Update(m_Time - m_vColors[i-1].time, l_aux);
			l_color.SetRed(l_aux);

			l_inter.SetValues(m_vColors[i-1].col1.GetGreen(), m_vColors[i].col1.GetGreen(),m_vColors[i].time - m_vColors[i-1].time, FUNC_INCREMENT);
			l_inter.Update(m_Time - m_vColors[i-1].time, l_aux);
			l_color.SetGreen(l_aux);
			
			l_inter.SetValues(m_vColors[i-1].col1.GetBlue(), m_vColors[i].col1.GetBlue(),m_vColors[i].time - m_vColors[i-1].time, FUNC_INCREMENT);
			l_inter.Update(m_Time - m_vColors[i-1].time, l_aux);
			l_color.SetBlue(l_aux);
			
			l_inter.SetValues(m_vColors[i-1].col1.GetAlpha(), m_vColors[i].col1.GetAlpha(),m_vColors[i].time - m_vColors[i-1].time, FUNC_INCREMENT);
			l_inter.Update(m_Time - m_vColors[i-1].time, l_aux);
			l_color.SetAlpha(l_aux);

			SetColor(l_color);

			i=m_vColors.size();
		}
	}

	for(unsigned int i=0; i<m_vDirections.size(); i++)
	{
		if(m_Time >= m_vDirections[i].time)
		{
			m_Dir = m_vDirections[i].dir1;
		}
		else
		{
			i=m_vDirections.size();
		}
	}

	for(unsigned int i=0; i<m_vAcelerations.size(); i++)
	{
		if(m_Time >= m_vAcelerations[i].time)
		{
			m_Acel = m_vAcelerations[i].acel1;
		}
		else
			i=m_vAcelerations.size();
	}

	for(unsigned int i=0; i<m_vSizes.size(); i++)
	{
		if(m_Time >= m_vSizes[i].time)
		{
			SetHeight(m_vSizes[i].size1);
			SetWidth(m_vSizes[i].size1);
		}
		else
		{
			if(m_vSizes[i-1].size1 < m_vSizes[i].size1)
				l_inter.SetValues(m_vSizes[i-1].size1, m_vSizes[i].size1,m_vSizes[i].time - m_vSizes[i-1].time, FUNC_INCREMENT);
			else
				l_inter.SetValues(m_vSizes[i-1].size1, m_vSizes[i].size1,m_vSizes[i].time - m_vSizes[i-1].time, FUNC_CONSTANT);

			float l_size;
			l_inter.Update(m_Time - m_vSizes[i-1].time, l_size);
			SetHeight(l_size);
			SetWidth(l_size);

			i=m_vSizes.size();
		}
	}
}

bool CParticle::Update(float deltaTime)
{
	m_Time +=deltaTime;
	Vect3f l_gravity = Vect3f(0.0f, 0.0f, 0.0f);
	if(m_bGravity)
		l_gravity = Vect3f(0.0f, -9.8f, 0.0f);
	if(m_Time < m_TimeToLive)
	{
		UpdateParams(deltaTime);
		//pos = (a*t^2)/2 + V0*t + pos0
		//Vect3f l_oldPos = GetPos();
		Vect3f l_oldPos = GetFirstPos();
		SetPos(l_oldPos + m_Dir * m_Time + ((m_Acel + l_gravity) * (m_Time*m_Time))/2);
		//CBillboard::Update(camera, 0.0);
		CBillboard::Update();
		return true;
	}

	return false;
}

void CParticle::AddColor(SColor color)
{
	m_vColors.push_back(SColor());
	m_vColors[m_vColors.size()-1].time = color.time;
	m_vColors[m_vColors.size()-1].col1 = color.col1;
	m_vColors[m_vColors.size()-1].col2 = color.col2;
}

void CParticle::AddDirection(SDirection direction)
{
	m_vDirections.push_back(SDirection());
	m_vDirections[m_vDirections.size()-1].time = direction.time;
	m_vDirections[m_vDirections.size()-1].dir1 = direction.dir1;
	m_vDirections[m_vDirections.size()-1].dir2 = direction.dir2;
}

void CParticle::AddSize(SSize size)
{
	m_vSizes.push_back(SSize());
	m_vSizes[m_vSizes.size()-1].time = size.time;
	m_vSizes[m_vSizes.size()-1].size1 = size.size1;
	m_vSizes[m_vSizes.size()-1].size2 = size.size2;
}

void CParticle::AddAceleration(SAceleration acel)
{
	m_vAcelerations.push_back(SAceleration());
	m_vAcelerations[m_vAcelerations.size()-1].time = acel.time;
	m_vAcelerations[m_vAcelerations.size()-1].acel1 = acel.acel1;
	m_vAcelerations[m_vAcelerations.size()-1].acel2 = acel.acel2;
}
