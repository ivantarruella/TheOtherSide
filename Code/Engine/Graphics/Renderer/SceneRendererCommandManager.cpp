#include "SceneRendererCommandManager.h"
#include "PoolRenderableObjectTechnique.h"
#include "RenderableObjectsTechniqueManager.h"
#include "RenderManager.h"
#include "Logger.h"
#include "Exception.h"

#include "BeginRenderSceneRendererCommand.h"
#include "EndRenderSceneRendererCommand.h"
#include "ClearSceneRendererCommand.h"
#include "EnableZWriteSceneRendererCommand.h"
#include "EnableZTestSceneRendererCommand.h"
#include "DisableZWriteSceneRendererCommand.h"
#include "DisableZTestSceneRendererCommand.h"
#include "GenerateShadowMapsSceneRendererCommand.h"
#include "BlurHShadowMapsSceneRendererCommand.h"
#include "BlurVShadowMapsSceneRendererCommand.h"
#include "SetRenderTargetSceneRendererCommand.h"
#include "UnsetRenderTargetSceneRendererCommand.h"
#include "RenderSceneSceneRendererCommand.h"
#include "DrawQuadRendererCommand.h"
#include "CaptureFrameBufferSceneRendererCommand.h"
#include "RenderableObjectTechniquesSceneRendererCommand.h"
#include "RenderDebugSceneSceneRendererCommand.h"
#include "RenderDebugLightsSceneSceneRendererCommand.h"
#include "RenderDebugPhysXSceneRendererCommand.h"
#include "RenderDebugCameraSceneRendererCommand.h"
#include "RenderDebugInfoSceneRendererCommand.h"
#include "RenderDebugIASceneRendererCommand.h"
#include "PresentSceneRendererCommand.h"
#include "SetupMatricesSceneRendererCommand.h"
#include "DeferredShadingSceneRendererCommand.h"
#include "ScissorRectSceneRendererCommand.h"
#include "UnsetScissorRectSceneRendererCommand.h"
#include "RenderGUISceneRendererCommand.h"
#include "RenderParticlesSceneRendererCommand.h"
#include "RenderMirrorsSceneRendererCommand.h"
#include "Base.h"

CSceneRendererCommandManager::CSceneRendererCommandManager()
{

}

CSceneRendererCommandManager::~CSceneRendererCommandManager()
{
	Release();
}

