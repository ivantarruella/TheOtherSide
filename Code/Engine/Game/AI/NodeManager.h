#pragma once
#ifndef INC_NODEMANAGER_H_
#define INC_NODEMANAGER_H_

#include "Vector3.h"
#include "Node.h"
#include <vector>

class CXMLTreeNode;

class CNodeManager
{
public:
	CNodeManager();
	~CNodeManager();

	void Load(const std::string &FileName);
	void Release();

	std::vector<CNode*> AStarAlgorithm(CNode* start, CNode* end);
	std::vector<CNode*> AStarAlgorithmSoldier(CNode* start, CNode* end);

	int NearestNode(const Vect3f &vector);
	int NearestNodeSoldier(const Vect3f &vector);

	CNode* GetNode(int i){return m_Nodes[i];};
	CNode* GetNodeSoldier(int i){return m_NodesSoldier[i];};

	int GetIndexNode(CNode* node);
	int GetIndexNodeSoldier(CNode* node);

	void AddNode(CNode* node);
	void AddNodeSoldier(CNode* node);

	void SetConnection(CNode* a, CNode* b);

	int GetNumNodes(){return m_Nodes.size();};
	int GetNumNodesSoldier(){return m_NodesSoldier.size();};
		
private:
	
	std::string m_Path;
	std::vector<CNode *> m_Nodes;
	std::vector<CNode *> m_NodesSoldier;
	bool ContainNode(const std::vector<CNode*> &vector, CNode* node);

	std::vector<CNode*> ReconstructPath(const std::vector<int> &path, CNode* node);
	std::vector<CNode*> ReconstructPathSoldier(const std::vector<int> &path, CNode* node);
};

#endif