#include "PatrolTask.h"
#include "Task.h"
#include "Character.h"
#include "PhysicsManager.h"
#include "Node.h"
#include "NodeManager.h"
#include "Random.h"
#include "Core.h"
#include "Renderer\RenderableObjectsLayersManager.h"

#include "Base.h"

CPatrolTask::~CPatrolTask()
{
	destroy();
}

bool CPatrolTask::checkCondition(float ElapsedTime)
{
	if(CORE->GetNodeManager()->GetNumNodesSoldier() > 0)
	{
		if(m_Time < 6.0f)
		{
			m_Time = m_Time + ElapsedTime;
			return false;
		}
		else{
			return true;
		}
	}
	return false;
}

void CPatrolTask::doAction(float ElapsedTime)
{
	if(m_Player==NULL)
	{
		m_Player=(CCharacter*)CORE->GetRenderableObjectsLayersManager()->GetResource("solid")->GetInstance("PLAYER");
	}
	if(m_Player==NULL)
		return;

	m_Time = m_Time + ElapsedTime;
	const float rotationSpeed = mathUtils::Deg2Rad( 180.0f );

	if(m_PatrolWay.size() > 0)
	{
		m_Owner->ChangeCharacterAnimation(WALK_ANIM, 0.3f);
		Vect3f l_point = CORE->GetNodeManager()->GetNodeSoldier(m_PatrolWay[m_numPatrol])->GetPos();
		if(!isReachable(l_point))
		{
			if(m_Path.empty())
			{
				CNode * l_start = CORE->GetNodeManager()->GetNodeSoldier(CORE->GetNodeManager()->NearestNodeSoldier(m_Owner->GetPosition()));
				CNode * l_end = CORE->GetNodeManager()->GetNodeSoldier(m_PatrolWay[m_numPatrol]);
				m_Path = CORE->GetNodeManager()->AStarAlgorithmSoldier(l_start, l_end);
			}
			if(!m_Path.empty())
			{
				if(isFaced(m_Path[0]->GetPos()))
				{
					m_Owner->ChangeCharacterAnimation(WALK_ANIM, 0.3f);	
					Vect3f l_dir = m_Path[0]->GetPos();
					l_dir.y = m_Owner->GetPosition().y;
					l_dir = (l_dir-m_Owner->GetPosition()).Normalize();
					m_Owner->SetYaw(atan2(-l_dir.x, l_dir.z));

					Vect3f l_prev= m_Owner->GetPosition();
				
					m_Owner->SetPosition(m_Owner->GetFront()*0.030f, ElapsedTime);
					Vect3f l_new = m_Owner->GetPosition();

					l_new.y = m_Path[0]->GetPos().y;

					float l_dist = l_new.SqDistance(m_Path[0]->GetPos()); 
					if(l_dist > 0.09f)
					{
						//m_Owner->SetPosition(l_new);
					}
					else
					{
						m_Owner->SetPosition(m_Path[0]->GetPos());
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
		else{
			if(isFaced(l_point))
			{
				m_Owner->ChangeCharacterAnimation(WALK_ANIM, 0.3f);
				Vect3f l_dir = l_point;
				l_dir.y = m_Owner->GetPosition().y;
				l_dir = (l_dir-m_Owner->GetPosition()).Normalize();
				m_Owner->SetYaw(atan2(-l_dir.x, l_dir.z));				
				
				Vect3f l_prev= m_Owner->GetPosition();
				
				m_Owner->SetPosition(m_Owner->GetFront()*0.030f, ElapsedTime);
				Vect3f l_new = m_Owner->GetPosition();

				l_new.y =l_point.y;
				float l_dist = l_new.SqDistance(l_point); 
				if(l_dist > 0.09f)
				{
					//m_Owner->SetPosition(l_new);
				}
				else
				{
					m_Owner->SetPosition(l_point);
					
					if(m_ascent)
					{
						if(m_numPatrol == m_PatrolWay.size()-1)
						{
							m_Time = 0.0f;
							m_numPatrol = m_PatrolWay.size()-2;
							m_ascent=false;
						}
						else{
							m_numPatrol++;
						}
					}
					else{
						if(m_numPatrol == 0)
						{
							m_Time = 0.0f;
							m_numPatrol = 1;
							m_ascent=true;
						}
						else{
							m_numPatrol--;
						}
					}
				}
			}
			else{
				if(isAtLeft(l_point))
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
	//else{
	//	if(m_numPatrol < 0)
	//	{
	//		m_numPatrol = CORE->GetNodeManager()->NearestNodeSoldier(m_Owner->GetPosition());
	//		return;
	//	}
	//	else{
	//		m_Owner->ChangeCharacterAnimation(WALK_ANIM, 0.3f);
	//		Vect3f l_point = CORE->GetNodeManager()->GetNodeSoldier(m_numPatrol)->GetPos();
	//		if(isFaced(l_point))
	//		{
	//			m_Owner->ChangeCharacterAnimation(WALK_ANIM, 0.3f);
	//			Vect3f l_dir = l_point;
	//			l_dir.y = m_Owner->GetPosition().y;
	//			l_dir = (l_dir-m_Owner->GetPosition()).Normalize();
	//			m_Owner->SetYaw(atan2(-l_dir.x, l_dir.z));				
	//			
	//			Vect3f l_prev= m_Owner->GetPosition();
	//			
	//			m_Owner->SetPosition(m_Owner->GetFront()*0.030f, ElapsedTime);
	//			//Vect3f l_new = m_Owner->GetPosition() +m_Owner->GetFront() * ElapsedTime * 1.5f;
	//			Vect3f l_new = m_Owner->GetPosition();

	//			l_new.y =l_point.y;
	//			float l_dist = l_new.SqDistance(l_point); 
	//			if(l_dist > 0.09f)
	//			{
	//				//m_Owner->SetPosition(l_new);
	//			}
	//			else
	//			{
	//				m_Owner->SetPosition(l_point);
	//				int l_num = CORE->GetNodeManager()->GetNodeSoldier(m_numPatrol)->NumConnections();
	//				if(l_num == 1)
	//				{
	//					m_Prev = m_numPatrol;
	//					m_numPatrol = CORE->GetNodeManager()->GetIndexNodeSoldier(CORE->GetNodeManager()->GetNodeSoldier(m_numPatrol)->GetConnection(0));
	//				}
	//				else{
	//					int l_aux;
	//					int n;
	//					do{
	//						n = CORE->GetRandom()->getRandUnsigned(0, l_num-1);
	//						l_aux = CORE->GetNodeManager()->GetIndexNodeSoldier(CORE->GetNodeManager()->GetNodeSoldier(m_numPatrol)->GetConnection(n));
	//						if(!isNotBlocked(CORE->GetNodeManager()->GetNodeSoldier(l_aux)->GetPos()))
	//							l_aux=m_Prev;
	//					}while(l_aux == m_Prev);
	//					m_Prev = m_numPatrol;
	//					m_numPatrol = l_aux;
	//				}

	//				m_Count--;
	//				if(m_Count == 0)
	//				{
	//					m_Time = 0.0f;
	//					m_Count = 5;
	//				}
	//			}
	//		}
	//		else{
	//			if(isAtLeft(l_point))
	//			{					
	//				m_Owner->SetYaw(m_Owner->GetYaw() + rotationSpeed*ElapsedTime);
	//			} 
	//			else{ 				
	//				m_Owner->SetYaw(m_Owner->GetYaw() - rotationSpeed*ElapsedTime);	
	//			}
	//			return;
	//		}
	//	}
	//}
}

void CPatrolTask::destroy()
{
	for(unsigned int i=0; i < m_Sons.size(); i++)
	{
		CHECKED_DELETE(m_Sons[i]);
	}
	m_Sons.clear();
}

void CPatrolTask::Reset()
{
	m_Time = 0.0f;
	m_Path.clear();
	m_ascent = false;
}

bool CPatrolTask::isReachable(Vect3f pos)
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

	l_user = CORE->GetPhysicsManager()->RaycastClosestActor(l_pos +l_dir*0.5f + l_right * 0.5f, l_dir , 0x1, l_info);
	if(l_info.m_fDistance < l_distance)
	{
		return false;
	}
	/*if(l_user == m_Player->GetPhysicUserData())
	{
		return true;
	}*/

	l_user = CORE->GetPhysicsManager()->RaycastClosestActor(l_pos +l_dir*0.5f - l_right * 0.5f, l_dir, 0x1, l_info);
	if(l_info.m_fDistance < l_distance)
	{
		return false;
	}
	/*if(l_user == m_Player->GetPhysicUserData())
	{
		return true;
	}*/

	return true;
}
