#include "PatrolState.h"
#include "State.h"
#include "StateMachine.h"
#include "Character.h"
#include "Monster.h"
#include "Node.h"
#include "NodeManager.h"
#include "PhysicsManager.h"
#include "Core.h"
#include "Renderer\RenderableObjectsLayersManager.h"
#include "Characters\Monster.h"
#include "Base.h"

CPatrolState::~CPatrolState()
{
	/*for(unsigned int i=0; i<m_PatrolWay.size(); i++)
	{
		CHECKED_DELETE(m_PatrolWay[i]);
	}
	m_PatrolWay.clear();*/
}

void CPatrolState::Update(float ElapsedTime)
{	
	m_Time = m_Time + ElapsedTime;	
	const float rotationSpeed = mathUtils::Deg2Rad( 90.0f );

	float l_sqdistance = m_Owner->GetPosition().SqDistance(m_Player->GetPosition());
	
	
	if(isFaced() && l_sqdistance<4.0f)
	{
		m_StateMachine->ChangeState("ATTACK");
		return;
	}

	if(isHeared() || isSeen())
	{
		m_StateMachine->ChangeState("CHASE");
		return;
	}

	if(m_PatrolWay.size() > 0)
	{
		Vect3f l_point = CORE->GetNodeManager()->GetNode(m_numPatrol)->GetPos();
		if(isReachable(l_point))
		{
			//if(m_Owner->GetPosition() != m_PatrolWay[m_numPatrol])
			if(m_Owner->GetPosition() != l_point)
			{
				m_Owner->ChangeCharacterAnimation(WALK_ANIM, 0.3f);
				Vect3f l_pos = m_Owner->GetPosition();
				//Vect3f l_dir = m_PatrolWay[m_numPatrol] - m_Owner->GetPosition();
				Vect3f l_dir = l_point - m_Owner->GetPosition();
				l_dir.Normalize();
				m_Owner->SetYaw(atan2(-l_dir.x, l_dir.z));
			
				Vect3f l_new = m_Owner->GetPosition() + l_dir * ElapsedTime * 1.30f;
				//Vect3f l_old = (m_PatrolWay[m_numPatrol] - l_new).Normalize();
				Vect3f l_old = (l_point - l_new).Normalize();
		
				if((l_dir * l_old) > 0.0f)
					m_Owner->SetPosition(m_Owner->GetFront()*0.015f, ElapsedTime);
				else{
					//m_Owner->SetPosition(m_PatrolWay[m_numPatrol]);
					//m_Owner->SetPosition(l_point);
					l_dir = l_point - m_Owner->GetPosition();
					l_dir.y=0.0f;
					m_Owner->SetPosition(l_dir, ElapsedTime);
					m_Time = 0.0f;
				}
			}
			else{
				if(m_Time >10.0f)
				{
					if(m_numPatrol == m_PatrolWay.size()-1)
					{
						m_numPatrol = 0;
					}
					else{
						m_numPatrol++;
					}
					m_Time = 0.0f;			
					m_bFaced=false;
				}
				else{
					int l_next;
					if(m_numPatrol == m_PatrolWay.size()-1 && m_ascent)
					{
						m_ascent = false;
						l_next = m_PatrolWay.size()-2;
					}
					else{
						if(m_numPatrol == 0 && !m_ascent)
						{
							m_ascent = true;
							l_next = 1;
						}
						else{
							if(m_ascent)
							{
								l_next = m_numPatrol+1;
							}
							else{
								l_next = m_numPatrol-1;
							}
						}
					}
					Vect3f l_nextPoint = CORE->GetNodeManager()->GetNode(l_next)->GetPos();
					//if(!m_bFaced && !isFaced(m_PatrolWay[l_next]))
					if(!m_bFaced && !isFaced(l_nextPoint))
					{
						//if( isAtLeft(m_PatrolWay[l_next]))
						if( isAtLeft(l_nextPoint))
						{
							m_Owner->SetYaw(m_Owner->GetYaw() + rotationSpeed*ElapsedTime);
						} 
						else{ 				
							m_Owner->SetYaw(m_Owner->GetYaw() - rotationSpeed*ElapsedTime);
						}
						return;
					}
					else{
						if(!m_bFaced)
						{
							//Vect3f l_dir = m_PatrolWay[l_next] - m_Owner->GetPosition();
							Vect3f l_dir = l_nextPoint - m_Owner->GetPosition();
							l_dir.Normalize();
							m_Owner->SetYaw(atan2(-l_dir.x, l_dir.z));
							m_bFaced=true;
						}
						m_Owner->ChangeCharacterAnimation(WAIT_ANIM, 0.3f);
					}
				}
			}
		}
		else{
			if(m_Path.empty())
			{
				CNode * l_start = CORE->GetNodeManager()->GetNode(CORE->GetNodeManager()->NearestNode(m_Owner->GetPosition()));
				CNode * l_end = CORE->GetNodeManager()->GetNode(m_numPatrol);
				m_Path = CORE->GetNodeManager()->AStarAlgorithm(l_start, l_end);
			}
			if(isFaced(m_Path[0]->GetPos()))
			{
				Vect3f l_dir = (m_Path[0]->GetPos() - m_Owner->GetPosition()).Normalize();
				m_Owner->SetYaw(atan2(-l_dir.x, l_dir.z));
				//m_Owner->SetPosition(m_Owner->GetPosition() + m_Owner->GetFront( ) * ElapsedTime*1.30f);
				Vect3f l_new = m_Owner->GetPosition() +m_Owner->GetFront() * ElapsedTime * 1.3f;
				Vect3f l_post = (m_Path[0]->GetPos() - l_new).Normalize();

				float l_dist = l_new.SqDistance(m_Path[0]->GetPos()); 
				if((l_dir * l_post) > 0.0f)
				//if(l_dist > 0.01f)
				{
					m_Owner->SetPosition(m_Owner->GetFront()*0.015f, ElapsedTime);
				}
				else{
					//m_Owner->SetPosition(m_Path[0]->GetPos());
					l_dir = m_Path[0]->GetPos() - m_Owner->GetPosition();
					l_dir.y=0.0f;
					m_Owner->SetPosition(l_dir, ElapsedTime);
					std::vector<CNode*>::iterator it = m_Path.begin();
					m_Path.erase(it);
				}
			}
			else{
				if(isAtLeft(m_Path[0]->GetPos()))
				{
					m_Owner->SetYaw(m_Owner->GetYaw() + rotationSpeed*ElapsedTime);
				} 
				else{ 				
					m_Owner->SetYaw(m_Owner->GetYaw() - rotationSpeed*ElapsedTime);
				}
				return;
			}
		}
	}
}

