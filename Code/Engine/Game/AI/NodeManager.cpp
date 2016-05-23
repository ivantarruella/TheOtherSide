#include "NodeManager.h"
#include "XML\XMLTreeNode.h"
#include "Exception.h"

#include "Base.h"

CNodeManager::CNodeManager()
{
	
}

void CNodeManager::Load(const std::string &FileName)
{
	CXMLTreeNode parser;
	if (!parser.LoadFile(FileName.c_str()))
	{
		std::string msg_error = "CNodeManager::Load->Error al intentar leer el archivo: " + FileName;
		LOGGER->AddNewLog(ELL_ERROR, msg_error.c_str());
		throw CException(__FILE__, __LINE__, msg_error);
	}
	
	m_Path = FileName;

	int l_count_objects;
	
	CXMLTreeNode  l_NodeManager = parser["NodeManager"];
	if (l_NodeManager.Exists())
	{
		CNode* l_Node;
		CXMLTreeNode  l_nodesNode = parser["Nodes"];
		if (l_nodesNode.Exists())
		{

			l_count_objects = l_nodesNode.GetNumChildren();
			for (int i = 0; i < l_count_objects; ++i)
			{
				l_Node = new CNode(l_nodesNode(i));

				AddNode(l_Node);
			}
		}

		l_nodesNode = parser["Connections"];
		if (l_nodesNode.Exists())
		{
			l_count_objects = l_nodesNode.GetNumChildren();
			for (int i = 0; i < l_count_objects; ++i)
			{
				int index1, index2;
				index1 = l_nodesNode(i).GetIntProperty("id1", 0);
				index2 = l_nodesNode(i).GetIntProperty("id2", 0);
				
				SetConnection(m_Nodes[index1], m_Nodes[index2]);
			}
		}
		
		l_nodesNode = parser["NodesSoldier"];
		if (l_nodesNode.Exists())
		{
			l_count_objects = l_nodesNode.GetNumChildren();
			for (int i = 0; i < l_count_objects; ++i)
			{
				l_Node = new CNode(l_nodesNode(i));

				AddNodeSoldier(l_Node);
			}
		}

		l_nodesNode = parser["ConnectionsSoldier"];
		if (l_nodesNode.Exists())
		{
			l_count_objects = l_nodesNode.GetNumChildren();
			for (int i = 0; i < l_count_objects; ++i)
			{
				int index1, index2;
				index1 = l_nodesNode(i).GetIntProperty("id1", 0);
				index2 = l_nodesNode(i).GetIntProperty("id2", 0);
				
				SetConnection(m_NodesSoldier[index1], m_NodesSoldier[index2]);
			}
		}
	}
}

CNodeManager::~CNodeManager()
{
	Release();
}

void CNodeManager::Release()
{
	//std::map<Vect3f,CNode*>::iterator it = m_Nodes.begin();
	for(unsigned int i=0; i < m_Nodes.size(); i++)
	{
		CHECKED_DELETE(m_Nodes[i]);
	}
	m_Nodes.clear();
	for(unsigned int i=0; i < m_NodesSoldier.size(); i++)
	{
		CHECKED_DELETE(m_NodesSoldier[i]);
	}
	m_NodesSoldier.clear();
}

void CNodeManager::AddNode(CNode* node)
{
	m_Nodes.push_back(node);
}

void CNodeManager::AddNodeSoldier(CNode* node)
{
	m_NodesSoldier.push_back(node);
}

void CNodeManager::SetConnection(CNode* a, CNode* b)
{
	a->AddConnection(b);
	b->AddConnection(a);
}

int CNodeManager::GetIndexNode(CNode* node)
{
	for(unsigned int i=0; i<m_Nodes.size(); i++)
	{
		if(m_Nodes[i] == node)
			return i;
	}
	return -1;
}

int CNodeManager::GetIndexNodeSoldier(CNode* node)
{
	for(unsigned int i=0; i<m_NodesSoldier.size(); i++)
	{
		if(m_NodesSoldier[i] == node)
			return i;
	}
	return -1;
}

int CNodeManager::NearestNode(const Vect3f &vector)
{
	float distsq;
	int aux=0;
	if(m_Nodes.size() >0)
	{
		distsq = m_Nodes[0]->GetSqDistance(vector);
		for(unsigned int i=1; i<m_Nodes.size(); i++)
		{
			float prev = m_Nodes[i]->GetSqDistance(vector);
			if(prev < distsq)
			{
				distsq = prev;
				aux = i;
			}
		}
		return aux;
	}
	else
	{
		return -1;
	}
}

