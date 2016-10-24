#pragma once
#include "Node.h"
class User :
	public Node
{
private:
	int m_userID;

public:
	User();
	~User();

	int& getUserID() { return m_userID; }
};

