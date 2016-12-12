#include "MirrorObject.h"
#include "SpawnPointObject.h"
#include "Renderer\RenderableObjectsLayersManager.h"
#include "LogicObjectsManager.h"
#include "Scripting\ScriptManager.h"
#include "Player.h"
#include "GUIManager.h"
#include "ActionToInput.h"
#include "RenderManager.h"
#include "MeshInstance.h"
#include "Camera\Camera.h"
#include "FreeCamera.h"
#include "EffectManager.h"
#include "ParticleManager.h"
#include "BulletManager.h"
#include "Logger.h"
#include "Math\MathUtils.h"
#include "Base.h"

#define MIRROR_ANIM_TIME	1.0f
#define MIRROR_USE_TIME		3.0f

#define MAX_DIST_MIRROR_RENDER		150.0f

#define FAR_CLIPPING	45.0f
#define NEAR_CLIPPING	0.01f

#define MIRROR_DARK_COEF_ROT	0.0f
#define MIRROR_STEP_CHANGE	0.03f
#define MIRROR_STEP_INI		-0.f

CMirrorObject::CMirrorObject()
	: CLogicObject(), m_MirrorMesh(NULL), m_Layer(""), m_bRotated(false), m_SpawnPointName(""), m_UseItemFile(""), m_bMirrorActive(false),
	m_fUseTime(0.f), m_fAngleRot(0.f), m_fCurrAngle(0.f), m_bLinked(false), m_bStart(false), m_bRotating(false), m_bAllDisabled(false), m_bUseActive(false), 
	m_MirrorTex(NULL), m_bChangeWorld(false), m_player(NULL), m_targetMirror(""), m_rotatedMirror(""), m_fAnimTime(0.0f), m_bMirrorON(true), m_bEnteredTrigger(false),
	m_bCanChangeWorld(true)//, m_MirrorCam(NULL)
{
	m_Type = MIRROR;
	SetEnabled(false);
}

CMirrorObject::CMirrorObject(CXMLTreeNode &atts)
	: CLogicObject(atts), m_MirrorMesh(NULL), m_Layer(""), m_bRotated(false), m_SpawnPointName(""), m_UseItemFile(""), m_bMirrorActive(false),
	m_fUseTime(0.f), m_fAngleRot(0.f), m_fCurrAngle(0.f), m_bLinked(false), m_bStart(false), m_bRotating(false), m_bAllDisabled(false), m_bUseActive(false), 
	m_MirrorTex(NULL), m_bChangeWorld(false), m_player(NULL), m_targetMirror(""), m_rotatedMirror(""), m_fAnimTime(0.0f), m_bMirrorON(true), m_bEnteredTrigger(false),
	m_bCanChangeWorld(true)//, m_MirrorCam(NULL)
{
	m_Type = MIRROR;
	SetEnabled(false);
	m_UseItemFile="UseItem.xml";
	
	m_Layer = atts.GetPszProperty("layer","");
	std::string l_Renderable_Object = atts.GetPszProperty("renderable_object","");
	if (m_Layer!="")
		m_MirrorMesh=(CInstanceMesh*)CORE->GetRenderableObjectsLayersManager()->GetResource(m_Layer)->GetResource(l_Renderable_Object);

	if (m_MirrorMesh==NULL) 
	{
		std::string msg_error = "CMirrorObject::CMirrorObject->No se encuentra renderable object " + l_Renderable_Object + " del espejo!";
		LOGGER->AddNewLog(ELL_ERROR, msg_error.c_str());
	}
	else
	{
		m_SpawnPointName = atts.GetPszProperty("spawn_point","");
		m_fAngleRot = atts.GetFloatProperty("angle_rot", 0.0f);
		m_bLinked = atts.GetBoolProperty("linked", false);

		Vect3f l_center = m_MirrorMesh->GetPosition() + m_MirrorMesh->GetStaticMesh()->getStaticMeshCenter();
		c = D3DXVECTOR3(l_center.x, l_center.y, l_center.z);

		// mirror plane vertexs
		p[0] = atts.GetVect3fProperty("p0",Vect3f(0.0f));
		p[1] = atts.GetVect3fProperty("p1",Vect3f(0.0f));
		p[2] = atts.GetVect3fProperty("p2",Vect3f(0.0f));
		p[3] = atts.GetVect3fProperty("p3",Vect3f(0.0f));
		
		// calculate mirror normal & up vectors
		CalculateMirrorPlane();
		
		// Textura espejo
		m_MirrorTex = new CTexture();
		CTexture::TFormatType l_FormatType =  m_MirrorTex->GetFormatTypeFromString("A8R8G8B8");
		std::string l_MirrorName = GetName() + "_Texture";
		uint32 w,h;
		CORE->GetRenderManager()->GetWidthAndHeight(w,h);	
		m_MirrorTex->Create(l_MirrorName,256,256,1,CTexture::RENDERTARGET,CTexture::DEFAULT,l_FormatType);

		//m_MirrorCam = new CFreeCamera(NEAR_CLIPPING, FAR_CLIPPING,55.0f * FLOAT_PI_VALUE/180.0f, (float)w/(float)h;, l_center);
	}
}

