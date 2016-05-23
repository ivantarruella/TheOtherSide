#include "EscapeState.h"
#include "State.h"
#include "StateMachine.h"
#include "Character.h"
#include "Core.h"
#include "Monster.h"
#include "Player.h"
#include "Camera\ThirdPersonCamera.h"
#include "LightManager.h"
#include "ParticleManager.h"
#include "SoundManager.h"
#include "SpotLight.h"
#include "Renderer\RenderableObjectsLayersManager.h"
#include "Scripting\ScriptManager.h"
#include "Characters\Monster.h"
#include "Base.h"

void CEscapeState::Update(float ElapsedTime)
{
	const float rotationSpeed = mathUtils::Deg2Rad( 90.0f );
	m_Owner->ChangeCharacterAnimation(USE_ANIM, 0.3f);
	float l_laserLength = ((CSpotLight*)CORE->GetLightManager()->GetResource("LASER"))->GetEndRangeAttenuation();
	if(isLighted() && (m_Owner->GetPosition().Distance(m_Player->GetPosition())) < l_laserLength)
	{
		/*float dist = CORE->GetCamera()->GetEye().Distance(m_Owner->GetPosition());
		std::stringstream action;
		action.clear();
		float vol = 1.0f - dist/25.0f;
		action << "sound_monster_damage(" << vol << ")";
		CORE->GetScriptManager()->RunCode(action.str());				*/
		
		Vect3f l_Pos = m_Owner->GetPosition() + Vect3f(0.f, 1.f, 0.f);
		if (!isHurted) {
			// damange sound
			m_Source = CORE->GetSoundManager()->CreateSource();
			CORE->GetSoundManager()->PlaySource2D(m_Source, "monster_damage", false);

			// damage particles
			CORE->GetParticleManager()->AddParticleEmitterInstance("monster_damage", m_Owner->GetName(), l_Pos, l_Pos, 0.5f, 2); 
			CParticleEmitter* l_Particles = CORE->GetParticleManager()->GetParticleEmitter(m_Owner->GetName());
			if (l_Particles!=NULL)
				l_Particles->SetEnabled(true);

			isHurted = true;
		}
		else
		{
			CParticleEmitter* l_Particles = CORE->GetParticleManager()->GetParticleEmitter(m_Owner->GetName());
			if (l_Particles!=NULL)
				l_Particles->SetPos(l_Pos);
		}

		m_Time=0.0f;
		if(isFaced())
		{
			Vect3f l_dir = (m_Player->GetPosition() - m_Owner->GetPosition()).Normalize();
			m_Owner->SetYaw(atan2(-l_dir.x, l_dir.z));
			m_Owner->SetPosition(-m_Owner->GetFront()*0.015f, ElapsedTime);
			return;
		}
		else
		{
			if( isAtLeft())
			{
				m_Owner->SetYaw(m_Owner->GetYaw() + rotationSpeed*ElapsedTime);
			} 
			else{ 				
				m_Owner->SetYaw(m_Owner->GetYaw() - rotationSpeed*ElapsedTime);
			}
			return;
		}
	}
	else{
		m_Owner->ChangeCharacterAnimation(AIM_ANIM, 0.3f);
		m_Time = m_Time + ElapsedTime;
		if(m_Time > 1.0f)
		{
			m_Owner->ChangeCharacterAnimation(WALK_ANIM, 0.3f);
			m_StateMachine->ChangeState(m_Previous);

			if (isHurted) {
				CORE->GetSoundManager()->DeleteSource(m_Source);
				CORE->GetParticleManager()->RemoveParticleEmitterInstance(m_Owner->GetName());
				isHurted = false;
			}
		}
	}
}

void CEscapeState::Create()
{
	m_Time = 0.0f;
	m_Name = "ESCAPE";
	m_Previous = "IDLE";
}

void CEscapeState::Init()
{
	m_Time = 0.0f;
}

void CEscapeState::Reset()
{	
	m_Time = 0.0f;
	m_Name = "ESCAPE";
	m_Previous = "IDLE";
}

bool CEscapeState::isFaced()
{
	Vect3f l_aux = (m_Player->GetPosition() - m_Owner->GetPosition()).Normalize();
	
	float l_vSuma = m_Owner->GetFront() * l_aux;
	if(l_vSuma < 0.95f)
		return false;
	return true;
}

bool CEscapeState::isAtLeft()
{
	Vect3f l_up(0.0f, 1.0f, 0.0f);
	Vect3f l_right = l_up ^ m_Owner->GetFront();
	l_right.Normalize();
	float l_d = -(l_right * m_Owner->GetPosition());

	float v( (l_right * m_Player->GetPosition()) + l_d );

	return v <= 0.0f;
}

bool CEscapeState::isLighted()
{
	Vect3f l_LightPos  = m_Player->GetPosition();
	CalSkeleton* l_pSkeleton = m_Player->GetCalModel()->getSkeleton();
	int l_iBoneId = l_pSkeleton->getCoreSkeleton()->getCoreBoneId("Dummy_Arma");
	if (l_iBoneId != -1)
	{
		CalBone* l_pBone = l_pSkeleton->getBone(l_iBoneId);
		CalVector l_vTranslation = l_pBone->getTranslationAbsolute();
		Mat44f l_TransfMat = m_Player->GetTransform();
		l_LightPos = l_TransfMat * Vect3f(l_vTranslation.x, l_vTranslation.y, l_vTranslation.z);
	}

	CThirdPersonCamera* l_Camera=(CThirdPersonCamera*)CORE->GetCamera();
	float l_fYaw = l_Camera->GetPlayerYaw();
	float l_fPitch = l_Camera->GetPlayerPitch();
	
	if(((CPlayer *)m_Player)->GetPlayerShoot() && ((CPlayer *)m_Player)->GetPlayerMunitionGUI() > 0.0f)
	{
		Vect3f l_pos = l_LightPos;
		Vect3f l_dir = Vect3f(cos(l_fYaw) * cos(l_fPitch), sin(l_fPitch), sin(l_fYaw) * cos(l_fPitch));
		if(((CMonster*)m_Owner)->isLighted(l_pos, l_dir,((CPlayer*)m_Player)->GetLanternDistance()))
		{
			return true;
		}
	}
	return false;
}
