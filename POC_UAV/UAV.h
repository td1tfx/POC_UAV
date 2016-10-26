#pragma once
#include "Node.h"
class UAV :
	public Node
{
private:
	int m_UAVID;
	float m_transRange = 70;
	bool m_cloudlet = false;

public:
	UAV();
	~UAV();

	int& getUAVID() { return m_UAVID; }
	float& getTransRange() { return m_transRange; }
	bool isCloudlet() { return m_cloudlet; }
};

