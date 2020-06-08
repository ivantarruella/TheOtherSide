#include "MoveTask.h"
#include "Task.h"
#include "Character.h"
#include "Core.h"
#include "NodeManager.h"
#include "Node.h"
#include "CoverManager.h"
#include "Random.h"
#include "Renderer\RenderableObjectsLayersManager.h"

#include "Base.h"

CMoveTask::~CMoveTask()
{
	destroy();
}

bool CMoveTask::checkCondition(float ElapsedTime)
{
	//return false;
	if(!m_bReady)
	{
		m_TimeUncover = 0.0f;
		return true;
	}
	else
	{
		m_TimeUncover -=ElapsedTime;
		if(m_TimeUncover < 0.0f)
		{
			m_bCovered = false;
			m_Time = 0.0f;
			m_bReady =false;
		}
		return false;
	}
	//Siempre tendremos que mirar si nos movemos
}

void CMoveTask::doAction(float ElapsedTime)
{
	if(m_ApropiateCover < 0)
	{
		if(m_bChasing)
		{
			if(!Chase(ElapsedTime))
			{
				m_bChasing = false;
				//m_bCoverReached = false;
				//m_bCovered = false;
				m_Path.clear();
			}
			else{
				return;
			}
		}
		
		int l_apropiate = MostApropiateCover();
		m_ApropiateCover = l_apropiate;
		((CSoldier*)m_Owner)->SetCover(m_ApropiateCover);
		if(l_apropiate == -1)
		{
			m_bChasing = true;
			m_TimeUncover =1.0f;
			m_bReady = true;
		}
		m_TimeWait = 0.0f;
		return;
	}
	else{
		if(m_bCoverReached && !isNotBlockedEyeLine(CORE->GetCoverManager()->GetExt(m_ApropiateCover)))
		{
			m_TimeWait +=ElapsedTime;
			if(m_TimeWait > 8.0f)
			{
				m_ApropiateCover = -2;
				((CSoldier*)m_Owner)->SetCover(m_ApropiateCover);
				m_bChasing = false;
				m_bCoverReached = false;
				m_bCovered = false;
				m_TimeUncover = 0.0f;
				m_TimeWait=0.0f;
				m_Time=0.0f;
				return;
			}
		}
		else{
			m_TimeWait = 0.0f;
		}

		if(!m_bCoverReached)
		{
			reachCover(m_ApropiateCover, ElapsedTime);
			if(!m_bCoverReached)
			{
				//m_ApropiateCover = MostApropiateCover();	
				return;
			}
		}
		if(!m_bCovered)
		{
			Covered(m_ApropiateCover, ElapsedTime);
			m_Time = CORE->GetRandom()->getRandFloat(2.0f, 4.0f);
			return;
		}
		m_Time -=ElapsedTime;

		if(m_Time < 0.0f)
		{
			if(!reachCover(m_ApropiateCover, ElapsedTime))
			{
				return;
			}
		}
		else{
			m_Owner->ChangeCharacterAnimation(WAIT_ANIM, 0.3f);
			return;
		}
		m_TimeUncover = CORE->GetRandom()->getRandFloat(3.0f, 7.0f);
		m_bReady=true;
	
		return;
	}
}

void CMoveTask::destroy()
{
	for(unsigned int i=0; i < m_Sons.size(); i++)
	{
		CHECKED_DELETE(m_Sons[i]);
	}
	m_Sons.clear();
}

void CMoveTask::Reset()
{
	m_Time = 0.0f;
	m_bChasing =false;
	m_bCoverReached = false;
	m_bCovered = false;
	m_ApropiateCover = -2;
	m_TimeUncover = 0.0f;
	m_TimeWait = 0.0f;
	m_bReady = false;
}

void CMoveTask::SetPlayer(CCharacter* player)
{
	m_Player = player;
	for(unsigned int i=0; i<m_Sons.size(); i++)
	{
		m_Sons[i]->SetPlayer(player);
	}
}

