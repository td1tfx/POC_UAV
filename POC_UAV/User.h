#pragma once
#include "Node.h"
class User :
	public Node
{
private:
	int m_userID;
	float m_transRange = 30;
	float m_interestTime = 1;
	d_matrix* m_routingMatrix;
	d_matrix* m_shortRouting;

public:
	User();
	~User();

	int& getUserID() { return m_userID; }
	float& getTransRange() { return m_transRange; }
};

