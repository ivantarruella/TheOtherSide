#include "UnsetRenderTargetSceneRendererCommand.h"
#include "SetRenderTargetSceneRendererCommand.h"
#include "RenderManager.h"
#include "Core.h"

#include "Base.h"

CUnsetRenderTargetSceneRendererCommand::CUnsetRenderTargetSceneRendererCommand(CSetRenderTargetSceneRendererCommand *SetRenderTargetRendererCommand, CXMLTreeNode &atts)
	: CSceneRendererCommand(atts)
{
	m_SetRenderTargetRendererCommand = SetRenderTargetRendererCommand;

}

void CUnsetRenderTargetSceneRendererCommand::Execute(CRenderManager &RM)
{
	m_SetRenderTargetRendererCommand->UnsetRenderTarget();
}