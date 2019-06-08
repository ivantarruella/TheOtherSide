#include "Character.h"
#include "XML\XMLTreeNode.h"
#include "include\PhysicController.h"
#include "include\PhysicsManager.h"
#include "AnimatedModelsManager.h"

#include "Base.h"

CCharacter::CCharacter(const CXMLTreeNode& XmlData, float size,float height): 
	m_PhysicController(NULL),CAnimatedInstanceModel(XmlData), m_Anim(WAIT_ANIM), m_Size(size),m_bHit(false), m_bDead(false),m_fHeight(height), m_fLife(1.0f)
{
	Init(XmlData.GetPszProperty("core", ""));
}

CCharacter::CCharacter(const Vect3f &pos, const std::string &CoreName, const std::string &Name, float size,float height): 
	m_PhysicController(NULL),CAnimatedInstanceModel(pos, CoreName, Name), m_Anim(WAIT_ANIM), m_Size(size),m_bHit(false), m_bDead(false),m_fHeight(height), m_fLife(1.0f)
{
	Init(CoreName);
}

CCharacter::~CCharacter(void)
{
	if(GetCreatePhysics())
		CHECKED_DELETE(m_PhysicController);
}


void CCharacter::Reload()
{
	CAnimatedInstanceModel::ClearAllAnims();
}

bool CCharacter::Init(const std::string& _coreName)
{
	std::string l_Name = _coreName;
	CAnimatedCoreModel* l_pCoreModel = CORE->GetAnimatedModelsManager()->GetResource(l_Name);
	CAnimatedInstanceModel::Initialize(l_pCoreModel);

	if(GetCreatePhysics())
	{
		std::string l_Type = "CharacterController";
		SetPhysicData(m_Name,l_Type);
		m_fSkinWidth = 0.15f;
		m_fInstancePos = Vect3f(0.f, m_fHeight / 2.0f + m_Size + m_fSkinWidth, 0.f);

		if(GetPhysicUserData())
		{
			float l_StepOffset=0.5f;			//Offset-> altura maxima para subir escalones = 0.5f
			uint32 l_CollisionGroupsMask=(1<<ECG_ESCENARI|1<<ECG_MOBILIARI);	// Mascara, sols col·lisiona amb ECG_ESCENARI (grup 0) i ECG_MOBILIARI (grup 2)

			// Character Controller
			CPhysicController* l_Controller= new CPhysicController(m_Size, m_fHeight, mathUtils::Deg2Rad(45.0f), m_fSkinWidth, l_StepOffset, l_CollisionGroupsMask, GetPhysicUserData(), m_Position+ m_fInstancePos);
			SetPhysicController(l_Controller);
			CORE->GetPhysicsManager()->AddPhysicController(l_Controller);
			l_Controller->SetCollision(true);
			l_Controller->SetScale(GetScale());
			l_Controller->SetYaw(GetYaw());
			l_Controller->SetPitch(GetPitch());
			l_Controller->SetRoll(GetRoll());
			l_Controller->SetVisible(GetVisible());
			l_Controller->SetCastShadows(GetCastShadows());
			l_Controller->SetCreatePhysics(GetCreatePhysics());
			l_Controller->GetUserData()->SetPaint(true);
		}
	}

	CAnimatedInstanceModel::ClearAllAnims();

	return true;
}

void CCharacter::Update(float ElapsedTime){

	if(m_fLife <= 0.0f)
		SetIsDead(true);

	CAnimatedInstanceModel::Update(ElapsedTime);
}

// Teletransportar objeto animado y controller a otra posicion
void CCharacter::SetPosition(const Vect3f & Position)
{
	//seteamos a partir de posicion del player
	if(m_PhysicController!=NULL)
		m_PhysicController->SetPosition(Position + m_fInstancePos);

	CAnimatedInstanceModel::SetPosition(Position);
}

// Mover objeto animado y controller
void CCharacter::SetPosition(const Vect3f & Direction, float ElapsedTime)
{
	if(m_PhysicController!=NULL)
		m_PhysicController->Move(Direction, ElapsedTime);
	
	Vect3f instancePos = m_PhysicController->GetPosition() - m_fInstancePos;
	CAnimatedInstanceModel::SetPosition(instancePos+Direction*ElapsedTime);
}

void CCharacter::ChangeCharacterAnimation(tAnimationStates NewAnim, float delay, float weight)
{
	if (m_Anim != NewAnim)
	{
		ClearCycle(m_Anim, delay);
		BlendCycle(NewAnim, delay, weight);
		m_Anim = NewAnim;
	}
}


void CCharacter::TakeDamage(float Damage)
{
	m_fLife -= Damage;
	SetIsHit(true);
}
