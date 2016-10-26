#include "Network.h"




Network::Network()
{
	m_cuTime = 0;
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
			t_node->initialPackage();
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

int Network::__initCloudletFrame() {
	//int nodeNum = Config::getInstance()->getMaxRow()*Config::getInstance()->getMaxColumn();
	//m_nodes->resize(0);
	//int cloud = Config::getInstance()->getMaxRow()*Config::getInstance()->getMaxColumn() - 1;
	//int cloudlet = rand() % (Config::getInstance()->getMaxRow()*Config::getInstance()->getMaxColumn() - 1);
	m_cloud = new Cloud();
	m_cloud->setId(0);
	m_cloud->setPos(0, 0, 0);
	m_cloud->getIsGW() = true;
	m_cloud->initialPackage();
	m_nodes.push_back(m_cloud);
	int t_cloudletID = 0;
	int t_UAVID = 0;
	for (int i = 0; i < Config::getInstance()->getMaxRow(); i++) {
		for (int j = 0; j < Config::getInstance()->getMaxColumn(); j++) {
			if (i == j && j == 0) {
				continue;
			}
			int test = i * Config::getInstance()->getMaxColumn() + j;
			if (rand() % 100 > 30) {
				t_UAVID++;
				UAV* t_UAV = new UAV();
				t_UAV->setId(i * Config::getInstance()->getMaxColumn() + j);
				t_UAV->getUAVID() = t_UAVID;
				t_UAV->setPos(i * 50, j * 50, 0);
				t_UAV->getGWNum() = 0;
				t_UAV->initialPackage();
				m_nodes.push_back(t_UAV);
				m_UAVs.push_back(t_UAV);
			}
			else {
				t_cloudletID++;
				Cloudlet* t_Cl = new Cloudlet();
				t_Cl->setId(i * Config::getInstance()->getMaxColumn() + j);
				t_Cl->getCloudletID() = t_cloudletID;
				t_Cl->setPos(i * 50, j * 50, 0);
				t_Cl->getGWNum() = 0;
				t_Cl->initialPackage();
				m_nodes.push_back(t_Cl);
				m_UAVs.push_back(t_Cl);
				m_cloudlets.push_back(t_Cl);
			}
		}
	}
	int t_offset = m_nodes.size();
	int t_userID = 0;
	for (int i = 0; i < Config::getInstance()->getMaxRow()*5; i++) {
		for (int j = 0; j < Config::getInstance()->getMaxColumn()*5; j++) {
			if (i == j && j == 0) {
				continue;
			}
			t_userID++;
			User* t_user = new User();
			t_user->setId(t_offset + i * Config::getInstance()->getMaxColumn()*5 + j);
			t_user->getUserID() = t_userID;
			t_user->setPos(i * 10, j * 10, 0);
			t_user->getGWNum() = 0;
			t_user->initialPackage();
			m_nodes.push_back(t_user);
			m_users.push_back(t_user);
		}
	}

	return 1;
}

int Network::__initCloudFrame() {
	//int nodeNum = Config::getInstance()->getMaxRow()*Config::getInstance()->getMaxColumn();
	//m_nodes->resize(0);
	//int cloud = Config::getInstance()->getMaxRow()*Config::getInstance()->getMaxColumn() - 1;
	//int cloudlet = rand() % (Config::getInstance()->getMaxRow()*Config::getInstance()->getMaxColumn() - 1);
	m_cloud = new Cloud();
	m_cloud->setId(0);
	m_cloud->setPos(0, 0, 0);
	m_cloud->getIsGW() = true;
	m_nodes.push_back(m_cloud);
	int t_UAVID = 0;
	for (int i = 1; i < Config::getInstance()->getMaxRow(); i++) {
		for (int j = 0; j < Config::getInstance()->getMaxColumn(); j++) {
			t_UAVID++;
			UAV* t_UAV = new UAV();
			t_UAV->setId(i * Config::getInstance()->getMaxColumn() + j);
			t_UAV->getUAVID() = t_UAVID;
			t_UAV->setPos(i * 50, j * 50, 0);
			t_UAV->getGWNum() = 0;
			t_UAV->initialPackage();
			m_nodes.push_back(t_UAV);
			m_UAVs.push_back(t_UAV);
		}
	}
	int t_userID = 0;
	for (int i = 1; i < Config::getInstance()->getMaxRow() * 5; i++) {
		for (int j = 0; j < Config::getInstance()->getMaxColumn() * 5; j++) {
			t_userID++;
			User* t_user = new User();
			t_user->setId(i * Config::getInstance()->getMaxColumn() * 5 + j);
			t_user->getUserID() = t_userID;
			t_user->setPos(i * 10, j * 10, 0);
			t_user->getGWNum() = 0;
			t_user->initialPackage();
			m_nodes.push_back(t_user);
			m_users.push_back(t_user);
		}
	}

	return 1;
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
		//__updatePribyLinkNum();
	}
}