CMirrorObject::~CMirrorObject()
{
	DisplayUse(false, false);

	CHECKED_DELETE(m_MirrorTex);
}

void CMirrorObject::Update(float ElapsedTime)
{
	if (!m_bMirrorON)
		return;

	m_fUseTime -= ElapsedTime;

	UpdateMirrorEnabled();						// Activación/desactivación espejos
	UpdateMirrorRotation(ElapsedTime);			// Rotación espejos
	UpdateMirrorChangeWorld(ElapsedTime);		// Cambios de mundo

	//m_MirrorCam->Update(ElapsedTime);
}

void CMirrorObject::Restart()
{
	m_fCurrAngle = 0.f;
	SetRotated(false);
	RotatePhysXMesh(0.0f);
}

void CMirrorObject::UpdateMirrorEnabled()
{
	if (m_bAllDisabled)
	{
		if (m_fUseTime <= 0.f)
		{
			CORE->GetLogicObjectsManager()->SetAllObjectsEnabled(GetType(), true);
			m_bAllDisabled = false;
			m_fUseTime = MIRROR_USE_TIME;
		}
	}
}

void CMirrorObject::UpdateMirrorRotation(float ElapsedTime)
{
	if (m_fAngleRot != 0.0f)	
	{
		if (!m_bRotated)		
		{
			if (m_bStart)		// Empezamos rotación de estado normal a rotado
			{
				if (m_fAngleRot<0)
				{
					m_LearpAnim.SetValues(0.0f, m_fAngleRot, MIRROR_ANIM_TIME, FUNC_DECREMENT);
				}
				else 
				{
					m_LearpAnim.SetValues(0.0f, m_fAngleRot, MIRROR_ANIM_TIME, FUNC_INCREMENT);
				}
				m_bStart = false;
				m_bRotating = true;
			}

			if (m_bRotating)
			{
				m_LearpAnim.Update(ElapsedTime, m_fCurrAngle);
				if (m_fCurrAngle<0.f || m_fCurrAngle>0.f)
					RotatePhysXMesh(m_fCurrAngle);

				if ((m_fAngleRot<0 && m_fCurrAngle<=m_fAngleRot) || (m_fAngleRot>0 && m_fCurrAngle>=m_fAngleRot))
				{
					RotatePhysXMesh(m_fAngleRot);
					m_bRotated = true;
					m_bRotating = false;
					if (m_fAngleRot != 0.0f)
						CalculateMirrorPlane();
				}
			}
		}
		else		// empezamos rotación de estado rotado a normal
		{
			if (m_bStart)
			{
				if (m_fAngleRot<0)
				{
					m_LearpAnim.SetValues(m_fAngleRot, 0.0f, MIRROR_ANIM_TIME, FUNC_INCREMENT);
				}
				else 
				{
					m_LearpAnim.SetValues(m_fAngleRot, 0.0f, MIRROR_ANIM_TIME, FUNC_DECREMENT);
				}
				m_bStart = false;
				m_bRotating = true;
			}

			if (m_bRotating)
			{
				m_LearpAnim.Update(ElapsedTime, m_fCurrAngle);
				if (m_fCurrAngle<0.f || m_fCurrAngle>0.f)
					RotatePhysXMesh(m_fCurrAngle);

				if (!m_fCurrAngle)
				{
					RotatePhysXMesh(m_fCurrAngle);
					m_bRotated = false;
					m_bRotating = false;
					if (m_fAngleRot != 0.0f)
						CalculateMirrorPlane();
				}
			}
		}
	}
}

