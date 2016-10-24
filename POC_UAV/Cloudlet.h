#pragma once
#include "UAV.h"
#include "Cloud.h"
class Cloudlet :
	public Cloud
{
private:
	int m_cloudletID;

public:
	Cloudlet();
	~Cloudlet();

	int& getCloudletID() { return m_cloudletID; }
};

