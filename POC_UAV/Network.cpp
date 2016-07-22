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
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 5; j++) {
			Node* t_node = new Node();
			t_node->setId(i * 5 + j);
			t_node->setPos(i * 10, j * 10, 0);
			//t_node->setRamdomRadios();
			m_nodes.push_back(t_node);
		}
	}
	return m_nodes.size();
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
	if (__initRandom()) {
		//cuTime = 0;
		m_conGraph = new Graph;
		__createNeighborGraph();
		__updatePri();
	}
}

void Network::initGraphByFile() {
	if (__initRandomFromFile()) {
		//cuTime = 0;
		m_conGraph = new Graph;
		__createNeighborGraph();
		__updatePri();
	}
}

void Network::__updatePri() {
	if (!m_priNodes.empty()) {
		std::cout << "wrong!!!priNodes is not empty!" << endl;
		return;
	}
	vector<Node*>::iterator i;
	for (i = m_nodes.begin(); i != m_nodes.end(); i++) {
		m_priNodes.push(*i);
		(*i)->setMNodes(m_nodes);
	}
}

void Network::runPOC()
{
	while (!m_priNodes.empty()) {
		//(*i)->printIMatrix();
		auto t_node = (Node*)m_priNodes.top();		
		std::cout << "node:" << t_node->getId() << "->edges:" << t_node->getNeigherNodes().size() << endl;
		t_node->channelAssignment();
		t_node->printIMatrix();
		m_priNodes.pop();		
	}
	printCH();
	system("pause");
}

void Network::runNormal()
{
	while (!m_priNodes.empty()) {
		//(*i)->printIMatrix();
		auto t_node = (Node*)m_priNodes.top();
		std::cout << "node:" << t_node->getId() << "->edges:" << t_node->getNeigherNodes().size() << endl;
		t_node->channelAssignmentNormal();
		t_node->printOCMatrix();
		m_priNodes.pop();
	}
	printCH();
	system("pause");
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
				Edge ed;
				ed = (add_edge((*i)->getId(), (*j)->getId(), *m_conGraph)).first;
				edges_index[ed] = linkEdge;
				edges_weight[ed] = -1;
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
		std::cout << edges_index[*ei] << "CH:" << edges_weight[*ei] + 1 << "---";
		if (edges_weight[*ei] >= 0) {
			connecty_num++;
		}
		edge_num++;
	}
	std::cout << endl;
	std::cout << "edge_num =" << edge_num << endl;
	std::cout << "connecty_num =" << connecty_num << endl;
}


string Network::toString(int a)
{
	char jF[32];
	sprintf(jF, "%d", a);
	string jFirst = jF;
	return jFirst;
}