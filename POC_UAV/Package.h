#pragma once
#include "Config.h"

class Package
{
private:
	int m_id;
	int m_size;
	float m_delay;
	float m_generateTime;
	float m_terminalTime;
	int m_source;
	int m_destination;
	int m_hop;
	double* m_path;
	bool m_signaling;

public:
	Package();
	Package(int id, float time);
	~Package();
	void setDestination(int dest) {
		m_destination = dest;
	}
	int getId() {
		return m_id;
	}

	int getSource() {
		return m_source;
	}

	void setSource(int id) {
		m_source = id;
	}

	int getDestination() {
		return m_destination;
	}
	int& getHop() {
		return m_hop;
	}
	int getGenerateTime() {
		return m_generateTime;
	}

	void setGenerateTime(int time) {
		m_generateTime = time;
	}

	float getDelay() {
		m_delay = m_terminalTime - m_generateTime;
		return m_delay;
	}

	int getNextNode() {
		return m_path[m_hop];
	}

	void setPathSize(int size) {		
		m_path = new double[size];
	}

	double& getPathData(int num) {
		return m_path[num];
	}

	double* getPath() {
		return m_path;
	}

	void setTerminalTime(float time) {
		m_terminalTime = time;
	}

	void setSignaling() {
		m_signaling = true;
		m_size = Config::getInstance()->getSignalSize();
	}
	bool isSignaling() {
		return m_signaling;
	}
};

