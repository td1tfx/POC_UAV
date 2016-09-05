#include "Network.h"



Network::Network()
{
}


Network::~Network()
{
}

int Network::__initGrid() {
	//int nodeNum = Config::getInstance()->getMaxRow()*Config::getInstance()->getMaxColumn();
	//m_nodes->resize(0);
	int GW = Config::getInstance()->getMaxRow()*Config::getInstance()->getMaxColumn() - 1;
	for (int i = 0; i <Config::getInstance()->getMaxRow(); i++) {
		for (int j = 0; j < Config::getInstance()->getMaxColumn(); j++) {
			Node* t_node = new Node();
			t_node->setId(i * Config::getInstance()->getMaxColumn() + j);
			t_node->setPos(i * 10, j * 10, 0);
			t_node->getGWNum() = GW;
			//t_node->setRamdomRadios();
			m_nodes.push_back(t_node);
			if (t_node->isOuterNode()) {
				m_outerNodes.push_back(t_node);
			}
		}
	}
	m_nodes.at(GW)->getIsGW() = true;
	if (m_nodes.begin() != m_nodes.end()) {
		if (Config::getInstance()->isFullMod()) {
			m_inDataSize = m_nodes.size();
		}
		else {
			m_inDataSize = m_outerNodes.size();
		}
		m_inData = new double[m_inDataSize];
		memset(m_inData, 0, m_inDataSize * sizeof(m_inData));
		return m_nodes.size();
	}
	else return -1;
}

int Network::__initRandom() {
	char filename[30];
	char dir[20];
	std::sprintf(filename, "%s", "topology.txt");
	FILE *fout = stdout;
	if (filename){
			fout = fopen(filename, "w+");
			fprintf(fout, "nodeId:");
			fprintf(fout, "\t");
			fprintf(fout, "X:");
			fprintf(fout, "\t");
			fprintf(fout, "Y:");
			fprintf(fout, "\t");
			fprintf(fout, "Z:");
			fprintf(fout, "\n");
			for (int i = 0; i < 45; i++) {
				Node* t_node = new Node();
				t_node->setId(i);
				int t_x = rand() % 100;
				int t_y = rand() % 100;
				int t_z = 0;
				fprintf(fout, "%d", i);
				fprintf(fout, "\t");
				fprintf(fout, "%d", t_x);
				fprintf(fout, "\t");
				fprintf(fout, "%d", t_y);
				fprintf(fout, "\t");
				fprintf(fout, "%d", t_z);
				fprintf(fout, "\n");
				t_node->setPos(t_x, t_y, t_z);
				//t_node->setRamdomRadios();
				m_nodes.push_back(t_node);
			}
			fclose(fout);
			return m_nodes.size();
	}else{
		return -1;
	}
}

int Network::__initRandomFromFile() {

	char filename[30];
	char dir[20];
	std::sprintf(filename, "%s", "topology.txt");
	std::string str = readStringFromFile(filename) + "\n";
	if (str == "")
		return -1;
	std::vector<double> v;
	int n = findNumbers(str, v);
	int groupCount = n / 4;
	for (int i = 0; i < groupCount; i++) {
		Node* t_node = new Node();
		t_node->setId(v[i * 4]);
		int t_x = v[i * 4 + 1];
		int t_y = v[i * 4 + 2];
		int t_z = v[i * 4 + 3];
		t_node->setPos(t_x, t_y, t_z);
		m_nodes.push_back(t_node);
		}
	return m_nodes.size();
}


void Network::initGraph() {
	if (int text = __initGrid()) {
		//cuTime = 0;
		m_conGraph = new Graph;
		m_dGraph = new DGraph;
		__createNeighborGraph();
		__updatePribyLinkNum();
	}
}

void Network::initGraphByFile() {
	if (__initGrid()) {
		//cuTime = 0;
		m_conGraph = new Graph;
		m_dGraph = new DGraph;
		__createNeighborGraph();
		__updatePribyLinkNum();
	}
}

void Network::__updatePribyLinkNum() {
	if (!m_priNodes.empty()) {
		std::cout << "wrong!!!priNodes is not empty!" << endl;
		return;
	}
	vector<Node*>::iterator i;
	for (i = m_nodes.begin(); i != m_nodes.end(); i++) {
		m_priNodes.push(*i);
		(*i)->setMNodes(m_nodes);
		(*i)->setOuterNodes(m_outerNodes);
	}
}

