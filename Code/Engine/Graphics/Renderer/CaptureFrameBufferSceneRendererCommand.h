#pragma once
#ifndef __CLASS_CAPTURE_FRAME_BUFFER_SCENE_RENDERER_COMMAND_H__
#define __CLASS_CAPTURE_FRAME_BUFFER_SCENE_RENDERER_COMMAND_H__

#include "StagedTexturedRendererCommand.h"

class CRenderManager;

class CCaptureFrameBufferSceneRendererCommand : public CStagedTexturedRendererCommand
{
public:
	CCaptureFrameBufferSceneRendererCommand(CXMLTreeNode &atts);
	~CCaptureFrameBufferSceneRendererCommand();

	void Execute(CRenderManager &RM);
};


#endif
