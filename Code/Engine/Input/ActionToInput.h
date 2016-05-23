#ifndef __CLASS_ACTIONTOINPUT_H__
#define __CLASS_ACTIONTOINPUT_H__

#pragma once

#include "Utils/Types.h"
#include <map>
#include <vector>
#include "ActionToInputDefs.h"

class CInputManager;

struct sInput{
	sInput()
		: DeviceType(""), EventType(""), AxisType(""), Code(""), Delta(0.0f)
	{
	}	

	std::string DeviceType;
	std::string EventType;
	std::string AxisType;
	std::string Code;
	float Delta;
};

typedef std::vector<sInput> vInputs;
typedef std::map< std::string, vInputs> mActions;

class CActionToInput
{
public:
	CActionToInput() 
		: m_bIsOk(false), m_pInputManager(NULL), m_sXMLFile("")
	{
	}
	~CActionToInput()
	{
	}

	bool Init (CInputManager* inputManager);
	bool LoadXML(const std::string& pathFile);
	bool ReLoadXML();
	bool DoAction (const std::string& action_name);
	bool DoAction (const std::string& action_name, float &delta);

	void GetActionInfo(const std::string& action_name, std::string& info, std::string& shortInfo);

private:
	void InitString2Input();
	bool GetCodeFromString(const std::string& input_type, uint32& code) const;

private:
	bool m_bIsOk;
	CInputManager* m_pInputManager;
	std::string		m_sXMLFile;
	std::map<std::string, uint32> m_String2Code;
	mActions m_Actions;
};

#endif