#include "Node.h"



Node::Node()
{
	m_IMatrix = new d_matrix(11, 12, -2);
	__initIMatrix();
	m_OCMatrix = new d_matrix(3, 2, -2);
	__initOCMatrix();
	m_linkNum = 0;
}


Node::~Node()
{
}

void Node::__updateIMatrix(Edge t_edge) {
	int ch = edges_weight[t_edge];
	vector<Node*>::iterator i;
	for (i = m_nodes.begin(); i != m_nodes.end(); i++) {
		float t_dist = this->getDistance(**i);
		if (t_dist < (*i)->getIMatrix()->getData(ch, 0)) {
			(*i)->getIMatrix()->getData(ch, 0) = t_dist;
			(*i)->__updateIF();
		}
		t_dist = m_nodes.at(target(t_edge, *m_conGraph))->getDistance(**i);
		if (t_dist < (*i)->getIMatrix()->getData(ch, 0)) {
			(*i)->getIMatrix()->getData(ch, 0) = t_dist;
			(*i)->__updateIF();
		}
	}
}

void Node::__updateIMatrixNormal(Edge t_edge) {
	int ch = edges_weight[t_edge];
	vector<Node*>::iterator i;
	for (i = m_nodes.begin(); i != m_nodes.end(); i++) {
		int test1 = (*i)->getId();
		float t_dist = this->getDistance(**i);
		if (t_dist < (*i)->getOCMatrix()->getData(ch, 0)) {
			(*i)->getOCMatrix()->getData(ch, 0) = t_dist;
			if (t_dist < IR[0]) {
				(*i)->getOCMatrix()->getData(ch, 1) = 0;
			}
		}
		float t_dist1 = m_nodes.at(target(t_edge, *m_conGraph))->getDistance(**i);
		if (t_dist1 < (*i)->getOCMatrix()->getData(ch, 0)) {
			(*i)->getOCMatrix()->getData(ch, 0) = t_dist1;
			if (t_dist1 < IR[0]) {
				(*i)->getOCMatrix()->getData(ch, 1) = 0;
			}
		}

	}
}

void Node::__initIMatrix() {

	for (int i = 0; i < 11; i++) {
		for (int j = 0; j < 12; j++) {
			if (j == 0) {
				m_IMatrix->getData(i, j) = INFINITY;
			}
			else {
				m_IMatrix->getData(i, j) = 0;
			}
		}
	}
}

void Node::__initOCMatrix() {

	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 2; j++) {
			if (j == 0) {
				m_OCMatrix->getData(i, j) = INFINITY;
			}
			else {
				m_OCMatrix->getData(i, j) = 1;
			}
		}
	}
}

void Node::printIMatrix() {
	for (int i = 0; i < 11; i++) {
		for (int j = 0; j < 12; j++) {
			cout << m_IMatrix->getData(i, j) << " ";
		}
		cout << endl;
	}
}

void Node::printOCMatrix() {
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 2; j++) {
			cout << m_OCMatrix->getData(i, j) << " ";
		}
		cout << endl;
	}
}

float Node::getDistance(Node m){

	float distance = sqrt(pow(m.getX() - m_x, 2) + pow(m.getY() - m_y, 2) + pow(m.getZ() - m_z, 2));
	return distance;
}

void Node::__updateIF() {
	for (int ch = 0; ch < 11; ch++) {
		for (int i = 1; i < 12; i++) {
			m_IMatrix->getData(ch, i) = __getIF(m_IMatrix->getData(i - 1, 0), ch, i - 1);
		}
	}
}


float Node::__getIF(float dist, int ch1, int ch2) {
	float t_dist = dist;
	int t_dRange = abs(ch2 - ch1);
	if (t_dist == 0 && t_dRange < 5) {
		return INFINITY;
	}
	else if (t_dRange >= 5) {
		return 0;
	}
	else if (t_dist > IR[t_dRange]) {
		return 0;
	}else{
		return IR[t_dRange] / t_dist;
	}
}

int Node::__chooseRadio(Node n) {
	int totalIF[11];
	int min = INFINITY;
	int minRadio = 0;
	for (int i = 0; i < 11; i++) {
		totalIF[i] = 0;
		for (int j = 1; j < 12; j++) {
			int p1 = m_IMatrix->getData(i, j);
			int p2 = n.getIMatrix()->getData(i, j);
			totalIF[i] = totalIF[i] + p1 + p2;
		}
		if(totalIF[i] < min) {
			min = totalIF[i];
			minRadio = i;
		};
	}
	if (min < ThresholdIF) {
		return minRadio;
	}else{
		return -1;
	}
}

int Node::__chooseRadioNormal(Node n) {

	for (int i = 0; i < 3; i++) {
		int a = m_OCMatrix->getData(i, 1);
		if (m_OCMatrix->getData(i, 1) == 1 && n.getOCMatrix()->getData(i, 1) == 1) {
			return i;
		}
	}
	return -1;
}

bool Node::__isCAValid(int ch) {
	return true;
}

void Node::channelAssignment() {
	vector<Edge>::iterator i;
	if(m_edges.empty()) {
		std::cout << "wrong!!!m_edges is empty!" << endl;
		return;
	}
	for (i = m_edges.begin(); i != m_edges.end(); i++) {
		int s = source(*i, *m_conGraph);
		int targetNode = target(*i, *m_conGraph);
		int p_id = m_id;
		if (edges_weight[*i] < 0) {
			int ch = __chooseRadio(*m_nodes.at(targetNode));
			if (ch == -1) {
				continue;
			}
			edges_weight[*i] = ch;
			__updateIMatrix(*i);
		}
	}	
}

void Node::channelAssignmentNormal() {
	vector<Edge>::iterator i;
	if (m_edges.empty()) {
		std::cout << "wrong!!!m_edges is empty!" << endl;
		return;
	}
	for (i = m_edges.begin(); i != m_edges.end(); i++) {
		int s = source(*i, *m_conGraph);
		int targetNode = target(*i, *m_conGraph);
		int p_id = m_id;
		if (edges_weight[*i] < 0) {
			int ch = __chooseRadioNormal(*m_nodes.at(targetNode));
			if (ch > 2) {
				cout << "something wrong is happen!" << endl;
			}
			if (ch == -1) {
				continue;
			}
			edges_weight[*i] = ch;
			__updateIMatrixNormal(*i);
		}
	}
}