int CMoveTask::MostApropiateCover()
{
	std::vector<int> l_posibles;
	for(int i=0; i< CORE->GetCoverManager()->GetNumCovers(); i++)
	{
		if(!CORE->GetEnemyManager()->isCoverBusy(i))
		{
			if(m_Player->GetPosition().Distance(CORE->GetCoverManager()->GetExt(i)) > 2.0f)
			{
				if(m_Owner->GetPosition().Distance(CORE->GetCoverManager()->GetExt(i)) <= 10.0f)
				{
					if(isNotBlocked(CORE->GetCoverManager()->GetExt(i)))
					{
						if(!isNotBlockedEyeLine(CORE->GetCoverManager()->GetCover(i)))
						{
							if(isNotBlockedEyeLine(CORE->GetCoverManager()->GetExt(i)))
							{
								l_posibles.push_back(i);
							}
						}
					}
				}
			}
		}
	}

	if(l_posibles.size()==1)
	{
		return l_posibles[0];
	}

	int l_aux=-1;
	float l_minLength=1000.0f;
	float l_temp;
	for(unsigned int i=0; i<l_posibles.size(); i++)
	{
		l_temp = m_Owner->GetPosition().Distance(CORE->GetCoverManager()->GetExt(l_posibles[i]));
		if(l_temp < l_minLength)
		{
			l_minLength=l_temp;
			l_aux = l_posibles[i];
		}
	}

	return l_aux;
}

bool CMoveTask::Chase(float ElapsedTime)
{
	if((isHeared() && isNotBlockedEyeLine()) || (isSeen() && isNotBlockedEyeLine()))
	{
		return false;
	}

	m_Owner->ChangeCharacterAnimation(RUN_ANIM, 0.3f);	
	const float rotationSpeed = mathUtils::Deg2Rad( 180.0f );
	int l_numNode = CORE->GetNodeManager()->NearestNodeSoldier(m_Player->GetPosition());
	Vect3f l_point = CORE->GetNodeManager()->GetNodeSoldier(l_numNode)->GetPos();
	if(!isReachable(l_point))
	{
		if(m_Path.empty())
		{
			CNode * l_start = CORE->GetNodeManager()->GetNodeSoldier(CORE->GetNodeManager()->NearestNodeSoldier(m_Owner->GetPosition()));
			CNode * l_end = CORE->GetNodeManager()->GetNodeSoldier(l_numNode);
			m_Path = CORE->GetNodeManager()->AStarAlgorithmSoldier(l_start, l_end);
		}
		if (!m_Path.empty())
		{
			if(isFaced(m_Path[0]->GetPos()))
			{
				Vect3f l_dir = m_Path[0]->GetPos();
				l_dir.y = m_Owner->GetPosition().y;
				l_dir = (l_dir-m_Owner->GetPosition()).Normalize();
				m_Owner->SetYaw(atan2(-l_dir.x, l_dir.z));				
			
				Vect3f l_prev= m_Owner->GetPosition();
				
				m_Owner->SetPosition(m_Owner->GetFront()*0.090f, ElapsedTime);
				//Vect3f l_new = m_Owner->GetPosition() +m_Owner->GetFront() * ElapsedTime * 3.0f;
				Vect3f l_new = m_Owner->GetPosition();
			
				l_new.y = m_Path[0]->GetPos().y;

				float l_dist = l_new.SqDistance(m_Path[0]->GetPos()); 
				if(l_dist > 0.09f)
				{
					//m_Owner->SetPosition(l_new);
				}
				else
				{
					//m_Owner->SetPosition(m_Path[0]->GetPos());
					//m_Owner->SetPosition(m_Owner->GetFront()*0.0f, ElapsedTime);					
					//m_Owner->ChangeCharacterAnimation(WAIT_ANIM, 0.3f);
					l_dir = m_Path[0]->GetPos() - m_Owner->GetPosition();
					l_dir.y=0.0f;
					m_Owner->SetPosition(l_dir, ElapsedTime);
					std::vector<CNode*>::iterator it = m_Path.begin();
					m_Path.erase(it);
					if (m_Path.size() == 0)
					{
						m_Owner->ChangeCharacterAnimation(WAIT_ANIM, 0.3f);
					}
				}
			}
			else{				
				if(!isFacedRun(m_Path[0]->GetPos()))
				{
					if(isAtLeft(m_Path[0]->GetPos()))
					{					
						m_Owner->SetYaw(m_Owner->GetYaw() + rotationSpeed*ElapsedTime);
					} 
					else{ 				
						m_Owner->SetYaw(m_Owner->GetYaw() - rotationSpeed*ElapsedTime);		
					}
				}
				else{
					Vect3f l_dir = m_Path[0]->GetPos();
					l_dir.y = m_Owner->GetPosition().y;
					l_dir = (l_dir-m_Owner->GetPosition()).Normalize();
					m_Owner->SetYaw(atan2(-l_dir.x, l_dir.z));
					Vect3f l_front = m_Owner->GetFront();
				}
			}
		}
		else
		{
			CNode* l_start = CORE->GetNodeManager()->GetNodeSoldier(CORE->GetNodeManager()->NearestNodeSoldier(m_Owner->GetPosition()));
			CNode* l_end = CORE->GetNodeManager()->GetNodeSoldier(CORE->GetNodeManager()->NearestNodeSoldier(m_Owner->GetPositionIni()));
			m_Path = CORE->GetNodeManager()->AStarAlgorithmSoldier(l_start, l_end);
			if (m_Path.size() == 1)
			{
				m_Owner->ChangeCharacterAnimation(WAIT_ANIM, 0.3f);
			}
		}
	}
	else{
		if(isFaced(l_point))
		{			
			m_Owner->ChangeCharacterAnimation(RUN_ANIM, 0.3f);	
			Vect3f l_dir = l_point;
			l_dir.y = m_Owner->GetPosition().y;
			l_dir = (l_dir-m_Owner->GetPosition()).Normalize();
			m_Owner->SetYaw(atan2(-l_dir.x, l_dir.z));				
			
			Vect3f l_prev= m_Owner->GetPosition();
				
			m_Owner->SetPosition(m_Owner->GetFront()*0.090f, ElapsedTime);
			//Vect3f l_new = m_Owner->GetPosition() +m_Owner->GetFront() * ElapsedTime * 3.0f;
			Vect3f l_new = m_Owner->GetPosition();
			
			l_new.y =l_point.y;
			float l_dist = l_new.SqDistance(l_point); 
			if(l_dist > 0.09f)
			{
				//m_Owner->SetPosition(l_new);
			}
			else
			{
				//m_Owner->SetPosition(l_point);
				//m_Owner->SetPosition(m_Owner->GetFront()*0.0f, ElapsedTime);					
				//m_Owner->ChangeCharacterAnimation(WAIT_ANIM, 0.3f);
				l_dir = l_point - m_Owner->GetPosition();
				l_dir.y=0.0f;
				m_Owner->SetPosition(l_dir, ElapsedTime);			
				m_Owner->ChangeCharacterAnimation(WAIT_ANIM, 0.3f);
			}
		}
		else{
			if(!isFacedRun(l_point))
			{
				if(isAtLeft(l_point))
				{					
					m_Owner->SetYaw(m_Owner->GetYaw() + rotationSpeed*ElapsedTime);
				} 
				else{ 				
					m_Owner->SetYaw(m_Owner->GetYaw() - rotationSpeed*ElapsedTime);		
				}
			}
			else{
				Vect3f l_dir = l_point;
				l_dir.y = m_Owner->GetPosition().y;
				l_dir = (l_dir-m_Owner->GetPosition()).Normalize();
				m_Owner->SetYaw(atan2(-l_dir.x, l_dir.z));
			}
		}
	}
	return true;
}

