#include "BeginRenderSceneRendererCommand.h"
#include "SceneRendererCommand.h"
#include "RenderManager.h"
#include "Base.h"

CBeginRenderSceneRendererCommand::CBeginRenderSceneRendererCommand(CXMLTreeNode &atts)
	: CSceneRendererCommand(atts)
{
}

void CBeginRenderSceneRendererCommand::Execute(CRenderManager &RM)
{
	// Begin the scene
	HRESULT hr= RM.GetDevice()->BeginScene();
    assert( SUCCEEDED( hr ) );

    //RM.GetDevice()->SetRenderState( D3DRS_CULLMODE, D3DCULL_CCW);
    RM.GetDevice()->SetRenderState( D3DRS_LIGHTING, FALSE );
    RM.GetDevice()->SetRenderState( D3DRS_DITHERENABLE, TRUE );
    RM.GetDevice()->SetRenderState( D3DRS_SPECULARENABLE, FALSE );

	if(RM.GetPaintSolid())
    {
   		RM.GetDevice()->SetRenderState( D3DRS_FILLMODE, D3DFILL_SOLID  );
    }
    else
    {
   		RM.GetDevice()->SetRenderState( D3DRS_FILLMODE, D3DFILL_WIREFRAME  );
    }
}