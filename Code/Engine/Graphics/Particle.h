#pragma once
#ifndef INC_PARTICLE_H_
#define INC_PARTICLE_H_

#include "Math\Vector3.h"
#include "Math\Color.h"
#include "Billboard.h"
#include <vector>


class CCamera;


struct SSize
{
	SSize::SSize() {;}
	float time;
	float size1;
	float size2;
};

struct SAceleration
{
	SAceleration::SAceleration(){;};
	float time;
	Vect3f acel1;
	Vect3f acel2;
};


struct SColor
{
	float time;
	CColor col1;
	CColor col2;
};

struct SDirection
{
	float time;
	Vect3f dir1;
	Vect3f dir2;
};



class CParticle : public CBillboard
{
public:
	CParticle()
	:m_Dir(Vect3f(0.0f,0.0f,0.0f)), m_Size(1.0f), m_Col(CColor(1.0f,1.0f,1.0f)), m_Acel(0.0f) 
	,m_Time(0.0f)
	,m_TimeToLive(0.0f)
	{
	};
	
	CParticle(const Vect3f& direction, float timetolive)
	:m_Dir(direction), m_Size(1.0f), m_Col(CColor(1.0f,1.0f,1.0f)), m_Acel(0.0f)
	,m_Time(0.0f)
	,m_TimeToLive(timetolive)
	{
	};

	~CParticle();

	bool Update(float deltaTime);
	void UpdateParams(float deltaTime);

	void SetDirection(const Vect3f& direction);
	const Vect3f& GetDirection();

	void SetTimeToLive(float time);
	void SetGravity(bool gravity){m_bGravity = gravity;};
	void SetAngle(float angle){m_fAngle=angle;};

	float GetTimeToLive();
	bool GetGravity(){return m_bGravity;};
	float GetAngle(){return m_fAngle;};

	void AddColor(SColor color);
	void AddDirection(SDirection direction);
	void AddSize(SSize size);
	void AddAceleration(SAceleration acel);

	std::vector<SColor> GetColorVector(){return m_vColors;};
	std::vector<SDirection> GetDirectionVector(){return m_vDirections;};
	std::vector<SSize> GetSizeVector(){return m_vSizes;};
	std::vector<SAceleration> GetAcelerationVector(){return m_vAcelerations;};

private:
	Vect3f m_Dir;
	CColor m_Col;
	float m_Size;
	Vect3f m_Acel;

	float m_Time;
	float m_TimeToLive;
	bool m_bGravity;
	float m_fAngle;

	std::vector<SColor> m_vColors;
	std::vector<SDirection> m_vDirections;
	std::vector<SSize> m_vSizes;
	std::vector<SAceleration> m_vAcelerations;
};

#endif