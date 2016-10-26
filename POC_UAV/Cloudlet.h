#pragma once
#include "UAV.h"
#include "Cloud.h"
class Cloudlet :
	public UAV
{
private:
	int m_cloudletID;
	bool m_cloudlet = true;

public:
	Cloudlet();
	~Cloudlet();

	int& getCloudletID() { return m_cloudletID; }
	bool isCloudlet() { return m_cloudlet; }
};