bool CMoveTask::reachCover(int numCover, float ElapsedTime)
{
	Vect3f l_pos = m_Owner->GetPosition();
	Vect3f l_point = CORE->GetCoverManager()->GetExt(numCover);
	if(l_pos.x!=l_point.x || l_pos.z != l_point.z)
	{
		m_Owner->ChangeCharacterAnimation(RUN_ANIM, 0.3f);		
	
		Vect3f l_dir = l_point;
		l_dir.y = l_pos.y;
		l_dir = (l_dir-l_pos).Normalize();

		if(m_bCovered)
		{
			Vect3f l_yaw = (m_Player->GetPosition() - l_pos).Normalize();
			m_Owner->SetYaw(atan2(-l_yaw.x, l_yaw.z));
		}
		else{
			m_Owner->SetYaw(atan2(-l_dir.x, l_dir.z));
		}
		
		Vect3f l_prev= m_Owner->GetPosition();
				
		m_Owner->SetPosition(l_dir*0.090f, ElapsedTime);
		//Vect3f l_new = m_Owner->GetPosition() +m_Owner->GetFront() * ElapsedTime * 3.0f;
		Vect3f l_new = m_Owner->GetPosition();
		
		l_new.y = l_point.y;

		float l_dist = l_new.SqDistance(l_point); 
		if(l_dist > 0.09f)
		{
			//m_Owner->SetPosition(l_new);
		}
		else
		{
			//m_Owner->SetPosition(l_point);
			//l_dir = l_point - m_Owner->GetPosition();
			//l_dir.y=0.0f;
			m_Owner->SetPosition(m_Owner->GetFront()*0.090f, ElapsedTime);
			//m_Owner->SetPosition(l_dir, ElapsedTime);
			m_Owner->ChangeCharacterAnimation(WAIT_ANIM, 0.3f);
			m_bCoverReached=true;
			return true;
		}
	}
	return false;
}

