#include "RenderParticlesSceneRendererCommand.h"
#include "RenderManager.h"
#include "ParticleManager.h"
#include "Weapon\BulletManager.h"
#include "Math\Matrix44.h"
#include "Process.h"
#include "Base.h"

CRenderParticlesSceneRendererCommand::CRenderParticlesSceneRendererCommand(CXMLTreeNode &atts)
	: CSceneRendererCommand(atts)
{

}

void CRenderParticlesSceneRendererCommand::Execute(CRenderManager &RM)
{
	if (!CORE->GetProcess()->GetStartGame())
		return;		// evita generación de particulas en GUI

	Mat44f mat;
	mat.SetIdentity();
	RM.SetTransform(mat);
	CORE->GetParticleManager()->Render(&RM, &CORE->GetRenderManager()->GetFrustum());
	CORE->GetBulletManager()->Render(&RM, &CORE->GetRenderManager()->GetFrustum());
}
