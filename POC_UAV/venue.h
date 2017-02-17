#pragma once
class venue
{
private:
	int m_id;
	float m_x, m_y, m_z;

public:
	venue();
	~venue();

	float &getX() { return m_x; }
	float &getY() { return m_y; }
	float &getZ() { return m_z; }
	int  &getId() { return m_id; }
};

