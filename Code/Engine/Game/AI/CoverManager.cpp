#include "CoverManager.h"
#include "XML\XMLTreeNode.h"
#include "Exception.h"

#include "Base.h"

CCoverManager::CCoverManager()
{
	
}

void CCoverManager::Load(const std::string &FileName)
{
	CXMLTreeNode parser;
	if (!parser.LoadFile(FileName.c_str()))
	{
		std::string msg_error = "CCoverManager::Load->Error al intentar leer el archivo: " + FileName;
		LOGGER->AddNewLog(ELL_ERROR, msg_error.c_str());
		throw CException(__FILE__, __LINE__, msg_error);
	}
	
	m_Path = FileName;

	int l_count_objects;
	
	CXMLTreeNode  l_CoverManager = parser["CoverManager"];
	if (l_CoverManager.Exists())
	{
		l_count_objects = l_CoverManager.GetNumChildren();
		for (int i = 0; i < l_count_objects; ++i)
		{
			Vect3f l_cover = l_CoverManager(i).GetVect3fProperty("pos", Vect3f(0.0f,0.0f,0.0f));
			Vect3f l_ext = l_CoverManager(i).GetVect3fProperty("ext", Vect3f(0.0f,0.0f,0.0f));

			m_Covers.push_back(l_cover);
			m_ExtCovers.push_back(l_ext);
		}
	}
}

CCoverManager::~CCoverManager()
{
	Release();
}

void CCoverManager::Release()
{
	m_Covers.clear();
	m_ExtCovers.clear();
}

int CCoverManager::NearestCover(const Vect3f &vector)
{
	Vect3f l_pos = vector;
	l_pos.y = m_ExtCovers[0].y;
	float l_minDistance=1000.0f;
	float l_distance;
	int n=0;
	for(unsigned int i=0; i<m_Covers.size(); i++)
	{
		l_distance = m_Covers[i].Distance(l_pos);
		if(l_distance < l_minDistance)
		{
			n=i;
			l_minDistance=l_distance;
		}
	}
	return n;
}

int CCoverManager::NearestExt(const Vect3f &vector)
{
	Vect3f l_pos = vector;
	l_pos.y = m_ExtCovers[0].y;
	float l_minDistance=1000.0f;
	float l_distance;
	int n=0;
	for(unsigned int i=0; i<m_ExtCovers.size(); i++)
	{
		l_distance = m_ExtCovers[i].Distance(l_pos);
		if(l_distance < l_minDistance)
		{
			n=i;
			l_minDistance=l_distance;
		}
	}
	return n;
}