void Network::initCloudletGraph() {
	if (int text = __initCloudletFrame()) {
		//cuTime = 0;
		m_conGraph = new Graph;
		m_dGraph = new DGraph;
		__createCloudletGraph();
		//__updatePribyLinkNum();
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

//aborted
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

void Network::updatePribyLoad() {
	if (!m_priNodes.empty()) {
		std::cout << "wrong!!!priNodes is not empty!" << endl;
		return;
	}
	float maxLoad = 0;
	vector<Node*>::iterator i;
	for (i = m_nodes.begin(); i != m_nodes.end(); i++) {
		m_priNodes.push(*i);
		(*i)->setMNodes(m_nodes);
		(*i)->setOuterNodes(m_outerNodes);
		if (maxLoad < (*i)->getPackageNum()) {
			maxLoad = (*i)->getPackageNum();
		}
	}

	for (i = m_nodes.begin(); i != m_nodes.end(); i++) {
		m_inData[(*i)->getId()] = (*i)->getPackageNum() / maxLoad;
	}
	for (i = m_nodes.begin(); i != m_nodes.end(); i++) {
		(*i)->getInData() = m_inData;
		(*i)->initMatrix();
		(*i)->initCHMatrix();
	}
}

void Network::updatePribyRandom() {
	while(!m_priNodesRandom.empty()) {
		m_priNodesRandom.pop();
	}
	vector<Node*>::iterator i;
	float maxLoad = 0;
	for (i = m_nodes.begin(); i != m_nodes.end(); i++) {
		(*i)->setRandom(rand());
		m_priNodesRandom.push(*i);
		(*i)->setMNodes(m_nodes);
		(*i)->setOuterNodes(m_outerNodes);
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

float Network::runPOCGame(int num, bool isSave)
{
	float max = 0;
	int time = 0;
	iterTimes = 0;
	//vector<Node*>::iterator i;
	while (!m_priNodes.empty()) {
		auto t_node = (Node*)m_priNodes.top();
		//std::cout << "node:" << (*i)->getId() << "->edges:" << (*i)->getNeigherNodes().size() << endl;
		float utility = t_node->bestResponseInAoCAG();
		max += utility;
		//t_node->printIMatrix();
		m_priNodes.pop();
		if (isSave) {
			if (num == 0) {
				t_node->saveNodeData(m_inDataSize, 1);
			}
			else {
				t_node->saveNodeData(m_inDataSize, 0);
			}
		}
		iterTimes++;
	}
	time++;
	std::cout << "NetUtility:" << max << " times:" << time << endl;
	printCH();
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
			if ((*i)->getDistance(**j) <= IR[0] ) {
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

void Network::__createCloudletGraph() {
	//connections between users and UAVs
	vector<User*>::iterator i;
	for (i = m_users.begin(); i != m_users.end(); i++) {
		vector<UAV*>::iterator j;
		for (j = m_UAVs.begin(); j != m_UAVs.end(); j++) {
			if ((*i)->getDistance(**j) <= (*i)->getTransRange() && (*i)->getDistance(**j) <= (*j)->getTransRange()) {
				int m = (*i)->getId();
				int n = (*j)->getId();
				(*i)->getLinkNum()++;
				(*j)->getLinkNum()++;
				string iFirst = toString((*i)->getUserID());
				string jFirst = toString((*j)->getUAVID());
				string linkEdge = "User" + iFirst + "->" + "UAV" + jFirst;
				string linkEdge2 = "UAV" + jFirst + "->" + "User" + iFirst;
				Edge ed;
				DEdge ed2, ed3;
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
	//connections between UAVs
	vector<UAV*>::iterator i_uav;
	for (i_uav = m_UAVs.begin(); i_uav != m_UAVs.end(); i_uav++) {
		vector<UAV*>::iterator j_uav;
		for (j_uav = m_UAVs.begin(); j_uav != m_UAVs.end(); j_uav++) {
			if ((*i_uav)->getDistance(**j_uav) <= (*i_uav)->getTransRange() && (*i_uav)->getDistance(**j_uav) <= (*j_uav)->getTransRange()) {
				int m = (*i_uav)->getId();
				int n = (*j_uav)->getId();
				(*i_uav)->getLinkNum()++;
				(*j_uav)->getLinkNum()++;
				string iFirst = toString((*i_uav)->getUAVID());
				string jFirst = toString((*j_uav)->getUAVID());
				string linkEdge = "UAV" + iFirst + "->" + "UAV" + jFirst;
				string linkEdge2 = "UAV" + jFirst + "->" + "UAV" + iFirst;
				Edge ed;
				DEdge ed2, ed3;
				ed = (add_edge((*i_uav)->getId(), (*j_uav)->getId(), *m_conGraph)).first;
				ed2 = (add_edge((*i_uav)->getId(), (*j_uav)->getId(), *m_dGraph)).first;
				ed3 = (add_edge((*j_uav)->getId(), (*i_uav)->getId(), *m_dGraph)).first;
				edges_index[ed] = linkEdge;
				edges_weight_channel[ed] = -1;
				edges_index_d[ed2] = linkEdge;
				edges_index_d[ed3] = linkEdge2;
				edges_weight_load[ed2] = (*j_uav)->getPackageNum() + 1;
				edges_weight_load[ed3] = (*i_uav)->getPackageNum() + 1;
			}
		}
	}
	//connections between UAV and cloud
	m_cloud;
	vector<UAV*>::iterator j_uav;
	for (j_uav = m_UAVs.begin(); j_uav != m_UAVs.end(); j_uav++) {
		if ((m_cloud)->getDistance(**j_uav) <= (m_cloud)->getTransRange() && (m_cloud)->getDistance(**j_uav) <= (*j_uav)->getTransRange()) {
			int m = m_cloud->getId();
			int n = (*j_uav)->getId();
			m_cloud->getLinkNum()++;
			(*j_uav)->getLinkNum()++;
			string iFirst = toString(m_cloud->getId());
			string jFirst = toString((*j_uav)->getUAVID());
			string linkEdge = "cloud" + iFirst + "->" + "UAV" + jFirst;
			string linkEdge2 = "UAV" + jFirst + "->" + "cloud" + iFirst;
			Edge ed;
			DEdge ed2, ed3;
			ed = (add_edge(m_cloud->getId(), (*j_uav)->getId(), *m_conGraph)).first;
			ed2 = (add_edge(m_cloud->getId(), (*j_uav)->getId(), *m_dGraph)).first;
			ed3 = (add_edge((*j_uav)->getId(), m_cloud->getId(), *m_dGraph)).first;
			edges_index[ed] = linkEdge;
			edges_weight_channel[ed] = -1;
			edges_index_d[ed2] = linkEdge;
			edges_index_d[ed3] = linkEdge2;
			edges_weight_load[ed2] = (*j_uav)->getPackageNum() + 1;
			edges_weight_load[ed3] = m_cloud->getPackageNum() + 1;
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
// 			int text = node_index[targ];
// 			m_nodes.at(node_index[targ])->getId();
			t_node->getEdges().push_back(e);
			t_node->getNeigherNodes().push_back(m_nodes.at(node_index[targ]));
		}
		t_node->setMcongraph(m_conGraph);
	}
}

void Network::__updateNeighborGraph() {
	//std::cout << "edges(g) = ";
	Dedge_iter ei, ei_end;
	int weight2;
	for (tie(ei, ei_end) = edges(*m_dGraph); ei != ei_end; ei++) {
		weight2 = m_nodes.at(target(*ei, *m_dGraph))->getPackageNum();
		edges_weight_load[*ei] = weight2 + 1;
	}
	edge_iter chi, chi_end;
	for (tie(chi, chi_end) = edges(*m_conGraph); chi != chi_end; chi++) {
		edges_weight_channel[*chi] = -1;
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

void Network::printWrongCount() {
	float wrongRatio = (float)wrongPCount / (float)totalPCount / 1.000;
	std::cout << "totalCount = " << totalPCount << ";Wrong_Count = " << wrongPCount << endl;
	std::cout << "wrongRatio = " << wrongRatio << endl;
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
void Network::getAllCHbyDP() {
	vector<Node*>::iterator i;
	totalPCount = 0;
	wrongPCount = 0;
	int inputGroupCount = 1;
	int outputNodeCount = 11;
	auto t_output = new double[outputNodeCount*inputGroupCount];
	auto t_Soutput = new double[11 * inputGroupCount];
	for (i = m_nodes.begin(); i != m_nodes.end(); i++) {
		for (int j = 0; j < (*i)->getEdges().size(); j++) {
			int test = (*i)->getEdges().size();
			Node* t_node = *i;
			int* t_CHmarix = new int[11]; 
			totalPCount++;
			int oneCount = 0;
			int pid = t_node->getId();
			t_node->getNet(j).resetGroupCount(inputGroupCount);
			t_node->getNet(j).activeOutputValue((*i)->getInData(), t_Soutput, inputGroupCount);
			for (int n = 0; n < 11; n++) {
				if (t_Soutput[n] > 0.5) {
					t_CHmarix[n] = 1;
					oneCount++;
				}
				else {
					t_CHmarix[n] = 0;
				}
			}
			if (oneCount != 1) {
				wrongPCount++;
			}
			t_node->getCHMatrix().push_back(t_CHmarix);
			t_node->CHMatrixTransferBack();
		}
	}
	while (!m_priNodes.empty()) {
		Node* t_node = m_priNodes.top();
		//std::cout << "node:" << (*i)->getId() << "->edges:" << (*i)->getNeigherNodes().size() << endl;
		//t_node->printIMatrix();
		m_priNodes.pop();
	}
	//printCH();
	delete[] t_output;
	delete[] t_Soutput;
}

void Network::runRounds(int num) {
	for (int i = 0; i < num; i++) {
		runOneRound();
	}
	//cout << "run round:" << num << " finisid!" << endl;
}

void Network::runOneRound() {
	vector<Node*>::iterator i;
	float t_minTime = 9999999999;
	for (i = m_nodes.begin(); i != m_nodes.end(); i++) {
		if ((*i)->getNodeTime() > m_cuTime) {
			continue;
		}
		if ((*i)->isOuterNode()) {
			(*i)->generatePaPerRound();
		}
		if (!(*i)->isQueueEmpty()) {
			Package* t_package = (*i)->outPackage();
			int t_dest = t_package->getDestination();
			int t_nextNodeId = (*i)->getNextNode(t_dest);
			Node* j = m_nodes.at(t_nextNodeId);
			j->inPackage(t_package);
			//float t_sigtime = (*i)->getPerTransSignalDelay();
			//float t_ptime = (*i)->getPerTransDelay();
			if (t_package->isSignaling()) {
				(*i)->getNodeTime() += (*i)->getPerTransSignalDelay();
			}
			else {
				(*i)->getNodeTime() += (*i)->getPerTransDelay();
			}
		}
		else {
			(*i)->getNodeTime() += (*i)->getPerTransDelay();
		}
		if (t_minTime >= (*i)->getNodeTime()) {
			t_minTime = (*i)->getNodeTime();
		}
	}
	m_cuTime = t_minTime;
}

string Network::toString(int a)
{
	char jF[32];
	sprintf(jF, "%d", a);
	string jFirst = jF;
	return jFirst;
}

