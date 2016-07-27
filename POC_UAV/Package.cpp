#include "stdafx.h"
#include "Package.h"


Package::Package()
{
}

Package::Package(int id, float time)
{
	this->m_id = id;
	m_size = Config::getInstance()->getPackageSize();
	m_generateTime = 0;
	m_terminalTime = 0;
	m_delay = 0;	
	m_hop = 0;
}


Package::~Package()
{
	if (m_path) {
		delete[] m_path;
	}
}
