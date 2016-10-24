#pragma once
#include "Node.h"
class UAV :
	public Node
{
private:
	int m_UAVID;

public:
	UAV();
	~UAV();

	int& getUAVID() { return m_UAVID; }
};

