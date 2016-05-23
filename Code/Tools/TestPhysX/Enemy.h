#pragma once
#ifndef INC_ENEMY_H_
#define INC_ENEMY_H_

#include "Vector3.h"
#include <vector>

class CNode;

class CEnemy
{
public:
	CEnemy();
	~CEnemy();

	Vect3f GetPos(){return m_Pos;};
	void SetPos(const Vect3f &pos){m_Pos=pos;};
	void Update(float deltaTime);
	void SetPath(CNode* start, CNode* end);
	CNode* getTail(){return m_path[m_path.size()-1];};

private:
	Vect3f m_Pos;
	std::vector<CNode*> m_path;
};

#endif
