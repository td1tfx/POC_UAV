#include "Node.h"




Node::Node()
{
	m_IMatrix = new d_matrix(11, 12, -2);
	m_IMatrix_copy = new d_matrix(11, 12, -2);
	m_OCMatrix = new d_matrix(3, 2, -2);
	m_linkNum = 0;
	m_packageCount = 0;
	m_nodeTime = 0;
	m_GWHop = 0;
	int maxRow = Config::getInstance()->getMaxRow()*Config::getInstance()->getMaxColumn();
	int maxColumn = Config::getInstance()->getMaxRow()*Config::getInstance()->getMaxColumn();
	float t_pSize = Config::getInstance()->getPackageSize();
	float t_bWidth = Config::getInstance()->getBandwidth();
	m_perTransDelay = t_pSize / t_bWidth;
	m_perTransSignalDelay = Config::getInstance()->getSignalSize() / t_bWidth;
	m_routingMatrix = new d_matrix(maxRow, maxColumn);
	m_shortRouting = new d_matrix(maxRow, maxColumn);
	m_routingMatrix->initData(0);
	m_shortRouting->initData(-1);
	m_inData = nullptr;
	p_type = type_node;
	int nnum = 3;
	m_netQ = new NeuralNet[nnum];
	__init();
}


Node::~Node()
{
// 	delete m_IMatrix;
// 	delete m_IMatrix_copy;
// 	delete m_OCMatrix;
// 	delete m_routingMatrix;
// 	delete m_shortRouting;

}


