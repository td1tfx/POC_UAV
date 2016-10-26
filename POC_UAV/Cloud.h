#pragma once
#include "Node.h"
class Cloud :
	public Node
{
private:
	float m_transRange = 80;

public:
	Cloud();
	~Cloud();

	float& getTransRange() { return m_transRange; }
};

