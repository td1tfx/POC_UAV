#include "User.h"



User::User()
{
	int maxRow = Config::getInstance()->getMaxRow()*Config::getInstance()->getMaxColumn() * 17;
	int maxColumn = Config::getInstance()->getMaxRow()*Config::getInstance()->getMaxColumn() * 17;
	m_routingMatrix = new d_matrix(maxRow, maxColumn);
	m_shortRouting = new d_matrix(maxRow, maxColumn);
	p_type = type_user;
}


User::~User()
{
}

void User::moveRandomByFoot() {
	float t_offset1 = 2 * (rand() % (int)(m_footSpeed * 100)) - m_footSpeed * 100;
	float t_offset2 = 2 * (rand() % (int)(m_footSpeed * 100)) - m_footSpeed * 100;
	t_offset1 = t_offset1 / 100;
	t_offset2 = t_offset2 / 100;
	int test1 = getX();
	int test2 = getY();
	getX() = getX() + t_offset1;
	if (getX()<0 || getX() >Config::getInstance()->getMaxNetworkSize()) {
		getX() = getX() - t_offset1;
	}
	getY() = getY() - t_offset2;
	if (getY()<0 || getY() >Config::getInstance()->getMaxNetworkSize()) {
		getY() = getY() - t_offset1;
	}
}

void User::moveRandomByCar() {
	float t_offset1 = 2 * (rand() % (int)(m_carSpeed * 100)) - m_carSpeed * 100;
	float t_offset2 = 2 * (rand() % (int)(m_carSpeed * 100)) - m_carSpeed * 100;
	t_offset1 = t_offset1 / 100;
	t_offset2 = t_offset2 / 100;
	getX() = getX() + t_offset1;
	if (getX() < 0 || getX() > Config::getInstance()->getMaxNetworkSize()) {
		getX() = getX() - t_offset1;
	}
	getY() = getY() - t_offset2;
	if (getY() < 0 || getY() > Config::getInstance()->getMaxNetworkSize()) {
		getY() = getY() - t_offset1;
	}
}

void User::moveByFoot(venue* t_venue) {
	float t_offset1, t_offset2;
	if (t_venue->getX() - getX() > 0) {
		t_offset1 = rand() % (int)(m_footSpeed * 100);
	}
	else {
		t_offset1 = -rand() % (int)(m_footSpeed * 100);
	}
	if (t_venue->getY() - getY() < 0) {
		 t_offset2 = rand() % (int)(m_footSpeed * 100);
	}
	else {
		t_offset2 = -rand() % (int)(m_footSpeed * 100);
	}
	t_offset1 = t_offset1 / 100;
	t_offset2 = t_offset2 / 100;
	getX() = getX() + t_offset1;
	if (getX() < 0 || getX() > Config::getInstance()->getMaxNetworkSize()) {
		getX() = getX() - t_offset1;
	}
	getY() = getY() - t_offset2;
	if (getY() < 0 || getY() > Config::getInstance()->getMaxNetworkSize()) {
		getY() = getY() - t_offset1;
	}
}

void User::moveByCar(venue* t_venue) {
	float t_offset1, t_offset2;
	if (t_venue->getX() - getX() > 0) {
		t_offset1 = rand() % (int)(m_carSpeed * 100);
	}
	else {
		t_offset1 = -rand() % (int)(m_carSpeed * 100);
	}
	if (t_venue->getY() - getY() < 0) {
		t_offset2 = rand() % (int)(m_carSpeed * 100);
	}
	else {
		t_offset2 = -rand() % (int)(m_carSpeed * 100);
	}
	t_offset1 = t_offset1 / 100;
	t_offset2 = t_offset2 / 100;
	getX() = getX() + t_offset1;
	if (getX() < 0 || getX() > Config::getInstance()->getMaxNetworkSize()) {
		getX() = getX() - t_offset1;
	}
	getY() = getY() - t_offset2;
	if (getY() < 0 || getY() > Config::getInstance()->getMaxNetworkSize()) {
		getY() = getY() - t_offset1;
	}
}

bool User::checkIsValid(venue* t_venue){
	float t_distance = sqrt(pow(t_venue->getX() - getX(), 2) + pow(t_venue->getY() - getY(), 2) + pow(t_venue->getZ() - getZ(), 2));
	if (t_distance > Config::getInstance()->getThresholdDis()) {
		return false;
	}
	else {
		return true;
	}
}