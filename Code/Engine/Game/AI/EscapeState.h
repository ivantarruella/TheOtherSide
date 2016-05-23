#ifndef INC_ESCAPESTATE_H_
#define INC_ESCAPESTATE_H_

#include <vector>
#include "Vector3.h"
#include "State.h"

class CEscapeState:public CState {
public:
	CEscapeState()
		: isHurted (false)
	{
	};
	
	~CEscapeState(){};

	void Create();
	void Update(float ElapsedTime);
	void Init();
	void Reset();
	void SetPrevious(std::string previous){m_Previous = previous;};

private:
	bool isFaced();
	bool isAtLeft();
	bool isLighted();

	std::string m_Previous;
	bool isHurted;
	uint32	m_Source;
};

#endif