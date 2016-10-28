#pragma once
#include "Node.h"
class Cloud :
	public Node
{
private:
	float m_transRange = 80;
	d_matrix* m_routingMatrix;
	d_matrix* m_shortRouting;

public:
	Cloud();
	~Cloud();

	float& getTransRange() { return m_transRange; }
};