int CNodeManager::NearestNodeSoldier(const Vect3f &vector)
{
	float distsq;
	int aux=0;
	if(m_NodesSoldier.size() >0)
	{
		distsq = m_NodesSoldier[0]->GetSqDistance(vector);
		for(unsigned int i=1; i<m_NodesSoldier.size(); i++)
		{
			float prev = m_NodesSoldier[i]->GetSqDistance(vector);
			if(prev < distsq)
			{
				distsq = prev;
				aux = i;
			}
		}
		return aux;
	}
	else
	{
		return -1;
	}
}

std::vector<CNode*> CNodeManager::ReconstructPath(const std::vector<int> &path, CNode* node)
{
	int l_index = GetIndexNode(node);
	std::vector<CNode*> l_path;
	if(path[l_index] != -2)
	{
		CNode* l_node = m_Nodes[path[l_index]];
		l_path = ReconstructPath(path, l_node);
		l_path.push_back(node);
		return l_path;
	}
	else
	{
		l_path.push_back(node);
		return l_path;
	}
}

std::vector<CNode*> CNodeManager::ReconstructPathSoldier(const std::vector<int> &path, CNode* node)
{
	int l_index = GetIndexNodeSoldier(node);
	std::vector<CNode*> l_path;
	if(path[l_index] != -2)
	{
		CNode* l_node = m_NodesSoldier[path[l_index]];
		l_path = ReconstructPathSoldier(path, l_node);
		l_path.push_back(node);
		return l_path;
	}
	else
	{
		l_path.push_back(node);
		return l_path;
	}
}

std::vector<CNode*> CNodeManager::AStarAlgorithm(CNode* start, CNode* end)
{
	std::vector<CNode*> l_notVisited;
	std::vector<CNode*> l_Visited;
	std::vector<CNode*> path;

	if(m_Nodes.size()==0)
	{
		path.clear();
		return path;
	}
	
	std::vector<int> l_path;
	for(unsigned int i=0; i<m_Nodes.size(); i++)
	{
		l_path.push_back(-1);
	}
	
	std::vector<float> caminoRecorrido;
	for(unsigned int i=0; i<m_Nodes.size(); i++)
	{
		caminoRecorrido.push_back(-1);
	}
	
	std::vector<float> estim;
	for(unsigned int i=0; i<m_Nodes.size(); i++)
	{
		estim.push_back(-1);
	}

	int l_startIndex = GetIndexNode(start);
	caminoRecorrido[l_startIndex]=0;
	estim[l_startIndex]= start->GetDistance(end->GetPos())/2;
	l_path[l_startIndex]=-2;

	l_notVisited.push_back(start);
	//l_path.push_back(start);

	while(l_notVisited.size() > 0)
	{
		CNode* l_current;
		float l_best;
		float l_offset = 0;
		int l_index;
		int l_realPosition;
		for(unsigned int i=0; i<l_notVisited.size(); i++)
		{
			l_realPosition = GetIndexNode(l_notVisited[i]);

			/*float l_distGoal = end->GetSqDistance(l_notVisited[i]->GetPos());
			float l_distStart= caminoRecorrido[l_realPosition] + l_path[l_path.size()-1]->GetSqDistance(l_notVisited[i]->GetPos());*/
			if(i==0)
			{
				l_best=estim[l_realPosition];
				l_index=0;
				//l_offset = l_distStart;
			}
			if(estim[l_realPosition] < l_best)
			{
				l_best=estim[l_realPosition];
				l_index=i;
				//l_offset = l_distStart;
			}
		}

		l_current = l_notVisited[l_index];
		if(l_current == end)
		{
			//return lista de nodos;
			path = ReconstructPath(l_path,l_current);		
			return path;
		}

		std::vector<CNode*>::iterator it = l_notVisited.begin();
		for(int i=0; i<l_index; i++)
			it++;
		l_notVisited.erase(it);

		l_Visited.push_back(l_current);
		l_realPosition = GetIndexNode(l_current);

		for(int i=0; i<l_current->NumConnections(); i++)
		{
			CNode * neighbor = l_current->GetConnection(i);
			if(!ContainNode(l_Visited, neighbor))
			{
				float l_tentative = caminoRecorrido[l_realPosition] + l_current->GetSqDistance(neighbor->GetPos());
				int neighborIndex = GetIndexNode(neighbor);
				
				if(!ContainNode(l_notVisited, neighbor) || (l_tentative<caminoRecorrido[neighborIndex]))
				{
					l_notVisited.push_back(neighbor);
					caminoRecorrido[neighborIndex] = l_tentative;
					l_path[neighborIndex] = l_realPosition;
					estim[neighborIndex]= caminoRecorrido[neighborIndex] + start->GetDistance(end->GetPos())/2;
				}
			}
		}
	}
	return path;
}

