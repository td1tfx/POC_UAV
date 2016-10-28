#include "User.h"



User::User()
{
	int maxRow = Config::getInstance()->getMaxRow()*Config::getInstance()->getMaxColumn() * 17;
	int maxColumn = Config::getInstance()->getMaxRow()*Config::getInstance()->getMaxColumn() * 17;
	m_routingMatrix = new d_matrix(maxRow, maxColumn);
	m_shortRouting = new d_matrix(maxRow, maxColumn);
}


User::~User()
{
}