void Network::__updatePribyLoad() {
	if (!m_priNodes.empty()) {
		std::cout << "wrong!!!priNodes is not empty!" << endl;
		return;
	}
	vector<Node*>::iterator i;
	for (i = m_nodes.begin(); i != m_nodes.end(); i++) {
		m_priNodes.push(*i);
		(*i)->setMNodes(m_nodes);
		(*i)->setOuterNodes(m_outerNodes);
	}
}

void Network::__updatePribyRandom() {
	while(!m_priNodesRandom.empty()) {
		m_priNodesRandom.pop();
	}
	vector<Node*>::iterator i;
	float maxLoad = 0;
	for (i = m_nodes.begin(); i != m_nodes.end(); i++) {
		(*i)->setRandom(rand());
		m_priNodesRandom.push(*i);
		if (maxLoad < (*i)->getRandom()) {
			maxLoad = (*i)->getRandom();
		}
	}
	for (i = m_nodes.begin(); i != m_nodes.end(); i++) {
		m_inData[(*i)->getId()] = (*i)->getRandom() / maxLoad;
	}
	for (i = m_nodes.begin(); i != m_nodes.end(); i++) {
		(*i)->getInData() = m_inData;
	}
}

void Network::runPOC(int num)
{
	//getAllShortestPath();
	while (!m_priNodes.empty()) {
		//(*i)->printIMatrix();
		auto t_node = (Node*)m_priNodes.top();		
		std::cout << "node:" << t_node->getId() << "->edges:" << t_node->getNeigherNodes().size() << endl;
		t_node->channelAssignment();
		t_node->printIMatrix();
		if (num == 0) {
			t_node->saveNodeData(m_inDataSize, 1);
		}
		else {
			t_node->saveNodeData(m_inDataSize, 0);
		}
		m_priNodes.pop();		
	}
	printCH();
	printPath();
	system("pause");
}

void Network::runNormal()
{
	//getAllShortestPath();
	while (!m_priNodes.empty()) {
		//(*i)->printIMatrix();
		auto t_node = (Node*)m_priNodes.top();
		std::cout << "node:" << t_node->getId() << "->edges:" << t_node->getNeigherNodes().size() << endl;
		t_node->channelAssignmentNormal();
		t_node->printOCMatrix();
		m_priNodes.pop();
	}
	printCH();
	printPath();
	system("pause");
}

float Network::runPOCGame(int num)
{
	getAllShortestPath();
	float max = 0;
	int time = 0;
	iterTimes = 0;
	__updatePribyRandom();
	//vector<Node*>::iterator i;
	while (!m_priNodesRandom.empty()) {
		auto t_node = (Node*)m_priNodesRandom.top();
		//std::cout << "node:" << (*i)->getId() << "->edges:" << (*i)->getNeigherNodes().size() << endl;
		float utility = t_node->bestResponseInAoCAG();
		max += utility;
		//t_node->printIMatrix();
		m_priNodesRandom.pop();
		if (num == 0) {
			t_node->saveNodeData(m_inDataSize, 1);
		}
		else {
			t_node->saveNodeData(m_inDataSize, 0);
		}
		iterTimes++;
	}
	time++;
	std::cout << "NetUtility:" << max << " times:" << time << endl;
	//printCH();
	//printPath();
	//system("pause");
	return max;
}

