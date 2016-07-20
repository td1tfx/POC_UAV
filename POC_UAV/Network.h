#pragma once
#include "Node.h"
#include <vector>
#include <queue> 



using namespace std;
class Network
{
private:
	vector<Node*> m_nodes;
	priority_queue<Node*, vector<Node*>, Compare> m_priNodes;
	Graph *m_conGraph;
	property_map<Graph, vertex_index_t>::type node_index = get(vertex_index, *m_conGraph);
	property_map<Graph, edge_index_t>::type edges_index = get(edge_index, *m_conGraph);
	property_map<Graph, edge_weight_t>::type edges_weight = get(edge_weight, *m_conGraph);

	int __initGrid();
	int __initRandom();
	void __createNeighborGraph();
	void __updatePri();

public:

	Network();
	~Network();
	void run();

	void initGraph();
	string toString(int a);
	void printCH();
};

