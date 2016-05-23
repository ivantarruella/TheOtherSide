#pragma once
#ifndef __CLASS_SCENE_RENDERER_COMMAND_H__
#define __CLASS_SCENE_RENDERER_COMMAND_H__

#include "XML\XMLTreeNode.h"
#include "Named.h"

class CRenderManager;

class CSceneRendererCommand :  public CNamed//,public CUABActive
{
public:	
	CSceneRendererCommand(CXMLTreeNode &atts);
	virtual ~CSceneRendererCommand();

	virtual void Execute(CRenderManager &RM) = 0;
};

#endif