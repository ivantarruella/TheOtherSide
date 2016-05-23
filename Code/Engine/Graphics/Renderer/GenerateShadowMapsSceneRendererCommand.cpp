#include "GenerateShadowMapsSceneRendererCommand.h"
#include "RenderManager.h"
#include "LightManager.h"
#include "Process.h"
#include "Base.h"

CGenerateShadowMapsSceneRendererCommand::CGenerateShadowMapsSceneRendererCommand(CXMLTreeNode &atts)
	: CSceneRendererCommand(atts)
{
}

void CGenerateShadowMapsSceneRendererCommand::Execute(CRenderManager &RM)
{
	if (!CORE->GetProcess()->GetStartGame())
		return;		// evita generación de sombras en GUI
	
	if (CORE->GetCamera()==NULL)
		return;

	CORE->GetLightManager()->GenerateShadowMaps(&RM);
}