#include "Node.h"
#include "XML\XMLTreeNode.h"

#include "Base.h"

CNode::CNode(CXMLTreeNode &parser)
{
	m_Connects.clear();

	m_Pos = parser.GetVect3fProperty("pos", Vect3f(0.0f,0.0f,0.0f));
}

CNode::~CNode()
{
	Release();
}

void CNode::Release()
{
	/*for(unsigned int i=0; i < m_Connects.size(); i++)
	{
		CHECKED_DELETE(m_Connects[i]);
	}
	m_Connects.clear();*/
}

void CNode::AddConnection(CNode * node)
{
	m_Connects.push_back(node);
}

CNode* CNode::GetConnection(int i)
{
	return m_Connects[i];
}

float CNode::GetDistance(Vect3f pos)
{
	return m_Pos.Distance(pos);
}

float CNode::GetSqDistance(Vect3f pos)
{
	return m_Pos.SqDistance(pos);
}