#pragma once
#ifndef INC_STATEMACHINE_H_
#define INC_STATEMACHINE_H_

#include <map>
#include <string>

class CState;
class CPlayer;

class CStateMachine
{
public:
	CStateMachine()
		:m_Current(NULL), m_currentString("")	
	{
	};
	~CStateMachine();

	void Create();
	void Update(float ElapsedTime);
	void Init();
	void Reset();
	
	void ChangeState(const std::string &state);
	void AddState(CState* state);
	void SetInitState(const std::string &state);
	void SetPlayer(CPlayer * player);

private:
	CState * m_Current;
	std::string m_currentString;
	std::map <std::string, CState*> m_States;
};

#endif