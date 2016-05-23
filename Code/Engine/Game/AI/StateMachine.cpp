#include "StateMachine.h"
#include "State.h"
#include "EscapeState.h"
#include "Player.h"

#include "Base.h"

CStateMachine::~CStateMachine()
{
	std::map<std::string, CState*>::iterator it = m_States.begin();
	for(; it!=m_States.end(); it++)
	{
		CHECKED_DELETE(it->second);
		//m_States.erase(it);
	}
	m_States.clear();
}

void CStateMachine::Create()
{
	
}

void CStateMachine::Update(float ElapsedTime)
{
	m_Current->Update(ElapsedTime);
}

void CStateMachine::Init()
{

}

void CStateMachine::Reset()
{
	std::map<std::string, CState*>::iterator it = m_States.begin();
	for(; it!=m_States.end(); it++)
	{
		it->second->Reset();
	}
	SetInitState("IDLE");
}

void CStateMachine::ChangeState(const std::string &state)
{
	
	std::map<std::string, CState*>::iterator l_it;
	l_it = m_States.find(state);

	

	m_Current = l_it->second;
	m_Current->Init();
	if(state == "ESCAPE" && m_currentString !="ESCAPE")
	{
		((CEscapeState *)m_Current)->SetPrevious(m_currentString);
	}
	m_currentString = state;
}

void CStateMachine::AddState(CState* state)
{
	m_States.insert(std::pair<std::string, CState*>(state->getName(),state));
	state->setStateMachine(this);
}

void CStateMachine::SetInitState(const std::string &state)
{
	std::map<std::string, CState*>::iterator l_it;
	l_it = m_States.find(state);
	m_currentString = state;

	m_Current = l_it->second;
	m_Current->Init();
}

void CStateMachine::SetPlayer(CPlayer * player)
{
	std::map<std::string, CState*>::iterator it = m_States.begin();
	for(; it!=m_States.end(); it++)
	{
		it->second->SetPlayer(player);
	}
}
