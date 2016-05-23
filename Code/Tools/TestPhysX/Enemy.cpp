#include "Enemy.h"
#include "Node.h"
#include "NodeManager.h"
#include "Random.h"
#include "Core.h"

#include "Base.h"

CEnemy::CEnemy()
	:m_Pos(Vect3f(0.0f, 0.0f, 0.0f))
{
}

CEnemy::~CEnemy()
{
}

void CEnemy::Update(float deltaTime)
{
	float l_dist = 0;
	if(m_path.size()>0)
	{
		l_dist = m_path[0]->GetDistance(m_Pos);
		if(l_dist < 1)
		{
			if(m_path.size()>0)
			{
				std::vector<CNode*>::iterator it = m_path.begin();
				m_path.erase(it);
			}
		}
	}
	if(m_path.size()>0)
	{
		Vect3f l_dir = m_path[0]->GetPos() - m_Pos;
		l_dir.Normalize();
		m_Pos = m_Pos + l_dir*deltaTime*8;
	}
	else{
		CNode* l_start;
		CNode* l_end;
		int l_aux = CORE->GetNodeManager()->NearestNode(m_Pos);
		int l_size = l_aux;
		
		while(l_aux == l_size)
		{
			l_size = CORE->GetRandom()->getRandUnsigned(0,CORE->GetNodeManager()->GetNumNodes()-1);
		}
		l_start=CORE->GetNodeManager()->GetNode(l_aux);
		l_end=CORE->GetNodeManager()->GetNode(l_size);
		SetPath(l_start, l_end);
	}
}

void CEnemy::SetPath(CNode* start, CNode* end)
{
	m_path = CORE->GetNodeManager()->AStarAlgorithm(start, end);
}

