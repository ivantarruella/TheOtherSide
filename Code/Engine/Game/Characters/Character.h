#pragma once
#include "AnimatedInstanceModel.h"

class CXMLTreeNode;
class CPhysicController;

enum tPlayerStates {
	IDDLE,
	WALK_FORWARD,
	WALK_BACKWARDS,
	WALK_LEFT,
	WALK_RIGHT,
	WALK_FORWARD_LEFT,
	WALK_FORWARD_RIGHT,
	WALK_BACKWARDS_LEFT,
	WALK_BACKWARDS_RIGHT
};

enum tAnimationStates {
	WAIT_ANIM,
	WALK_ANIM,
	RUN_ANIM,
	AIM_ANIM,
	USE_ANIM,
	SHOOT_ANIM,
	WALK_AIM_ANIM,
	ACTIVATE_MIRROR_ANIM,
	HIT_ANIM,
	RECARGA_ARMA_ANIM,
	RECARGA_ARMA_EST_ANIM,
	DYE_ANIM,
	DEAD_ANIM,
	AIM_UP_ANIM,
	AIM_DOWN_ANIM,
	WALK_AIM_UP_ANIM,
	WALK_AIM_DOWN_ANIM,
	
	NUM_ANIMS
};





class CCharacter :
	public CAnimatedInstanceModel
{
public:
	CCharacter() {};
	CCharacter(const CXMLTreeNode& XmlData, float size,float height);
	CCharacter(const Vect3f &pos, const std::string &CoreName, const std::string &Name, float size,float height);
	virtual ~CCharacter(void);
	// PhysX
	CPhysicController* GetPhysicController(){return m_PhysicController;}
	void SetPhysicController(CPhysicController* controller){m_PhysicController=controller;}

	float GetLife() { return m_fLife; }
	void SetLife(float Life) { m_fLife=Life;}
	void TakeDamage(float Damage);


	void Reload();
	bool Init(const std::string& _coreName);

	virtual void Update(float ElapsedTime);

	void SetPosition(const Vect3f & Position);
	//Controller + modelo animado
	void SetPosition(const Vect3f & Direction, float ElapsedTime);
	const Vect3f& GetPositionIni() { return m_initPosition; }
	void ChangeCharacterAnimation(tAnimationStates NewAnim, float delay, float weight = 1.0f);

	void SetIsHit(bool Hit) {	m_bHit = Hit;}
	bool GetIsHit() { return m_bHit; }
	bool IsHit() {	return m_bHit;}
	void SetIsDead(bool Dead) {	m_bDead = Dead;}
	bool IsDead() {	return m_bDead;}

	void SetHeight(float Height) {m_fHeight=Height;}
	float GetHeight() { return m_fHeight;}
	int GetAnim(){return m_Anim;};

protected:

	float m_fHeight;
	float m_fSkinWidth;

	Vect3f m_fInstancePos;
	Vect3f m_initPosition;

private:
	CPhysicController* m_PhysicController;
	
	tAnimationStates m_Anim;
	float m_Size;

	float m_fLife;
	bool m_bHit, m_bDead;

};

