#pragma once
#ifndef INC_AISOLDIER_H_
#define INC_AISOLDIER_H_

#include "AI.h"
#include <string>

class CAISoldier: public CAI 
{
//protected:
//	std::string m_Name;
//
public:
	CAISoldier(){};
	
	~CAISoldier(){};

	void Create();
	void Update(float ElapsedTime);
	void Init();

	/*void SetName(const std::string &name){ m_Name = name;};
	std::string GetName(){return m_Name;};*/

};

#endif