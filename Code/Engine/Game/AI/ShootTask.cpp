#include "ShootTask.h"
#include "Task.h"
#include "Character.h"
#include "BulletManager.h"
#include "Core.h"
#include "Renderer\RenderableObjectsLayersManager.h"
#include "Scripting\ScriptManager.h"
#include "Camera\Camera.h"
#include "Base.h"

CShootTask::CShootTask()
	:isShooting(false), m_TimeShoot(0.0f), m_shoot(false)
{
	m_Time = 0.0f;
}

CShootTask::~CShootTask()
{
	destroy();
}

bool CShootTask::checkCondition(float ElapsedTime)
{
	//if(m_Alarm)
	if(isNotBlockedEyeLine())
		return true;
	//TODO Está a tiro
	m_Time=0.0f;
	isShooting=false;
	m_shoot=false;
	//m_Owner->ChangeCharacterAnimation(WAIT_ANIM, 0.3f);
	//m_Owner->ClearCycle(AIM_ANIM, 0.3f);
	return false;
}

void CShootTask::doAction(float ElapsedTime)
{
	if(!isShooting)
	{
		m_Owner->ChangeCharacterAnimation(AIM_ANIM, 0.3f);
		m_Time = m_Owner->GetAnimationDuration() * 0.40f;
		m_TimeShoot = m_Owner->GetAnimationDuration() * 0.40f;
		isShooting=true;
	}
	else {
		m_Time = m_Time - ElapsedTime;
		m_TimeShoot = m_TimeShoot - ElapsedTime;
		if(m_Time <= 0.0f && !m_shoot)
		{
			//m_Owner->BlendCycle(AIM_ANIM, 0.3f);
			//m_Owner->ChangeCharacterAnimation(AIM_ANIM, 0.3f);
			Mat44f l_TransfMat;
			l_TransfMat.SetIdentity();
			CalSkeleton* l_pSkeleton = m_Owner->GetCalModel()->getSkeleton();
			int l_BoneIdx = l_pSkeleton->getCoreSkeleton()->getCoreBoneId("esbirro R Hand");
			if (l_BoneIdx != -1)
			{
				CalBone* l_pBone = l_pSkeleton->getBone(l_BoneIdx);
				CalVector l_vTranslation = l_pBone->getTranslationAbsolute();
				l_TransfMat = m_Owner->GetTransform();
				Vect3f l_vGunPosition = l_TransfMat * Vect3f(l_vTranslation.x, l_vTranslation.y+0.1f, l_vTranslation.z+0.5f);
				Vect3f l_pla = m_Player->GetPosition();
				l_pla.y += 1.0f;
				//Vect3f l_front = m_Owner->GetFront();
				//l_front = l_front.GetRotatedY(mathUtils::Deg2Rad(10.0f));
				//m_Owner->SetYaw(atan2(-l_front.x, l_front.z));
				Vect3f l_dir = (l_pla - l_vGunPosition).Normalize();	
				m_Owner->SetYaw(atan2(-l_dir.x, l_dir.z));
				
				float dist = CORE->GetCamera()->GetEye().Distance(m_Owner->GetPosition());
				std::stringstream action;
				action.clear();
				float vol = 1.0f - dist/25.0f;
				action << "sound_disparo_soldado(" << vol << ")";
				CORE->GetScriptManager()->RunCode(action.str());				
				
				CORE->GetBulletManager()->AddBullet(l_vGunPosition, l_dir, m_Player, BULLET_SPEED, 0.085f);
				isShooting=false;
			}
			m_shoot=true;
		}
		else{
			if(m_TimeShoot<=0)
			{
				isShooting=false;
				m_shoot=false;
			}
		}
	}
}

void CShootTask::destroy()
{
	for(unsigned int i=0; i < m_Sons.size(); i++)
	{
		CHECKED_DELETE(m_Sons[i]);
	}
	m_Sons.clear();
}

void CShootTask::Reset()
{
	m_Time = 0.0f;
}