void CMirrorObject::UpdateMirrorChangeWorld(float ElapsedTime)
{
	if (m_bChangeWorld)
	{
		m_fAnimTime += ElapsedTime;

		if (m_fAnimTime > 0.1f) 
		{
			CORE->GetEffectManager()->SetMirrorAnimCoef(CORE->GetEffectManager()->GetMirrorAnimCoef() + MIRROR_STEP_CHANGE);
			m_fAnimTime = 0.0f;
		}

		CORE->GetEffectManager()->SetMirrorAnimCoef(CORE->GetEffectManager()->GetMirrorAnimCoef() + MIRROR_STEP_CHANGE);

		if (m_player->GetUseItem())
		{
			Use(m_player, m_targetMirror, m_rotatedMirror);
			CORE->GetLogicObjectsManager()->SetAllObjectsEnabled(GetType(), false);
			CORE->GetEffectManager()->SetMirrorAnimCoef(MIRROR_STEP_INI);
			m_fUseTime = MIRROR_USE_TIME;
			m_bAllDisabled = true;			
			m_bChangeWorld = false;

			m_fAnimTime = 0.0f;
		}
	}
}

void CMirrorObject::DisplayUse(bool show, bool rot)
{
	if (m_UseItemFile!="")
	{
		if (GetEnabled() && show)
		{
			if (!m_bUseActive)
			{
				CORE->GetGUIManager()->GetGUIElement("rotar_espejo")->SetVisible(rot);
				//CORE->GetGUIManager()->PushWindows(m_UseItemFile);	
				CORE->GetGUIManager()->GetGUIElement("use_item")->SetVisible(true);
				m_bUseActive = true;
			}
		}
		else
		{
			if (m_bUseActive)
			{
				//CORE->GetGUIManager()->PopWindows();
				CORE->GetGUIManager()->GetGUIElement("use_item")->SetVisible(false);
				CORE->GetGUIManager()->GetGUIElement("rotar_espejo")->SetVisible(false);
				m_bUseActive = false;
			}
		}
	}
}

