#include "SetupMatricesSceneRendererCommand.h"
#include "RenderManager.h"
#include "Base.h"

CSetupMatricesSceneRendererCommand::CSetupMatricesSceneRendererCommand(CXMLTreeNode &atts)
	: CSceneRendererCommand(atts)
{

}

void CSetupMatricesSceneRendererCommand::Execute(CRenderManager &RM)
{
	CCamera* l_Camera = CORE->GetCamera();
	if (l_Camera != NULL)
	{
		RM.SetupMatrices(l_Camera);
	}
}