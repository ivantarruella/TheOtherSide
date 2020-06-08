#include "RenderMirrorsSceneRendererCommand.h"
#include "RenderManager.h"
#include "LogicObjectsManager.h"
#include "CProcess.h"
#include "Base.h"

CRenderMirrorsSceneRendererCommand::CRenderMirrorsSceneRendererCommand(CXMLTreeNode &atts)
	: CSceneRendererCommand(atts)
{
}

void CRenderMirrorsSceneRendererCommand::Execute(CRenderManager &RM)
{
	if (!CORE->GetProcess()->GetStartGame())
		return;		// evita generación de sombras en GUI
	
	CCamera *l_Camera=CORE->GetCamera();
	if (l_Camera==NULL) 
		return;		// si la camara no esta disponible, no pintamos espejos aún

	CORE->GetLogicObjectsManager()->RenderMirrors();
}
