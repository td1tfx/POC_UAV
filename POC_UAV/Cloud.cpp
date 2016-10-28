#include "Cloud.h"



Cloud::Cloud()
{
	int maxRow = Config::getInstance()->getMaxRow()*Config::getInstance()->getMaxColumn() * 17;
	int maxColumn = Config::getInstance()->getMaxRow()*Config::getInstance()->getMaxColumn() * 17;
	m_routingMatrix = new d_matrix(maxRow, maxColumn);
	m_shortRouting = new d_matrix(maxRow, maxColumn);
	p_type = type_cloud;
}


Cloud::~Cloud()
{
}