void CMirrorObject::Use(CPlayer* player, const std::string& targetMirror,  const std::string& rotatedMirror)
{
	CMirrorObject* l_targetMirror = NULL;
	CMirrorObject* l_rotatedMirror = NULL;

	if (targetMirror!="")
		l_targetMirror = (CMirrorObject*) CORE->GetLogicObjectsManager()->GetResource(targetMirror);
	if (rotatedMirror!="")
		l_rotatedMirror = (CMirrorObject*) CORE->GetLogicObjectsManager()->GetResource(rotatedMirror);

	std::string msg_error="";
	if (!l_targetMirror && targetMirror!="")
		msg_error = "CPlayer::ChangeWorld->Espejo " + targetMirror + " no encontrado !\n";
	if (!l_rotatedMirror && rotatedMirror!="")
		msg_error = "CPlayer::ChangeWorld->Espejo " + rotatedMirror + " no encontrado !\n";

	if (msg_error!="")
		LOGGER->AddNewLog(ELL_ERROR, msg_error.c_str());

	CSpawnPointObject* l_SpawnPoint=NULL;
	std::string l_SpawnPointName="";
	bool l_changeWorld = false;

	if (!m_bRotated && l_targetMirror!=NULL)
	{
		l_SpawnPointName = l_targetMirror->GetMirrorSpawnPoint();
		l_changeWorld = !l_targetMirror->GetRotated();

		l_SpawnPoint = (CSpawnPointObject*) CORE->GetLogicObjectsManager()->GetResource(l_SpawnPointName);

		if (!l_SpawnPoint)
		{
			std::string msg_error = "CPlayer::ChangeWorld->Punto de spawn espejo " + l_SpawnPointName + " no existe!\n";
			LOGGER->AddNewLog(ELL_ERROR, msg_error.c_str());
		}
		else	
		{
			// cambiar player de mundo
			CORE->GetScriptManager()->RunCode("sound_teleport()");
			player->ChangeWorld(l_changeWorld && m_bCanChangeWorld, l_SpawnPoint->GetPosition());
		}
	}
	else if (m_bRotated && l_rotatedMirror!=NULL)
	{
		l_SpawnPointName = l_rotatedMirror->GetMirrorSpawnPoint();
		l_SpawnPoint = (CSpawnPointObject*) CORE->GetLogicObjectsManager()->GetResource(l_SpawnPointName);

		if (!l_SpawnPoint)
		{
			std::string msg_error = "CPlayer::ChangeWorld->Punto de spawn espejo rotado " + l_SpawnPointName + " no existe!\n";
			LOGGER->AddNewLog(ELL_ERROR, msg_error.c_str());
		}
		else	
		{
			// cambiar player de mundo
			CORE->GetScriptManager()->RunCode("sound_teleport()");
			player->ChangeWorld(l_changeWorld && m_bCanChangeWorld, l_SpawnPoint->GetPosition());
		}
	}
}

void CMirrorObject::Rotate(const std::string& targetMirrorName)
{
	if (m_fAngleRot != 0.0f)	// Espejo rotable
	{
		m_bStart = true;
		
		if (m_bLinked && targetMirrorName!="")	// Si espejo linkado con el correspondiente del otro mundo lo rotamos tb
		{
			CMirrorObject* l_targetMirror = (CMirrorObject*) CORE->GetLogicObjectsManager()->GetResource(targetMirrorName);
			if (l_targetMirror == NULL)
			{
				std::string msg_error = "CPlayer::ChangeWorld->Link con espejo " + targetMirrorName + " no encontrado!\n";
				LOGGER->AddNewLog(ELL_ERROR, msg_error.c_str());
			}
			else
			{
				l_targetMirror->SetRotated(!l_targetMirror->GetRotated());
				l_targetMirror->RotatePhysXMesh(l_targetMirror->GetRotated() ? -m_fAngleRot : 0.0f);
				l_targetMirror->SetCurrRotAngle(l_targetMirror->GetRotated() ? -m_fAngleRot : 0.0f);
				l_targetMirror->CalculateMirrorPlane();
			}
		}
	}
}

void CMirrorObject::RotatePhysXMesh(float angle)
{
	if (m_MirrorMesh!=NULL)
	{
		m_MirrorMesh->SetYaw(angle);
		m_MirrorMesh->GetPhysicActor()->SetRotation(Vect3f(0.f, angle, 0.f));
		CalculateMirrorPlane();		// realtime reflection while rotating mirror! ;D
	}
}

