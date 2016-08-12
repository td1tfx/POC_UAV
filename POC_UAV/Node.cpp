#include "Node.h"



Node::Node()
{
	m_IMatrix = new d_matrix(11, 12, -2);
	m_IMatrix_copy = new d_matrix(11, 12, -2);
	__initIMatrix();
	m_OCMatrix = new d_matrix(3, 2, -2);
	__initOCMatrix();
	m_linkNum = 0;
	m_packageCount = 0;
	m_nodeTime = 0;
	m_GWHop = 0;
	int maxRow = Config::getInstance()->getMaxRow()*Config::getInstance()->getMaxColumn();
	int maxColumn = Config::getInstance()->getMaxRow()*Config::getInstance()->getMaxColumn();
	m_routingMatrix = new d_matrix(maxRow, maxColumn);
	m_shortRouting = new d_matrix(maxRow, maxColumn);
	m_routingMatrix->initData(0);
	m_shortRouting->initData(-1);
	m_inData = nullptr;
	__init();
}


Node::~Node()
{
	delete m_IMatrix;
	delete m_IMatrix_copy;
	delete m_OCMatrix;
	delete m_routingMatrix;
	delete m_shortRouting;
}

void Node::__updateIMatrix(Edge t_edge) {
	int ch = edges_weight_channel[t_edge];
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

void Node::__init() {
	if (m_id/Config::getInstance()->getMaxRow() == 0 || m_id / Config::getInstance()->getMaxRow() == Config::getInstance()->getMaxRow() - 1
		|| m_id % Config::getInstance()->getMaxRow() == 0 || m_id % Config::getInstance()->getMaxRow() == Config::getInstance()->getMaxColumn() - 1) {
		m_isOuterNode = true;
		//paGenerateRate = rand() % (int)Config::getInstance()->getMaxGenerateRate() + 1;
	}
	else {
		m_isOuterNode = false;
		//paGenerateRate = 0;
	}
	for (int i = 0; i < Config::getInstance()->getMaxGenerateRate(); i++) {
		//generatePackage();
	}

}

void Node::__updateIMatrixNormal(Edge t_edge) {
	int ch = edges_weight_channel[t_edge];
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
			std::cout << m_IMatrix->getData(i, j) << " ";
		}
		std::cout << endl;
	}
}

void Node::printOCMatrix() {
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 2; j++) {
			std::cout << m_OCMatrix->getData(i, j) << " ";
		}
		std::cout << endl;
	}
}

float Node::getDistance(Node m){

	float distance = sqrt(pow(m.getX() - m_x, 2) + pow(m.getY() - m_y, 2) + pow(m.getZ() - m_z, 2));
	return distance;
}

void Node::__updateIF() {
	for (int ch = 0; ch < 11; ch++) {
		for (int i = 1; i < 12; i++) {
			m_IMatrix->getData(ch, i) += __getIF(m_IMatrix->getData(i - 1, 0), ch, i - 1);
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
		if (edges_weight_channel[*i] < 0) {
			int ch = __chooseRadio(*m_nodes.at(targetNode));
			if (ch == -1) {
				continue;
			}
			edges_weight_channel[*i] = ch;
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
		if (edges_weight_channel[*i] < 0) {
			int ch = __chooseRadioNormal(*m_nodes.at(targetNode));
			if (ch > 2) {
				cout << "something wrong is happen!" << endl;
			}
			if (ch == -1) {
				continue;
			}
			edges_weight_channel[*i] = ch;
			__updateIMatrixNormal(*i);
		}
	}
}

void Node::generatePaPerRound() {
	double pNumPer = (double)(Config::getInstance()->getBandwidth() / Config::getInstance()->getPackageSize());
	double gRatePerRound = Config::getInstance()->getMaxGenerateRate() / pNumPer;
	int threshold = rand() % 2000 * gRatePerRound;
	int ge_random = rand() % 1000;
	while (ge_random < threshold) {
		generatePackage();
		ge_random = ge_random + 1000;
	}
}



void Node::generatePackage() {
	int pid = m_id * Config::getInstance()->gerMaxPacNumPerNode() + m_packageCount + 1;
	Package *m_package = new Package(pid, m_nodeTime);
	int dest = m_GWNum;
	m_package->setSource(m_id);
	m_package->setDestination(dest);
	m_package->setGenerateTime(m_nodeTime);
	int size = Config::getInstance()->getMaxColumn()*Config::getInstance()->getMaxRow();
	m_package->setPathSize(size);
	for (int i = 0; i < size; i++) {
		m_package->getPathData(i) = m_shortRouting->getData(dest, i);
	}
	m_qServe.push(m_package);
	m_packageCount++;
}

int Node::bestResponse() {
	
	d_matrix::cpyData(m_IMatrix_copy, m_IMatrix);
	for (int i = 0; i < 11; i++) {
		
	}
}

float Node::__calculateUtility() {

	if (m_GWHop <= 0) {
		return 0;
	}
	int min = INFINITY;
	float rate = Config::getInstance()->getBandwidth() / 1024 * m_edges.size();
	float interN = 0;
	int j = 0;
	vector<Edge>::iterator ei;
	float up = 0;
	for (ei = m_edges.begin(); ei != m_edges.end(); ei++) {
		float up_t = 0;
		if (edges_weight_channel[*ei] >= 0) {
			int targetNode = target(*ei, *m_conGraph);
			j = edges_weight_channel[*ei];
			for (int i = 1; i < 12; i++) {
				int p1 = m_IMatrix->getData(j, i);
				int p2 = m_nodes.at(targetNode)->getIMatrix()->getData(j, i);
				interN = interN + p1 + p2;
			}
			up_t = rate / interN;
			up = up + up_t;
		}
	}
	float utility = up / m_GWHop;
	return utility;
}

