#include "World.h"
#include "LightManager.h"
#include "ParticleManager.h"
#include "EffectManager.h"
#include "Renderer\RenderableObjectsLayersManager.h"
#include "Scripting\ScriptManager.h"
#include "Base.h"

CWorld::CWorld()
	: m_iWorld(ALL_WORLDS)
{
}

CWorld::~CWorld() 
{
}

void CWorld::ChangeWorld()
{
	if (m_iWorld == REAL_WORLD)
	{
		m_iWorld = MIRROR_WORLD;
		CORE->GetScriptManager()->RunCode("sound_ME()");
	}
	else
	{
		m_iWorld = REAL_WORLD;
		CORE->GetScriptManager()->RunCode("sound_MR()");
	}
}

void CWorld::LoadWorld()
{
	// Disable world geometry & lighting	
	CORE->GetLightManager()->ChangeWorldLighting(CORE->GetRenderManager(), REAL_WORLD, false);
	CORE->GetLightManager()->ChangeWorldLighting(CORE->GetRenderManager(), MIRROR_WORLD, false);

	// Render
	CORE->GetRenderableObjectsLayersManager()->SetLayerEnabled("solid_MR", false);
	CORE->GetRenderableObjectsLayersManager()->SetLayerEnabled("alpha_blend_objects_MR", false);
	CORE->GetRenderableObjectsLayersManager()->SetLayerEnabled("enemies_MR", false);

	CORE->GetRenderableObjectsLayersManager()->SetLayerEnabled("solid_ME", false);
	CORE->GetRenderableObjectsLayersManager()->SetLayerEnabled("alpha_blend_objects_ME", false);
	CORE->GetRenderableObjectsLayersManager()->SetLayerEnabled("enemies_ME", false);
	
	// Physic
	CORE->GetRenderableObjectsLayersManager()->EnablePhysicLayer("solid_MR", false);
	CORE->GetRenderableObjectsLayersManager()->EnablePhysicLayer("solid_ME", false);


	// Lights
	CLight* l_Ambient=NULL;

	switch(m_iWorld) {
	case ALL_WORLDS:
	case REAL_WORLD:
		l_Ambient = CORE->GetLightManager()->GetResource("AMBIENT_MR");
		CORE->GetLightManager()->ChangeWorldLighting(CORE->GetRenderManager(), REAL_WORLD, true);
		break;
	case MIRROR_WORLD:
		l_Ambient = CORE->GetLightManager()->GetResource("AMBIENT_ME");
		CORE->GetLightManager()->ChangeWorldLighting(CORE->GetRenderManager(), MIRROR_WORLD, true);
		break;
	}

	CColor l_Color = l_Ambient->GetColor();
	CORE->GetLightManager()->SetAmbientLight(Vect3f(l_Color.GetRed(), l_Color.GetGreen(), l_Color.GetBlue()));

	// Mirrors
	switch(m_iWorld) {
	case ALL_WORLDS:
	case REAL_WORLD:
		CORE->GetEffectManager()->SetMirrorDarkCoef(0.80f);
		break;		
	
	case MIRROR_WORLD:
		CORE->GetEffectManager()->SetMirrorDarkCoef(0.90f);
		break;
	}

	// Layers
	switch(m_iWorld) {
	case ALL_WORLDS:
	case REAL_WORLD:
		// gfx
		CORE->GetRenderableObjectsLayersManager()->SetLayerEnabled("solid_MR", true);
		CORE->GetRenderableObjectsLayersManager()->SetLayerEnabled("alpha_blend_objects_MR", true);
		CORE->GetRenderableObjectsLayersManager()->SetLayerEnabled("enemies_MR", true);
		// physx
		CORE->GetRenderableObjectsLayersManager()->EnablePhysicLayer("solid_MR", true);
		break;		
	
	case MIRROR_WORLD:
		// gfx
		CORE->GetRenderableObjectsLayersManager()->SetLayerEnabled("solid_ME", true);
		CORE->GetRenderableObjectsLayersManager()->SetLayerEnabled("alpha_blend_objects_ME", true);
		CORE->GetRenderableObjectsLayersManager()->SetLayerEnabled("enemies_ME", true);
		// physx
		CORE->GetRenderableObjectsLayersManager()->EnablePhysicLayer("solid_ME", true);
		break;
	}

	// Particles
	CORE->GetParticleManager()->SetWorld(m_iWorld);
}

