#include "Player.h"
#include "InputManager.h"
#include "ActionToInput.h"
#include "LightManager.h"
#include "RenderManager.h"
#include "include\PhysicController.h"
#include "include\PhysicsManager.h"
#include "Base.h"

CPlayer::~CPlayer(){

	CHECKED_DELETE(m_PhysicController);
}
void CPlayer::Render(CRenderManager* rendermanager)
{
	Mat44f t,r;

	t.SetIdentity();
	r.SetIdentity();
	t.Translate(CObject3D::m_Position);
	r.RotByAngleY(-CObject3D::m_fYaw);
	rendermanager->SetTransform(t*r);

	rendermanager->DrawCube(m_Size, m_Color);
}

void CPlayer::Update(float ElapsedTime)
{
	UpdateInputActions(ElapsedTime);
}

void CPlayer::UpdateInputActions(float ElapsedTime)
{
	CActionToInput* actionToInput = CORE->GetActionToInput();
	CLightManager* lightManager = CORE->GetLightManager();
	Vect3f l_DirController(0.0f);
	// Mouse
    float delta;
	
	if (actionToInput->DoAction(DOACTION_PLAYERLOOKLEFT, delta) || actionToInput->DoAction(DOACTION_PLAYERLOOKRIGHT, delta))
    {
		CObject3D::m_fYaw -= (delta*0.02f);
		//lightManager->GetLight(1)->SetYaw(lightManager->GetLight(1)->GetYaw() - delta);
    }
    
	if (actionToInput->DoAction(DOACTION_PLAYERLOOKUP, delta) || actionToInput->DoAction(DOACTION_PLAYERLOOKDOWN, delta))
    {
		CObject3D::m_fPitch -= (delta*0.02f);
		//lightManager->GetLight(1)->SetPitch(lightManager->GetLight(1)->GetPitch() - delta);
    }

	// Keyboard
	bool move_up    = actionToInput->DoAction(DOACTION_PLAYERMOVEUP);
	bool move_down  = actionToInput->DoAction(DOACTION_PLAYERMOVEDOWN);
	bool move_left  = actionToInput->DoAction(DOACTION_PLAYERMOVELEFT);
	bool move_right = actionToInput->DoAction(DOACTION_PLAYERMOVERIGHT);


	Vect3f dir(cos(m_fYaw)*200*ElapsedTime, 0.0f, sin(m_fYaw)*200*ElapsedTime);						// up, down
	Vect3f dir2(cos(m_fYaw+ePIf/2)*200*ElapsedTime, 0.0f, sin(m_fYaw+ePIf/2)*200*ElapsedTime);		// left, right
	
	if (move_up && move_left)
	{
		Vect3f dir3(cos(m_fYaw+ePIf/4)*200*ElapsedTime, 0.0f, sin(m_fYaw+ePIf/4)*200*ElapsedTime);		// up+left
		CObject3D::m_Position += Vect3f(dir3);
		l_DirController=Vect3f(dir3);
		//lightManager->GetLight(1)->SetPosition(lightManager->GetLight(1)->GetPosition() + Vect3f(dir3));
		move_up=false;
		move_left=false;
	}
	if (move_up && move_right)
	{
		Vect3f dir3(cos(m_fYaw-ePIf/4)*200*ElapsedTime, 0.0f, sin(m_fYaw-ePIf/4)*200*ElapsedTime);		// up+right
		CObject3D::m_Position += Vect3f(dir3);
		l_DirController=Vect3f(dir3);
		//lightManager->GetLight(1)->SetPosition(lightManager->GetLight(1)->GetPosition() + Vect3f(dir3));
		move_up=false;
		move_right=false;
	}
	if (move_down && move_left)
	{
		Vect3f dir3(cos(m_fYaw-ePIf/4)*200*ElapsedTime, 0.0f, sin(m_fYaw-ePIf/4)*200*ElapsedTime);		// down+left
		CObject3D::m_Position -= Vect3f(dir3);
		//lightManager->GetLight(1)->SetPosition(lightManager->GetLight(1)->GetPosition() - Vect3f(dir3));
		l_DirController=-Vect3f(dir3);
		move_down=false;
		move_left=false;
	}
	if (move_down && move_right)
	{
		Vect3f dir3(cos(m_fYaw+ePIf/4)*200*ElapsedTime, 0.0f, sin(m_fYaw+ePIf/4)*200*ElapsedTime);		// down+right
		CObject3D::m_Position -= Vect3f(dir3);
		//lightManager->GetLight(1)->SetPosition(lightManager->GetLight(1)->GetPosition() - Vect3f(dir3));
		l_DirController=-Vect3f(dir3);
		move_down=false;
		move_right=false;
	}
	if (move_up)
	{
		CObject3D::m_Position += Vect3f(dir);
		l_DirController=Vect3f(dir);
		//lightManager->GetLight(1)->SetPosition(lightManager->GetLight(1)->GetPosition() + Vect3f(dir));
	}
	if (move_down)
	{
		CObject3D::m_Position -= Vect3f(dir);
		l_DirController=-Vect3f(dir);
		//lightManager->GetLight(1)->SetPosition(lightManager->GetLight(1)->GetPosition() - Vect3f(dir));
	}
	if (move_left)
	{
		CObject3D::m_Position += Vect3f(dir2);
		l_DirController=Vect3f(dir2);
		//lightManager->GetLight(1)->SetPosition(lightManager->GetLight(1)->GetPosition() + Vect3f(dir2));
	}
	if (move_right)
	{
		CObject3D::m_Position -= Vect3f(dir2);
		l_DirController=-Vect3f(dir2);
		//lightManager->GetLight(1)->SetPosition(lightManager->GetLight(1)->GetPosition() - Vect3f(dir2));
	}

	if(m_PhysicController){
	m_PhysicController->Move(l_DirController, ElapsedTime);
		
	}
	
}

void CPlayer::Init(){

	if(m_CreatePhysics){

	if(m_PhysicElement==NULL ){
		std::string str="PlayerPhysics";
		std::string str2="";
		m_PhysicElement= new CPhysicElement(str,str2);
		
	}
	
	

	m_PhysicController = new CPhysicController(3.0f, 6.0f, mathUtils::Deg2Rad(45.0f), 0.1f, 1.0f, 1<<0, GetPhysicUserData(), m_Position);
	CORE->GetPhysicsManager()->AddPhysicController(m_PhysicController);
	
	m_PhysicController->SetVisible(true);
	m_PhysicController->SetCastShadows(m_CastShadows);
	m_PhysicController->SetCreatePhysics(true);
	m_PhysicController->SetCollision(true);
	
	}
	
}