void Node::__init() {
	if (m_id / Config::getInstance()->getMaxRow() == 0 || m_id / Config::getInstance()->getMaxRow() == Config::getInstance()->getMaxRow() - 1
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

void Node::__updateIMatrix(Edge t_edge) {
	int ch = edges_weight_channel[t_edge];
	vector<Node*>::iterator i;
	for (i = m_nodes.begin(); i != m_nodes.end(); i++) {
		m_sigCount++;
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

void Node::__updateIMatrixCopy(Edge t_edge, int ch) {
	vector<Node*>::iterator i;
	for (i = m_nodes.begin(); i != m_nodes.end(); i++) {
		float t_dist = this->getDistance(**i);
		if (t_dist < (*i)->getIMatrixCopy()->getData(ch, 0)) {
			(*i)->getIMatrixCopy()->getData(ch, 0) = t_dist;
			(*i)->__updateIFCopy();
		}
		t_dist = m_nodes.at(target(t_edge, *m_conGraph))->getDistance(**i);
		if (t_dist < (*i)->getIMatrixCopy()->getData(ch, 0)) {
			(*i)->getIMatrixCopy()->getData(ch, 0) = t_dist;
			(*i)->__updateIFCopy();
		}
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

void Node::__updateIFCopy() {
	for (int ch = 0; ch < 11; ch++) {
		for (int i = 1; i < 12; i++) {
			m_IMatrix_copy->getData(ch, i) += __getIF(m_IMatrix_copy->getData(i - 1, 0), ch, i - 1);
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

void Node::channelAssignmentCopy(int ch) {

	//return NULL;
}

void Node::initialPackage() {
	if (m_id / Config::getInstance()->getMaxColumn() == 0 || m_id / Config::getInstance()->getMaxColumn() == Config::getInstance()->getMaxRow() - 1
		|| m_id % Config::getInstance()->getMaxColumn() == 0 || m_id % Config::getInstance()->getMaxColumn() == Config::getInstance()->getMaxColumn() - 1) {
		m_isOuterNode = true;
		//paGenerateRate = rand() % (int)Config::getInstance()->getMaxGenerateRate() + 1;
	}
	else {
		m_isOuterNode = false;
		//paGenerateRate = 0;
	}
// 	if (m_id==4) {
// 		generatePackage();
// 		generatePackage();
// 		generatePackage();
// 		generatePackage();
// 	}else if(m_id == 3) {
// 		generatePackage();
// 		generatePackage();
// 		generatePackage();
// 	}else if (m_id == 2) {
// 		generatePackage();
// 		generatePackage();
// 	}else{
// 		generatePackage();
// 	}
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
// 	while (dest == m_id) {
// 		dest = m_outerNodes.at(rand() % m_outerNodes.size())->getId();
// 	}
	m_package->setSource(m_id);
	m_package->setDestination(dest);
	m_package->setGenerateTime(m_nodeTime);
// 	int size = Config::getInstance()->getMaxColumn()*Config::getInstance()->getMaxRow();
// 	m_package->setPathSize(size);
// 	for (int i = 0; i < size; i++) {
// 		m_package->getPathData(i) = m_trainRouting->getData(dest, i);
// 	}
	m_qServe.push(m_package);
	m_packageCount++;
}

void Node::generatePackage(int dest, float nodeTime) {	
	int pid = m_id * Config::getInstance()->gerMaxPacNumPerNode() + m_packageCount + 1;
	Package *m_package = new Package(pid, m_nodeTime);
	m_package->setSource(m_id);
	m_package->setDestination(dest);
	if (nodeTime == -1) {
		m_package->setGenerateTime(m_nodeTime);
	}
	else {
		m_package->setGenerateTime(nodeTime);
	}
	m_qServe.push(m_package);
	m_packageCount++;
}

float Node::bestResponseInCoCAG() {
	m_utility.clear();
	float maxUtility = 0;
	int ch = -1;
	vector<Edge>::iterator eii;
	if (m_edges.empty()) {
		std::cout << "wrong!!!m_edges is empty!" << endl;
		return -1;
	}
	for (eii = m_edges.begin(); eii != m_edges.end(); eii++) {
		int s = source(*eii, *m_conGraph);
		//int targetNode = target(*eii, *m_conGraph);
		//int p_id = m_id;
		float maxUtility_edge = 0;
		for (int i = 0; i < 11; i++) {
			float oneUtility = 0;
			vector<Node*>::iterator nodei;
			for (nodei = m_nodes.begin(); nodei != m_nodes.end(); nodei++) {
				(*nodei)->copyIMatrix();
			}
			int t_ch = edges_weight_channel[*eii];
			edges_weight_channel[*eii] = i;
			//__updateIMatrixCopy(*eii, i);
			vector<Node*>::iterator j;
			for (j = m_nodes.begin(); j != m_nodes.end(); j++) {
				oneUtility += (*j)->calculateUtilityInCoCAG();
			}
			edges_weight_channel[*eii] = t_ch;
			if (oneUtility > maxUtility_edge) {
				maxUtility_edge = oneUtility;
				ch = i;
			}
		}
		edges_weight_channel[*eii] = ch;
		__updateIMatrix(*eii);
		m_utility.push_back(maxUtility_edge);
	}
	for (int i = 0; i < m_edges.size(); i++) {
		if (m_utility.at(i) > maxUtility) {
			maxUtility = m_utility.at(i);
		}
	}
	return maxUtility;
}

float Node::bestResponseInAoCAG() {
	m_utility.clear();
	float maxUtility = 0;
	int ch = -1;
	vector<Edge>::iterator eii;
	if (m_edges.empty()) {
		std::cout << "wrong!!!m_edges is empty!" << endl;
		return -1;
	}
	for (eii = m_edges.begin(); eii != m_edges.end(); eii++) {
		int s = source(*eii, *m_conGraph);
		//int targetNode = target(*eii, *m_conGraph);
		//int p_id = m_id;
		float maxUtility_edge = 0;
		for (int i = 0; i < 11; i++) {
			float oneUtility = 0;
// 			vector<Node*>::iterator nodei;
// 			for (nodei = m_nodes.begin(); nodei != m_nodes.end(); nodei++) {
// 				(*nodei)->copyIMatrix();
// 			}
			int t_ch = edges_weight_channel[*eii];
			edges_weight_channel[*eii] = i;
			//__updateIMatrixCopy(*eii, i);
			oneUtility = calculateUtilityInAoCAG();
			edges_weight_channel[*eii] = t_ch;
			if (oneUtility > maxUtility_edge) {
				maxUtility_edge = oneUtility;
				ch = i;
			}
		}
		edges_weight_channel[*eii] = ch;
		__updateIMatrix(*eii);
		m_utility.push_back(maxUtility_edge);
	}
	for (int i = 0; i < m_edges.size(); i++) {
		maxUtility += m_utility.at(i);
	}
	return maxUtility;
}

float Node::calculateUtilityInCoCAG() {

	if (m_GWHop <= 0) {
		return 0;
	}
	int min = INFINITY;
	float rate = Config::getInstance()->getBandwidth() / 1024 * m_edges.size();
	float interN = 0;
	int interNum = 0;
	int j = 0;
	vector<Edge>::iterator ei;
	float up = 0;
	for (ei = m_edges.begin(); ei != m_edges.end(); ei++) {
		float up_t = 0;
		if (edges_weight_channel[*ei] >= 0) {
			int targetNode = target(*ei, *m_conGraph);
			j = edges_weight_channel[*ei];
			for (int i = 1; i < 12; i++) {
				int p1 = m_IMatrix_copy->getData(j, i);
				int p2 = m_nodes.at(targetNode)->getIMatrixCopy()->getData(j, i);
				interN = interN + p1 + p2;
				if (p1 > 0 || p2 > 0) {
					interNum++;
				}
			}
			up_t = rate / (interN + 1);
			up = up + up_t;
		}
	}
	float utility = up / (m_GWHop + 1);
	return utility;
}

float Node::calculateUtilityInAoCAG() {

	if (m_GWHop <= 0) {
		return 0;
	}
	int min = INFINITY;
	float rate = Config::getInstance()->getBandwidth() / 1024 * m_edges.size();
	float interN = 0;
	int interNum = 0;
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
				//int p2 = m_nodes.at(targetNode)->getIMatrixCopy()->getData(j, i);
				interN = interN + p1;
			}
			up_t = rate / (interN + 1);
			up = up + up_t;
		}
	}
/*	float utility = up / (log(m_GWHop + 1));*/
	float utility = up;
	return utility;
}

void Node::__CHMatrixTransfer() {

	vector<Edge>::iterator ei;
	int numE = 0;
	int test1 = m_edges.size();
	for (ei = m_edges.begin(); ei != m_edges.end(); ei++) {
		int CH = edges_weight_channel[*ei];
		if (CH >= 0) {
			int* CHNum = new int[11];
			for (int i = 0; i < 11; i++) {
				CHNum[i] = 0;
			}
			CHNum[CH] = 1;
			m_CHMatrix.push_back(CHNum);
			numE++;
		}
		else {
			int* CHNum = new int[11];
			for (int i = 0; i < 11; i++) {
				CHNum[i] = 0;
			}
			m_CHMatrix.push_back(CHNum);
			numE++;
		}
	}
}

void Node::CHMatrixTransferBack() {

	vector<int*>::iterator ch;
	int numE = 0;
	for (ch = m_CHMatrix.begin(); ch != m_CHMatrix.end(); ch++) {
		for (int i = 0; i < 11; i++) {
			if ((*ch)[i] > 0) {
				edges_weight_channel[m_edges.at(numE)] = i;
			}
		}
		numE++;
	}
}


void Node::saveNodeData(int inDataSize, bool clean)
{
	__CHMatrixTransfer();
	char filename[30];
	char dir[20];
	int t_outputCount = 11;
	int t_inputCount = inDataSize;
	sprintf(dir, "%s%d", "node" , m_id);
	if (_access(dir, 0) == -1) {
		_mkdir(dir);
	}
	vector<int*>::iterator CHi;
	int linknum = 0;
	for (CHi = m_CHMatrix.begin(); CHi != m_CHMatrix.end(); CHi++) {
		sprintf(filename, "%s%d%s%d%s", "node", m_id, "/link", linknum, ".txt");
		FILE *fout = stdout;
		if (filename) {
			if (clean) {
				fout = fopen(filename, "w+t");
				fprintf(fout, "input:");
				fprintf(fout, "%d", t_inputCount);
				fprintf(fout, "\t");
				fprintf(fout, "output:");
				fprintf(fout, "%d", t_outputCount);
				fprintf(fout, "\t");
				fprintf(fout, "testCount:");
				fprintf(fout, "%d", 0);
				fprintf(fout, "---------------------------------------\n");
			}
			else {
				fout = fopen(filename, "a+t");
			}

			for (int i = 0; i < t_inputCount; i++)
			{
				fprintf(fout, "%1.2f", m_inData[i]);
				fprintf(fout, "\t");
			}
			fprintf(fout, "toOut:");

			for (int i = 0; i < t_outputCount; i++)
			{
				float outCH = (*CHi)[i];
				fprintf(fout, "%1.2f", outCH);
				fprintf(fout, "\t");
			}
			fprintf(fout, "\n");

			if (filename)
				fclose(fout);
		linknum++;
		}
	}
}

Package* Node::outPackage() {
	Package* out_package = m_qServe.front();
	m_qServe.pop();
	return out_package;
}

void Node::inPackage(Package* in_package, int recType) {//type: 0, cloudlet, 1, cloud 
	in_package->getHop()++;
	if (in_package->getDestination() == m_id) {
		if (in_package->getGenerateTime() == m_nodeTime) {	//this is to avoid the same round
			in_package->setTerminalTime(m_nodeTime + m_perTransDelay);
		}
		if (in_package->isSignaling()) {
			if (in_package->getHop() > 3) {
			}
			else {
				vector<Node*>::iterator iter;
				for (iter = m_neigherNodes.begin(); iter != m_neigherNodes.end(); iter++) {
					int dest = (*iter)->getId();
// 					if (istrained) {
// 						if (!m_nodes->at(dest)->isOuterNode())
// 							continue;
// 					}
// 					Package *sPac = new Package;
// 					*sPac = *in_package;
// 					sPac->setDestination(dest);
// 					qServe->push(sPac);
 				}
// 				//need more
 			}
// 			qSFinished->push(in_package);
		}
		else {
			if (this->p_type == type_UAV) {
				if (recType == 1) {
					in_package->setDestination(m_GWNum);
					m_qServe.push(in_package);
				}
				else {
					generatePackage(in_package->getSource(), in_package->getGenerateTime());
					in_package->setTerminalTime(m_nodeTime);
					m_qFinished.push(in_package);
				}
			}
			else if (this->p_type == type_cloud) {
				generatePackage(in_package->getSource(),in_package->getGenerateTime());
				in_package->setTerminalTime(m_nodeTime);
				m_qFinished.push(in_package);
			}
			else {
				in_package->setTerminalTime(m_nodeTime);
				m_qFinished.push(in_package);
			}
		}
	}
	else {
		if (this->p_type == type_UAV) {
			m_qServe.push(in_package);
		}
		else if (this->p_type == type_cloud) {
			m_qServe.push(in_package);
		}
		else {
			m_qServe.push(in_package);
		}
	}
}

void Node::initCHMatrix() {
	m_CHMatrix.clear();
}

void Node::initMatrix() {
	__initIMatrix();
	__initOCMatrix();
}

void Node::calculateDelay(bool isTrained)
{
	char dir[20];
	char delayFilename[50];
	m_pacNum = 0;
	allDelay = 0;
	allOnehopDelay = 0;
	if (isTrained) {
		sprintf(delayFilename, "%s%d%s", "../TrainedDelay/delayOfDestination", m_id, ".txt");
		sprintf(dir, "%s", "../TrainedDelay");
		if (_access(dir, 0) == -1) {
			_mkdir(dir);
		}
	}
	else {
		sprintf(delayFilename, "%s%d%s", "../OSPFDelay/delayOfDestination", m_id, ".txt");
		sprintf(dir, "%s", "../OSPFDelay");
		if (_access(dir, 0) == -1) {
			_mkdir(dir);
		}
	}
	FILE *fout = stdout;
	if (delayFilename)
		fout = fopen(delayFilename, "w+t");
	fprintf(fout, "---------------------------------------\n");

	//routingMatrix->memcpyDataOut(outData, t_outputCount * sizeof(outData));
	fprintf(fout, "id");
	fprintf(fout, "\t");
	fprintf(fout, "initNode");
	fprintf(fout, "\t");
	fprintf(fout, "hop");
	fprintf(fout, "\t");
	fprintf(fout, "generateTime");
	fprintf(fout, "\t");
	fprintf(fout, "totalDeley");
	fprintf(fout, "\t");
	fprintf(fout, "oneHopDeley");
	fprintf(fout, "\n");

	Package* pac;
	while (!m_qFinished.empty())
	{
		pac = m_qFinished.front();
		m_pacNum++;
		allDelay = allDelay + pac->getDelay();
		float t_oneDelay = pac->getDelay() / pac->getHop();
		allOnehopDelay = allOnehopDelay + t_oneDelay;
		int t_nodeNum = floor(pac->getId() / Config::getInstance()->gerMaxPacNumPerNode());
		fprintf(fout, "%d", pac->getId());
		fprintf(fout, "\t");
		fprintf(fout, "%d", t_nodeNum);
		fprintf(fout, "\t");
		fprintf(fout, "%d", pac->getHop());
		fprintf(fout, "\t");
		fprintf(fout, "%d", pac->getGenerateTime());
		fprintf(fout, "\t");
		fprintf(fout, "%1.2f", pac->getDelay());
		fprintf(fout, "\t");
		fprintf(fout, "%1.2f", t_oneDelay);
		fprintf(fout, "\n");
		m_qFinished.pop();
	}
	pac = nullptr;
	float averageDelay = allDelay / m_pacNum;
	float averageOnehopDelay = allOnehopDelay / m_pacNum;
	fprintf(fout, "total package number:");
	fprintf(fout, "%d", m_pacNum);
	fprintf(fout, "\n");
	fprintf(fout, "average delay:");
	fprintf(fout, "%1.2f", averageDelay);
	fprintf(fout, "\n");
	fprintf(fout, "average one-hop delay:");
	fprintf(fout, "%1.2f", averageOnehopDelay);
	fprintf(fout, "\n");
	if (delayFilename)
		fclose(fout);
}


void Node::moveRandom() {
	int t_offset1 = 2 * rand() % m_speed - m_speed;
	int t_offset2 = 2 * rand() % m_speed - m_speed;
	m_x = m_x + t_offset1;
	if (m_x<0 || m_x >Config::getInstance()->getMaxNetworkSize()) {
		m_x = m_x - t_offset1;
	}
	m_y = m_y - t_offset2;
	if (m_y<0 || m_y >Config::getInstance()->getMaxNetworkSize()) {
		m_y = m_y - t_offset1;
	}
}

void Node::initNerualNet() {
// 	int nnum = m_edges.size();
// 	m_netQ = new NeuralNet[nnum];
}