void Network::__createNeighborGraph() {
	vector<Node*>::iterator i;
	for (i = m_nodes.begin(); i != m_nodes.end(); i++) {
		vector<Node*>::iterator j;
		//int p_id = (*i)->getId()
		for (j = i + 1; j != m_nodes.end(); j++) {
			if ((*i)->getDistance(**j) <= IR[0]+10) {
// 				int rand1 = rand() % 100;
// 				if (rand1 > 40) {
// 					continue;
// 				}
				if ((*i)->getLinkNum() > 2 || (*j)->getLinkNum() > 2) {
					continue;
				}
				int m = (*i)->getId();
				int n = (*j)->getId();
				(*i)->getLinkNum()++;
				(*j)->getLinkNum()++;
				string iFirst = toString((*i)->getId());
				string jFirst = toString((*j)->getId());
				string linkEdge = iFirst + "->" + jFirst;
				string linkEdge2 = jFirst + "->" + iFirst;
				Edge ed;
				DEdge ed2,ed3;
				ed = (add_edge((*i)->getId(), (*j)->getId(), *m_conGraph)).first;
				ed2 = (add_edge((*i)->getId(), (*j)->getId(), *m_dGraph)).first;
				ed3 = (add_edge((*j)->getId(), (*i)->getId(), *m_dGraph)).first;
				edges_index[ed] = linkEdge;
				edges_weight_channel[ed] = -1;
				edges_index_d[ed2] = linkEdge;
				edges_index_d[ed3] = linkEdge2;
				edges_weight_load[ed2] = (*j)->getPackageNum() + 1;
				edges_weight_load[ed3] = (*i)->getPackageNum() + 1;
			}
		}
	}
	pair<vertex_iter, vertex_iter> vp;
	for (vp = vertices(*m_conGraph); vp.first != vp.second; ++vp.first) {
		int f = node_index[*vp.second];
		int p = node_index[*vp.first];
		Node* t_node = m_nodes.at(node_index[*vp.first]);
		Vertex pV = *vp.first;
// 		adj_iter ai, ai_end;
// 		for (tie(ai, ai_end) = adjacent_vertices(pV, *m_conGraph); ai != ai_end; ++ai) {
// 			int nextNode = node_index[*ai];
// 			t_node->getNeigherNodes().push_back(m_nodes.at(nextNode));
// 		}
		out_edge_iter out_i, out_end;
		Edge e;
		for (tie(out_i, out_end) = out_edges(pV, *m_conGraph); out_i != out_end; ++out_i)
		{
			e = *out_i;
			Vertex src = source(e, *m_conGraph), targ = target(e, *m_conGraph);
			t_node->getEdges().push_back(e);
			t_node->getNeigherNodes().push_back(m_nodes.at(node_index[targ]));
		}
		t_node->setMcongraph(m_conGraph);
	}

}

void Network::printCH() {
	edge_iter ei, ei_end;
	int edge_num = 0;
	int connecty_num = 0;
	for (tie(ei, ei_end) = edges(*m_conGraph); ei != ei_end; ++ei)
	{
		std::cout << edges_index[*ei] << "CH:" << edges_weight_channel[*ei] + 1 << "---";
		if (edges_weight_channel[*ei] >= 0) {
			connecty_num++;
		}
		edge_num++;
	}
	std::cout << endl;
	std::cout << "edge_num =" << edge_num << endl;
	std::cout << "connecty_num =" << connecty_num << endl;
}

void Network::printPath() {
	Dedge_iter ei, ei_end;
	int edge_num = 0;
	int connecty_num = 0;
	for (tie(ei, ei_end) = edges(*m_dGraph); ei != ei_end; ++ei)
	{
		std::cout << edges_index_d[*ei] << "Path:" << edges_weight_load[*ei] << "---";
		if (edges_weight_load[*ei] >= 1) {
			connecty_num++;
		}
		edge_num++;
	}
	std::cout << endl;
	//std::cout << "edge_num =" << edge_num << endl;
	//std::cout << "connecty_num =" << connecty_num << endl;
}