void CSceneRendererCommandManager::Load(const std::string &FileName)
{
	CXMLTreeNode parser;
	if (!parser.LoadFile(FileName.c_str()))
	{
		std::string msg_error = "CSceneRendererCommandManager::Load->Error al intentar leer el archivo: " + FileName;
		LOGGER->AddNewLog(ELOG_LEVEL::ELL_ERROR, msg_error.c_str());
		throw CException(__FILE__, __LINE__, msg_error);
	}

	m_FileName = FileName;

	// Obtenemos el nodo "scene_renderer_commands"
	CXMLTreeNode  l_SceneRendererCommandManagerNode = parser["scene_renderer_commands"];
	if (l_SceneRendererCommandManagerNode.Exists())
	{
		// Obtenemos los datos para todos los scene renderer commands
		int l_count_objects = l_SceneRendererCommandManagerNode.GetNumChildren();
		for (int i = 0; i < l_count_objects; ++i)
		{
			if (!l_SceneRendererCommandManagerNode(i).IsComment())
			{
				CSceneRendererCommand* l_RendererSceneCommand = NULL;
				std::string l_CommandName = l_SceneRendererCommandManagerNode(i).GetName();
				
				if (l_CommandName=="begin_scene")
				{
					l_RendererSceneCommand = new CBeginRenderSceneRendererCommand(l_SceneRendererCommandManagerNode(i));
				}
				else if (l_CommandName=="clear_scene")
				{
					l_RendererSceneCommand = new CClearSceneRendererCommand(l_SceneRendererCommandManagerNode(i));
				}
				else if (l_CommandName=="enable_z_write")
				{
					l_RendererSceneCommand = new CEnableZWriteSceneRendererCommand(l_SceneRendererCommandManagerNode(i));
				}
				else if (l_CommandName=="enable_z_test")
				{
					l_RendererSceneCommand = new CEnableZTestSceneRendererCommand(l_SceneRendererCommandManagerNode(i));
				}
				else if (l_CommandName=="disable_z_test")
				{
					l_RendererSceneCommand = new CDisableZTestSceneRendererCommand(l_SceneRendererCommandManagerNode(i));
				}
				else if (l_CommandName=="render_scene")
				{
					l_RendererSceneCommand = new CRenderSceneSceneRendererCommand(l_SceneRendererCommandManagerNode(i));
				}
				else if (l_CommandName=="disable_z_write")
				{
					l_RendererSceneCommand = new CDisableZWriteSceneRendererCommand(l_SceneRendererCommandManagerNode(i));
				}
				else if (l_CommandName=="end_scene")
				{
					l_RendererSceneCommand = new CEndRenderSceneRendererCommand(l_SceneRendererCommandManagerNode(i));
				}
				else if (l_CommandName=="present")
				{
					l_RendererSceneCommand = new CPresentSceneRendererCommand(l_SceneRendererCommandManagerNode(i));
				}
				else if (l_CommandName=="set_pool_renderable_objects_technique")
				{
					l_RendererSceneCommand = new CRenderableObjectTechniquesSceneRendererCommand(l_SceneRendererCommandManagerNode(i));
				}
				else if (l_CommandName=="setup_matrices")
				{
					l_RendererSceneCommand = new CSetupMatricesSceneRendererCommand(l_SceneRendererCommandManagerNode(i));
				}
				else if (l_CommandName=="set_render_target")
				{
					l_RendererSceneCommand = new CSetRenderTargetSceneRendererCommand(l_SceneRendererCommandManagerNode(i));
					
				}
				else if (l_CommandName=="unset_render_target")
				{
					std::string l_RenderTarget = l_SceneRendererCommandManagerNode(i).GetPszProperty("render_target","");
					CSetRenderTargetSceneRendererCommand * l_SetRenderer=NULL;
					l_SetRenderer = (CSetRenderTargetSceneRendererCommand *)m_SceneRendererCommands.GetResource(l_RenderTarget);
					if (l_SetRenderer!=NULL)
					{
						l_RendererSceneCommand = new CUnsetRenderTargetSceneRendererCommand(l_SetRenderer, l_SceneRendererCommandManagerNode(i));
					}
				}
				else if (l_CommandName=="generate_shadow_maps")
				{
					l_RendererSceneCommand = new CGenerateShadowMapsSceneRendererCommand(l_SceneRendererCommandManagerNode(i));
				}
				else if (l_CommandName=="blurH_shadow_maps")
				{
					l_RendererSceneCommand = new CBlurHShadowMapsSceneRendererCommand(l_SceneRendererCommandManagerNode(i));
				}
				else if (l_CommandName=="blurV_shadow_maps")
				{
					l_RendererSceneCommand = new CBlurVShadowMapsSceneRendererCommand(l_SceneRendererCommandManagerNode(i));
				}
				else if (l_CommandName=="capture_frame_buffer")
				{
					l_RendererSceneCommand = new CCaptureFrameBufferSceneRendererCommand(l_SceneRendererCommandManagerNode(i));
				}
				else if (l_CommandName=="render_draw_quad")
				{
					l_RendererSceneCommand = new CDrawQuadRendererCommand(l_SceneRendererCommandManagerNode(i));
				}
				else if (l_CommandName=="render_deferred_shading")
				{
					l_RendererSceneCommand = new CDeferredShadingSceneRendererCommand(l_SceneRendererCommandManagerNode(i));
				}
				else if (l_CommandName=="set_scissor_rect")
				{
					l_RendererSceneCommand = new CScissorRectSceneRendererCommand(l_SceneRendererCommandManagerNode(i));
				}
				else if (l_CommandName=="unset_scissor_rect")
				{
					l_RendererSceneCommand = new CUnsetScissorRectSceneRendererCommand(l_SceneRendererCommandManagerNode(i));
				}
				else if (l_CommandName=="render_gui")
				{
					l_RendererSceneCommand = new CRenderGUISceneRendererCommand(l_SceneRendererCommandManagerNode(i));
				}
				else if (l_CommandName=="render_particles")
				{
					l_RendererSceneCommand = new CRenderParticlesSceneRendererCommand(l_SceneRendererCommandManagerNode(i));
				}
				else if (l_CommandName=="render_mirrors")
				{
					l_RendererSceneCommand = new CRenderMirrorsSceneRendererCommand(l_SceneRendererCommandManagerNode(i));
				}

				// DEBUG:
				else if (l_CommandName=="render_debug_scene")
				{
					l_RendererSceneCommand = new CRenderDebugSceneSceneRendererCommand(l_SceneRendererCommandManagerNode(i));
				}
				else if (l_CommandName=="render_debug_lights")
				{
					l_RendererSceneCommand = new CRenderDebugLightsSceneSceneRendererCommand(l_SceneRendererCommandManagerNode(i));
				}
				else if (l_CommandName=="render_debug_info")
				{
					l_RendererSceneCommand = new CRenderDebugInfoSceneRendererCommand(l_SceneRendererCommandManagerNode(i));
				}
				else if (l_CommandName=="render_debug_camera")
				{
					l_RendererSceneCommand = new CRenderDebugCameraSceneRendererCommand(l_SceneRendererCommandManagerNode(i));
				}
				else if (l_CommandName=="render_debug_physX")
				{
					l_RendererSceneCommand = new CRenderDebugPhysXSceneRendererCommand(l_SceneRendererCommandManagerNode(i));
				}
				else if (l_CommandName == "render_debug_IA")
				{
					l_RendererSceneCommand = new CRenderDebugIASceneRendererCommand(l_SceneRendererCommandManagerNode(i));
				}

				if (l_RendererSceneCommand!=NULL)
				{
					std::string l_Name = l_SceneRendererCommandManagerNode(i).GetPszProperty("name","");
					if (l_Name=="")
						l_Name = l_CommandName + "_" + GetNextName();

					l_RendererSceneCommand->SetName(l_Name);
					m_SceneRendererCommands.AddResource(l_Name, l_RendererSceneCommand);
				}
			}
		}
	}
}

void CSceneRendererCommandManager::Reload()
{
	if (m_FileName != "")
	{
		m_SceneRendererCommands.Destroy();
		Load (m_FileName);
	}
}

void CSceneRendererCommandManager::Execute(CRenderManager &RM)
{
	CVectorMapManager<CSceneRendererCommand>::TVectorResources& l_VectorResources = m_SceneRendererCommands.GetResourcesVector();
	size_t l_Size = l_VectorResources.size();
	
	for (size_t i=0; i<l_Size; ++i)
	{
		l_VectorResources[i]->Execute(RM);
	}
}

void CSceneRendererCommandManager::Release()
{
	m_SceneRendererCommands.Destroy();
}

std::string CSceneRendererCommandManager::GetNextName()
{
	size_t l_Size = m_SceneRendererCommands.GetResourcesVector().size();
	std::stringstream l_Name;
	l_Name << l_Size;
	return l_Name.str();
}