void CMirrorObject::Trigger(const std::string& action, CPlayer* player, const std::string& targetMirror, const std::string& rotatedMirror)
{
	if (m_bMirrorON)
	{
		if (GetEnabled() && m_fUseTime <= 0.f)
		{
			bool show_rot = (m_fAngleRot != 0.0f);
			if (action=="OnEnter")
			{
				m_player = player;
				m_bEnteredTrigger = true;
			}
			else if (action=="OnLeave")
			{
				player->SetCanUseItem(false);
				player->SetUseItemAnim(USE_ANIM);
				DisplayUse(false, false);
				m_bChangeWorld = false;
				CORE->GetEffectManager()->SetMirrorAnimCoef(MIRROR_STEP_INI);
				m_bEnteredTrigger = false;
				CORE->GetGUIManager()->GetGUIElement("rotar_espejo")->SetVisible(false);
			}
			else if (action=="OnTrigger")
			{
				m_player = player;

				D3DXVECTOR3 vp(-player->GetFront().x, -player->GetFront().y, -player->GetFront().z);
				if (D3DXVec3Dot(&n, &vp) >= 0.80){
					m_player = player;
					DisplayUse(true, show_rot);
					m_player->SetCanUseItem(true);
					m_player->SetUseItemAnim(ACTIVATE_MIRROR_ANIM);
					m_bChangeWorld = false;
					m_bEnteredTrigger = true;
				}
				else
				{
					player->SetCanUseItem(false);
					player->SetUseItemAnim(USE_ANIM);
					DisplayUse(false, false);
					m_bChangeWorld = false;
					CORE->GetEffectManager()->SetMirrorAnimCoef(MIRROR_STEP_INI);
					m_bEnteredTrigger = false;
					CORE->GetGUIManager()->GetGUIElement("rotar_espejo")->SetVisible(false);
				}

				if (m_bEnteredTrigger)
				{
					m_player->SetCanUseItem(true);
					m_player->SetUseItemAnim(ACTIVATE_MIRROR_ANIM);
					
					// Usar espejo
					m_bChangeWorld = m_player->GetUseButtonPressed();
					if (m_bChangeWorld)
					{
						DisplayUse(false, false);
						
						m_targetMirror = targetMirror;
						m_rotatedMirror = rotatedMirror;
					}
					else 
					{
						DisplayUse(true, show_rot);
						CORE->GetEffectManager()->SetMirrorAnimCoef(MIRROR_STEP_INI);
					}

					// Rotar espejo
					if (CORE->GetActionToInput()->DoAction(DOACTION_PLAYERROTATEMIRROR) || CORE->GetActionToInput()->DoAction(DOACTION_PLAYERROTATEMIRROR_PAD))
					{
						Rotate(targetMirror);
						m_fUseTime = MIRROR_ANIM_TIME;
					}
				}
				else
				{
					DisplayUse(false, false);
					m_player->SetCanUseItem(false);
				}
			}
		}
		else 
		{
			player->SetCanUseItem(false);
			player->SetUseItemAnim(USE_ANIM);
			DisplayUse(false, false);
			m_bChangeWorld = false;
			CORE->GetEffectManager()->SetMirrorAnimCoef(MIRROR_STEP_INI);
			m_bEnteredTrigger = false;
			CORE->GetGUIManager()->GetGUIElement("rotar_espejo")->SetVisible(false);
		}
	}
}


void CMirrorObject::Generate()
{
	if (!m_bMirrorActive || !m_bMirrorON || !m_MirrorMesh || !CORE->GetRenderableObjectsLayersManager()->GetLayerEnabled(m_Layer))
		return;

	float l_Dist = CORE->GetCamera()->GetEye().Distance(m_MirrorMesh->GetPosition());
	if (l_Dist < MAX_DIST_MIRROR_RENDER)
	{
		const CFrustum& l_Frustum = CORE->GetRenderManager()->GetFrustum();		
		float l_Radius=m_MirrorMesh->GetStaticMesh()->getStaticMeshBSRadius();
		Vect3f l_center(c.x, c.y, c.z);

		if(l_Frustum.SphereVisible(l_center, l_Radius))
			GenerateMirror(CORE->GetRenderManager());
	}
}