bool Network::__getShortestPath(int destId) {

	DVertex s = vertex(destId, *m_dGraph);
	std::vector<DVertex> parent(num_vertices(*m_dGraph));
	std::vector<int> distMap(num_vertices(*m_dGraph));
	weight_map(boost::make_iterator_property_map(distMap.begin(), get(boost::vertex_index, *m_dGraph)));
	dijkstra_shortest_paths(*m_dGraph, s, predecessor_map(boost::make_iterator_property_map(parent.begin(),
		get(boost::vertex_index, *m_dGraph))).distance_map(boost::make_iterator_property_map(distMap.begin(), get(boost::vertex_index, *m_dGraph))));

	float sum = 0;
	vector<Node*>::iterator i;
	for (i = m_nodes.begin(); i != m_nodes.end(); i++) {
		if ((*i)->getId() == destId) {
		}
		else {
			int test1 = (*i)->getId();
			if (distMap.at((*i)->getId()) == NULL) {
				cout << "error!! Node=" << (*i)->getId() << "distMap = NULL, packageNum =" << (*i)->getPackageNum() << endl;
				return false;
			}
			else {
				vector<int>* path = new vector<int>;
				int p = (*i)->getId();
				(*i)->getRoutingMatrix()->getData(destId, p) = 1;
				int mmm = (*i)->getGWHop() = distMap.at(p);
				string linkId = toString(p);
				string routingId = "";
				int num = 0;
				while (p != destId) {
					p = parent.at(p);
					(*i)->getRoutingMatrix()->getData(destId, p) = 1;
					(*i)->getShortPath()->getData(destId, num) = p;
					linkId = linkId + "->" + toString(p);
					num++;
				}
				// print the path vector
				//cout << "node=" << (*i)->getId() << ";dest=" << destId << ";dist=" << distMap[(*i)->getId()] << ";path=" << linkId << endl;
// 				print the routing vector
// 				for (int j = 0; j < m_nodes->size(); j++) {
// 				routingId = routingId + "->" + toString((*i)->getRoutingMatrix()->getData(destId, j));
// 				}
// 				cout << "node=" << (*i)->getId() << ";dest=" << destId << ";dist=" << distMap[(*i)->getId()] << ";path=" << routingId << endl;
			}
		}
	}
	return true;
}

//OSPF
void Network::getAllShortestPath() {
	__updateNeighborGraph();
	vector<Node*>::iterator i;
	for (i = m_nodes.begin(); i != m_nodes.end(); i++) {
		(*i)->getRoutingMatrix()->initData(0);
		(*i)->getShortPath()->initData(-1);
	}
	for (i = m_nodes.begin(); i != m_nodes.end(); i++) {
		if (__getShortestPath((*i)->getId())) {
			//cout << "finished destination:" << (*i)->getId() << endl;
		}
	}
	__getNodesLoad();
}

void Network::__updateNeighborGraph() {
	//std::cout << "edges(g) = ";
	Dedge_iter ei, ei_end;
	int weight2;
	for (tie(ei, ei_end) = edges(*m_dGraph); ei != ei_end; ei++) {
		weight2 = m_nodes.at(target(*ei, *m_dGraph))->getPackageNum();
		edges_weight_load[*ei] = weight2 + 1;
	}
}

void Network::__getNodesLoad() {
	//runOneSignalRound(isTrained);
	int maxPackageNum = 1;
	vector<Node*>::iterator i;
	if (Config::getInstance()->isFullMod()) {
		for (i = m_nodes.begin(); i != m_nodes.end(); i++) {
			if ((*i)->getPackageNum() + 1 > maxPackageNum) {
				maxPackageNum = (*i)->getPackageNum() + 1;
			}
		}
		for (int i = 0; i < m_inDataSize; i++) {
			double pkNum = m_nodes.at(i)->getPackageNum();
			double a;
			if (!m_nodes.at(i)->isOuterNode()) {
				a = rand() % 1000 / 1000;
			}
			else {
				a = pkNum / maxPackageNum;
			}
			//a = pkNum / maxPackageNum;
			m_inData[i] = a;
		}
		for (i = m_nodes.begin(); i != m_nodes.end(); i++) {
			(*i)->getInData() = m_inData;
		}
	}
	else {
		for (i = m_outerNodes.begin(); i != m_outerNodes.end(); i++) {
			if ((*i)->getPackageNum() + 1 > maxPackageNum) {
				maxPackageNum = (*i)->getPackageNum() + 1;
			}
		}
		for (int i = 0; i < m_inDataSize; i++) {
			double pkNum = m_outerNodes.at(i)->getPackageNum();
			double a = pkNum / maxPackageNum;
			m_inData[i] = a;
		}
		for (i = m_nodes.begin(); i != m_nodes.end(); i++) {
			(*i)->getInData() = m_inData;
		}
	}
}