void CMoveTask::Covered(int numCover, float ElapsedTime)
{
	Vect3f l_pos = m_Owner->GetPosition();
	Vect3f l_point = CORE->GetCoverManager()->GetCover(numCover);
	if(l_pos.x!=l_point.x || l_pos.z != l_point.z)
	{
		m_Owner->ChangeCharacterAnimation(RUN_ANIM, 0.3f);		
	
		Vect3f l_dir = l_point;
		l_dir.y = l_pos.y;
		l_dir = (l_dir-l_pos).Normalize();

		Vect3f l_yaw = (m_Player->GetPosition() - l_pos).Normalize();
		m_Owner->SetYaw(atan2(-l_yaw.x, l_yaw.z));						
		
		Vect3f l_prev= m_Owner->GetPosition();
				
		m_Owner->SetPosition(l_dir*0.090f, ElapsedTime);
		//Vect3f l_new = m_Owner->GetPosition() +m_Owner->GetFront() * ElapsedTime * 3.0f;
		Vect3f l_new = m_Owner->GetPosition();
		
		l_new.y = l_point.y;

		float l_dist = l_new.SqDistance(l_point); 
		if(l_dist > 0.09f)
		{
			//m_Owner->SetPosition(l_new);
		}
		else
		{
			//m_Owner->SetPosition(l_point);
			//l_dir = l_point - m_Owner->GetPosition();
			//l_dir.y=0.0f;
			//m_Owner->SetPosition(l_dir, ElapsedTime);
			m_Owner->SetPosition(m_Owner->GetFront()*0.090f, ElapsedTime);
			m_Owner->ChangeCharacterAnimation(WAIT_ANIM, 0.3f);
			m_bCovered=true;
		}
	}
	else{
		m_bCovered = true;
	}
}

bool CMoveTask::isReachable(Vect3f pos)
{
	Vect3f l_up(0.0f, 1.0f, 0.0f);	
	CPhysicUserData * l_user;	
	CPhysicUserData * l_pla = m_Player->GetPhysicUserData();
	SCollisionInfo l_info;
	Vect3f l_fron = m_Owner->GetFront();
	Vect3f l_play = pos;
	Vect3f l_dir = pos - m_Owner->GetPosition();
	float l_distance = l_dir.Length() - 0.5f;
	l_dir.Normalize();
	Vect3f l_right = l_up ^ l_dir;
	Vect3f l_pos = m_Owner->GetPosition();
	l_pos.y = 0.7f;

	l_user = CORE->GetPhysicsManager()->RaycastClosestActor(l_pos +l_dir*0.35f + l_right * 0.35f, l_dir , 0x1, l_info);
	if(l_info.m_fDistance < l_distance)
	{
		return false;
	}

	l_user = CORE->GetPhysicsManager()->RaycastClosestActor(l_pos +l_dir*0.35f - l_right * 0.35f, l_dir, 0x1, l_info);
	if(l_info.m_fDistance < l_distance)
	{
		return false;
	}

	return true;
}

bool CMoveTask::isFacedRun()
{
	Vect3f l_aux = (m_Player->GetPosition() - m_Owner->GetPosition()).Normalize();

	float l_vSuma = m_Owner->GetFront() * l_aux;
	if(l_vSuma < 0.90f)
		return false;
	return true;
};

bool CMoveTask::isFacedRun(Vect3f pos)
{
	Vect3f l_pos = m_Owner->GetPosition();
	Vect3f l_aux = (pos - m_Owner->GetPosition()).Normalize();

	float l_vSuma = m_Owner->GetFront() * l_aux;
	if(l_vSuma < 0.90f)
		return false;
	return true;
}