void CPatrolState::Create()
{
	m_Time = 0.0f;
	m_Name = "PATROL";
	m_Path.clear();
}

void CPatrolState::Init()
{
	m_Time = 0.0f;
	m_bFaced = false;
	m_ascent = true;
	m_numPatrol = 0;
	m_Path.clear();
}

void CPatrolState::Reset()
{
	m_Time = 0.0f;
	m_Name = "PATROL";
	m_bFaced = false;
	m_ascent = true;
	m_numPatrol = 0;
	m_Path.clear();
}

bool CPatrolState::isFaced()
{
	Vect3f l_aux = (m_Player->GetPosition() - m_Owner->GetPosition()).Normalize();
	
	float l_vSuma = m_Owner->GetFront() * l_aux;
	if(l_vSuma < 0.98f)
		return false;
	return true;
}

bool CPatrolState::isFaced(Vect3f pos)
{
	Vect3f l_pos = m_Owner->GetPosition();
	Vect3f l_aux = (pos - m_Owner->GetPosition()).Normalize();
	
	float l_vSuma = m_Owner->GetFront() * l_aux;
	if(l_vSuma < 0.98f)
		return false;
	return true;
}

bool CPatrolState::isHeared()
{
	Vect3f l_aux = (m_Player->GetPosition() - m_Owner->GetPosition());		
	if(l_aux.Length() > m_HearDistance)
		return false;
	return true;
}

bool CPatrolState::isSeen()
{
	Vect3f l_aux = (m_Player->GetPosition() - m_Owner->GetPosition());		
	if(l_aux.Length() > m_VisionDistance)
		return false;
	l_aux.Normalize();
	float l_vSuma = m_Owner->GetFront() * l_aux;
	if(l_vSuma < degToDot(m_VisionAngle/2.0f))
		return false;
	return true;
}

bool CPatrolState::isAtLeft()
{
	Vect3f l_up(0.0f, 1.0f, 0.0f);
	Vect3f l_right = l_up ^ m_Owner->GetFront();
	l_right.Normalize();
	float l_d = -(l_right * m_Owner->GetPosition());

	float v( (l_right * m_Player->GetPosition()) + l_d );

	return v <= 0.0f;
}

bool CPatrolState::isAtLeft(Vect3f pos)
{
	Vect3f l_up(0.0f, 1.0f, 0.0f);
	Vect3f l_right = l_up ^ m_Owner->GetFront();
	l_right.Normalize();
	float l_d = -(l_right * m_Owner->GetPosition());

	float v( (l_right * pos) + l_d );

	return v <= 0.0f;
}

bool CPatrolState::isReachable(Vect3f pos)
{
	Vect3f l_up(0.0f, 1.0f, 0.0f);
	Vect3f l_right = l_up ^ m_Owner->GetFront();
	
	CPhysicUserData * l_user;	
	CPhysicUserData * l_pla = m_Player->GetPhysicUserData();
	SCollisionInfo l_info;
	Vect3f l_fron = m_Owner->GetFront();
	Vect3f l_play = pos;
	Vect3f l_dir = (pos - m_Owner->GetPosition()).Normalize();
	Vect3f l_pos = m_Owner->GetPosition();

	l_user = CORE->GetPhysicsManager()->RaycastClosestActor(l_pos + l_up * 1.05f +l_dir*0.2f + l_right * 0.1f, l_dir , 0xffffffff, l_info);
	if(l_user == m_Player->GetPhysicUserData())
	{
		return true;
	}
	l_user = CORE->GetPhysicsManager()->RaycastClosestActor(l_pos + l_up * 0.95f +l_dir*0.2f - l_right * 0.1f, l_dir, 0xffffffff, l_info);
	if(l_user == m_Player->GetPhysicUserData())
	{
		return true;
	}
	l_user = CORE->GetPhysicsManager()->RaycastClosestActor(l_pos + l_up * 1.05f +l_dir*0.2f - l_right * 0.1f, l_dir, 0xffffffff, l_info);
	if(l_user == m_Player->GetPhysicUserData())
	{
		return true;
	}
	l_user = CORE->GetPhysicsManager()->RaycastClosestActor(l_pos + l_up * 0.95f +l_dir*0.2f + l_right * 0.1f, l_dir, 0xffffffff, l_info);
	if(l_user == m_Player->GetPhysicUserData())
	{
		return true;
	}

	return false;
}