void Network::initTrainNet() {
	vector<Node*>::iterator i;
	for (i = m_nodes.begin(); i != m_nodes.end(); i++) {
		for (int j = 0; j < (*i)->getEdges().size(); j++) {
			(*i)->initNerualNet();
			(*i)->getNet(j).loadOptoin("learnConfig.ini");
			(*i)->getNet(j).resetOption((*i)->getId(), 1, j);
			(*i)->getNet(j).init();
			cout << "node:" << (*i)->getId() << "-link:" << j << "NeuralNet init finished!" << endl;
		}
	}
}


int Network::trainNet() {
	vector<Node*>::iterator i;
	for (i = m_nodes.begin(); i != m_nodes.end(); i++) {
		Timer t;
		t.start();
		for (int j = 0; j < (*i)->getEdges().size(); j++) {
			(*i)->getNet(j).run();
		}
		t.stop();
		fprintf(stderr, "node %d Run neural net end. Time is %lf s.\n", (*i)->getId(), t.getElapsedTime());
	}
#ifdef _WIN32
	getchar();
#endif
	return 0;
}

//use learning algorithm
void Network::getAllTrainedPath() {

	__updatePribyRandom();
	vector<Node*>::iterator i;
	totalPCount = 0;
	wrongPCount = 0;
	int inputGroupCount = 1;
	int outputNodeCount = 11;
	auto t_output = new double[outputNodeCount*inputGroupCount];
	//auto t_Soutput = new double[sqrt(outputNodeCount)*inputGroupCount];
	for (i = m_nodes.begin(); i != m_nodes.end(); i++) {
		if (Config::getInstance()->isSingleDestMod()) {
			if (Config::getInstance()->isSingleOutputMod()) {
				for (int j = 0; j < m_nodes->size(); j++) {
					Node* t_node = *i;
					if (m_nodes->at(j)->isOuterNode()) {
						totalPCount++;
					}
					int count = 0;
					bool isbreak = false;
					int t_id = 0;
					while (t_node->getId() != j && isbreak == false) {
						count++;
						int pid = t_node->getId();
						t_node->getNet(j).resetGroupCount(inputGroupCount);
						t_node->getNet(j).activeOutputValue((*i)->getInData(), t_Soutput, inputGroupCount);
						for (int n = 0; n < m_nodes->size(); n++) {
							int r = t_Soutput[n];
							if (t_Soutput[n] > 0.5) {
								(*i)->getTrainPath()->getData(j, t_id) = n;
								t_node = m_nodes->at(n);
								t_id++;
								break;
							}
						}
						if (count > m_nodes->size()) {
							for (int q = 0; q < m_nodes->size(); q++) {
								int p = (*i)->getShortPath()->getData(j, q);
								(*i)->getTrainPath()->getData(j, q) = p;
								if (p == j) { break; }
							}
							if (m_nodes->at(j)->isOuterNode()) {
								wrongPCount++;
							}
							isbreak = true;
						}
					}
				}
			}
			else {
				for (int j = 0; j < m_nodes->size(); j++) {
					(*i)->getNet(j).resetGroupCount(inputGroupCount);
					//(*i)->getNet(j).InputNodeCount = m_nodes->size();
					//(*i)->getNet(j).OutputNodeCount = outputNodeCount;
					(*i)->getNet(j).activeOutputValue((*i)->getInData(), t_Soutput, inputGroupCount);
					for (int n = 0; n < m_nodes->size(); n++) {
						if (t_Soutput[n] > 0.5) {
							(*i)->getRoutingMatrix()->getData(j, n) = 1;
						}
						else {
							(*i)->getRoutingMatrix()->getData(j, n) = 0;
						}
					}
				}
			}
		}
		
		/*
		cout << "node:" << (*i)->getId() << ":";
		for (int m = 0; m < m_nodes->size(); m++) {
		for (int n = 0; n < m_nodes->size(); n++) {
		cout << "-" << (*i)->getRoutingMatrix()->getData(m, n);
		}
		}
		cout << endl;
		*/
	}
	delete[] t_output;
	if (Config::getInstance()->isSingleOutputMod()) {
	}
	else {
		for (i = m_outerNodes->begin(); i != m_outerNodes->end(); i++) {
			getTrainedPath((*i)->getId());
		}
	}
}


string Network::toString(int a)
{
	char jF[32];
	sprintf(jF, "%d", a);
	string jFirst = jF;
	return jFirst;
}

