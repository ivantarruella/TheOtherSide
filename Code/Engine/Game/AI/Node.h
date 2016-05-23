#pragma once
#ifndef INC_NODE_H_
#define INC_NODE_H_

#include "Vector3.h"
#include <vector>

class CXMLTreeNode;

class CNode
{
public:
	CNode(CXMLTreeNode &parser);
	~CNode();

	void SetPos(Vect3f pos){m_Pos = pos;};
	Vect3f GetPos(){return m_Pos;};
	void AddConnection(CNode * node);
	CNode* GetConnection(int i);
	int NumConnections(){return m_Connects.size();}
	float GetDistance(Vect3f pos);
	float GetSqDistance(Vect3f pos);

private:
	Vect3f m_Pos;
	std::vector<CNode *> m_Connects;

	void Release();

};

#endif