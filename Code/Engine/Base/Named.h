#ifndef __CLASS_NAMED_H__
#define __CLASS_NAMED_H__

#pragma once

#include <string>

class CNamed
{
protected:
	std::string m_Name;

public:
	CNamed(void) 
		: m_Name("")
	{
	}
CNamed(const std::string& Name) 
		: m_Name(Name)
	{
	}	virtual ~CNamed(void)
	{
	}

	inline const std::string& GetName() const {return m_Name;}
	inline void SetName(const std::string& Name) {m_Name=Name;}
};

#endif