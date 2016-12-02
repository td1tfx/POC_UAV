#pragma once
#include "Node.h"
class User :
	public Node
{
private:
	int m_userID;
	float m_footSpeed = 1.5;
	float m_carSpeed = 18;
	float m_transRange = 30;
	float m_interestTime = 1;
	d_matrix* m_routingMatrix;
	d_matrix* m_shortRouting;

public:
	User();
	~User();

	int& getUserID() { return m_userID; }
	float& getTransRange() { return m_transRange; }
	
	void moveRandomByFoot();
	void moveRandomByCar();
	void moveByFoot(venue* t_venue);
	void moveByCar(venue* t_venue)
};

