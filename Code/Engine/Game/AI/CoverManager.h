#pragma once
#ifndef INC_COVERMANAGER_H_
#define INC_COVERMANAGER_H_

#include "Vector3.h"
#include <vector>
#include <string>

class CXMLTreeNode;

class CCoverManager
{
public:
	CCoverManager();
	~CCoverManager();

	void Load(const std::string &FileName);
	void Release();

	int NearestCover(const Vect3f &vector);
	int NearestExt(const Vect3f &vector);

	Vect3f GetCover(int i){return m_Covers[i];};
	Vect3f GetExt(int i){return m_ExtCovers[i];};

	int GetNumCovers(){return m_Covers.size();};
		
private:
	
	std::string m_Path;
	std::vector<Vect3f> m_Covers;
	std::vector<Vect3f> m_ExtCovers;
};

#endif