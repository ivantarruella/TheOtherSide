#include "BlurHShadowMapsSceneRendererCommand.h"
#include "RenderManager.h"
#include "LightManager.h"
#include "CProcess.h"
#include "Base.h"

CBlurHShadowMapsSceneRendererCommand::CBlurHShadowMapsSceneRendererCommand(CXMLTreeNode &atts)
	: CSceneRendererCommand(atts)
{
}

void CBlurHShadowMapsSceneRendererCommand::Execute(CRenderManager &RM)
{
	if (!CORE->GetProcess()->GetStartGame())
		return;		// evita generación de sombras en GUI
	
	CORE->GetLightManager()->BlurShadowMapsH(&RM);
}