void CMirrorObject::GenerateMirror(CRenderManager* RM)
{
	Vect3f eye = CORE->GetCamera()->GetEye();
	D3DXVECTOR3 v(eye.x, eye.y, eye.z);

	if (D3DXVec3Dot(&n, &v) > D3DXVec3Dot(&n, &c))
	{
		// build the view matrix for virtual camera space
        //
        D3DXVECTOR3 vc = v - c;
		float dis = D3DXVec3Dot(&vc, &n);			// shortest distance to the plane of the reflecting surface
        D3DXVECTOR3 l_Vp = v - 2.0f * dis * n;		// virtual camera position
        D3DXVECTOR3 l_At = l_Vp + n;				// look at point

        D3DXMATRIX view;
        D3DXMatrixLookAtLH(&view, &l_Vp, &l_At, &u);

		// build the off-center projection matrix
        //
        // vertices in virtual camera space
		Vect3f pView[4];        
		for(int i = 0; i < 4; i++)
            pView[i] = Vect3f(Mat44f(view) * pWorld[i]);

		// min, max coordinate values in virtual camera space
        float xmin = pView[0].x;
        float xmax = pView[0].x;
        float ymin = pView[0].y;
        float ymax = pView[0].y;
        for(int i = 1; i < 4; i++) {
            if (pView[i].x < xmin) xmin = pView[i].x;
            if (pView[i].x > xmax) xmax = pView[i].x;
            if (pView[i].y < ymin) ymin = pView[i].y;
            if (pView[i].y > ymax) ymax = pView[i].y;
        }
        float zmin = dis;
        float zmax = dis + FAR_CLIPPING - NEAR_CLIPPING;

		D3DXMATRIX projection;
        D3DXMatrixPerspectiveOffCenterLH(&projection, xmin, xmax, ymin, ymax, zmin, zmax);

		// update mirror frustum
		m_MirrorFrustum.Update(view * projection);

		// activate virtual camera from mirror
		CORE->GetEffectManager()->ActivateCamera(Mat44f(view), Mat44f(projection), Vect3f(l_Vp.x, l_Vp.y, l_Vp.z));


		// render scene except this mirror
		m_MirrorTex->SetAsRenderTarget(0);

		// render reflected scene
		RM->Clear(true, true, true, 0xffffffff);
		CORE->GetRenderableObjectsLayersManager()->RenderReflected(RM, m_MirrorMesh, &m_MirrorFrustum);

		// render particles from mirror
		//Mat44f mat;
		//mat.SetIdentity();
		//RM->SetTransform(mat);
		//RM->Clear(false, true, false, 0xffffffff);
		//CORE->GetParticleManager()->Render(RM, &m_MirrorFrustum);
		//CORE->GetBulletManager()->Render(RM, &m_MirrorFrustum);

		m_MirrorTex->UnsetAsRenderTarget(0);

		// set mirror texture from virtual camera to mesh
		m_MirrorMesh->GetStaticMesh()->SetTexture(m_MirrorTex);
	}
}

void CMirrorObject::CalculateMirrorPlane()
{
	// calculate plane normal & up vector
	Vect3f v[4];
	for(int i = 0; i < 4; i++)
		v[i] = p[i];

	Vect3f a = v[1]-v[0];
	Vect3f b = v[2]-v[0];
	a.Normalize();
	b.Normalize();
	D3DXVECTOR3 v1(a.x, a.y, a.z);
	D3DXVECTOR3 v2(b.x, b.y, b.z);
	D3DXVec3Cross(&n, &v2, &v1);
	u = D3DXVECTOR3(b.x, b.y, b.z);

	// calculate mirror plane rotation matrix
	Mat44f mat, mat2;
	mat.SetIdentity();
	mat2.SetIdentity();
	mat.RotByAngleY(m_fCurrAngle);
	mat2 = m_MirrorMesh->GetTransform() * mat;
	
	// rotate plane vertexs and normal vector
	Vect3f l_n(n.x, n.y, n.z);
	l_n = mat * l_n;
	n = D3DXVECTOR3(l_n.x, l_n.y, l_n.z);

	for(int i = 0; i < 4; i++)
		v[i] = mat * v[i];

	// transform new plane vertex to world space
	for(int i = 0; i < 4; i++)
		pWorld[i] = Vect3f(mat2 * v[i]);
}