std::vector<CNode*> CNodeManager::AStarAlgorithmSoldier(CNode* start, CNode* end)
{
	std::vector<CNode*> l_notVisited;
	std::vector<CNode*> l_Visited;
	std::vector<CNode*> path;

	if(m_NodesSoldier.size()==0)
	{
		path.clear();
		return path;
	}
	
	std::vector<int> l_path;
	for(unsigned int i=0; i<m_NodesSoldier.size(); i++)
	{
		l_path.push_back(-1);
	}
	
	std::vector<float> caminoRecorrido;
	for(unsigned int i=0; i<m_NodesSoldier.size(); i++)
	{
		caminoRecorrido.push_back(-1);
	}
	
	std::vector<float> estim;
	for(unsigned int i=0; i<m_NodesSoldier.size(); i++)
	{
		estim.push_back(-1);
	}

	int l_startIndex = GetIndexNodeSoldier(start);
	caminoRecorrido[l_startIndex]=0;
	estim[l_startIndex]= start->GetDistance(end->GetPos())/2;
	l_path[l_startIndex]=-2;

	l_notVisited.push_back(start);
	//l_path.push_back(start);

	while(l_notVisited.size() > 0)
	{
		CNode* l_current;
		float l_best;
		float l_offset = 0;
		int l_index;
		int l_realPosition;
		for(unsigned int i=0; i<l_notVisited.size(); i++)
		{
			l_realPosition = GetIndexNodeSoldier(l_notVisited[i]);

			/*float l_distGoal = end->GetSqDistance(l_notVisited[i]->GetPos());
			float l_distStart= caminoRecorrido[l_realPosition] + l_path[l_path.size()-1]->GetSqDistance(l_notVisited[i]->GetPos());*/
			if(i==0)
			{
				l_best=estim[l_realPosition];
				l_index=0;
				//l_offset = l_distStart;
			}
			if(estim[l_realPosition] < l_best)
			{
				l_best=estim[l_realPosition];
				l_index=i;
				//l_offset = l_distStart;
			}
		}

		l_current = l_notVisited[l_index];
		if(l_current == end)
		{
			//return lista de nodos;
			path = ReconstructPathSoldier(l_path,l_current);		
			return path;
		}

		std::vector<CNode*>::iterator it = l_notVisited.begin();
		for(int i=0; i<l_index; i++)
			it++;
		l_notVisited.erase(it);

		l_Visited.push_back(l_current);
		l_realPosition = GetIndexNodeSoldier(l_current);

		for(int i=0; i<l_current->NumConnections(); i++)
		{
			CNode * neighbor = l_current->GetConnection(i);
			if(!ContainNode(l_Visited, neighbor))
			{
				float l_tentative = caminoRecorrido[l_realPosition] + l_current->GetSqDistance(neighbor->GetPos());
				int neighborIndex = GetIndexNodeSoldier(neighbor);
				
				if(!ContainNode(l_notVisited, neighbor) || (l_tentative<caminoRecorrido[neighborIndex]))
				{
					l_notVisited.push_back(neighbor);
					caminoRecorrido[neighborIndex] = l_tentative;
					l_path[neighborIndex] = l_realPosition;
					estim[neighborIndex]= caminoRecorrido[neighborIndex] + start->GetDistance(end->GetPos())/2;
				}
			}
		}
	}
	return path;
}

bool CNodeManager::ContainNode(const std::vector<CNode*> &vector, CNode* node)
{
	for(unsigned int i=0; i<vector.size(); i++)
	{
		if(vector[i] == node)
			return true;
	}
	return false;
}