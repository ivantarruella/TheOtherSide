#include "BlurVShadowMapsSceneRendererCommand.h"
#include "RenderManager.h"
#include "LightManager.h"
#include "Process.h"
#include "Base.h"

CBlurVShadowMapsSceneRendererCommand::CBlurVShadowMapsSceneRendererCommand(CXMLTreeNode &atts)
	: CSceneRendererCommand(atts)
{
}

void CBlurVShadowMapsSceneRendererCommand::Execute(CRenderManager &RM)
{
	if (!CORE->GetProcess()->GetStartGame())
		return;		// evita generación de sombras en GUI
	
	CORE->GetLightManager()->BlurShadowMapsV(&RM);
}