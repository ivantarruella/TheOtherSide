#pragma once

#include "Utils/VectorMapManager.h"
#include "BillboardAnimation.h"


class CCamera;
class CRenderManager;
class CFrustum;

class CBillboardManager : public std::vector<CBillboardAnimation*>
{
private:
	std::string m_Path;
	CVectorMapManager<CBillboardAnimation> m_BillboardAnimationCores;
	CVectorMapManager<CBillboard> m_BillboardCores;

public:
	CBillboardManager(void);
	
	~CBillboardManager(void);
	void Destroy();
	void Reload();
	void Load(const std::string &FileName);
	void Render(CRenderManager *RenderManager, const CFrustum* Frustum);
	void Update(float ElapsedTime);
	CBillboard* GetBillboardCore(std::string BillboardName);
};

