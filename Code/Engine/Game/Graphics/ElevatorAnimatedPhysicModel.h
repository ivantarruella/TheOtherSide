#pragma once
#include "ElevatorAnimatedPhysicModel.h"
#include "AnimatedInstanceModel.h"


class CXMLTreeNode;
class CAnimatedCoreModel;

class CElevatorAnimatedPhysicModel : public CAnimatedInstanceModel
{
public:
	CElevatorAnimatedPhysicModel();
	CElevatorAnimatedPhysicModel(const CXMLTreeNode& XmlData, CAnimatedCoreModel* coreModel);
	virtual ~CElevatorAnimatedPhysicModel();
};

