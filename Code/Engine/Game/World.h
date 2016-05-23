#ifndef __CLASS_WORLD_H__
#define __CLASS_WORLD_H__

#pragma once

class CWorld
{
public:
	enum {
		ALL_WORLDS,
		REAL_WORLD,
		MIRROR_WORLD
	};

public:
	CWorld();
	~CWorld();

	int GetWorld() { return m_iWorld; }
	void SetWorld(int world) { m_iWorld = world; }

	void ChangeWorld();
	void LoadWorld();

private:
	int m_iWorld;
};

#endif