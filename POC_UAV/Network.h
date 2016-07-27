#pragma once
#include "Node.h"
#include <vector>
#include <queue> 



using namespace std;
class Network
{
private:
	vector<Node*> m_nodes;
	vector<Node*> m_outerNodes;
	priority_queue<Node*, vector<Node*>, Compare> m_priNodes;
	Graph *m_conGraph;
	property_map<Graph, vertex_index_t>::type node_index = get(vertex_index, *m_conGraph);
	property_map<Graph, edge_index_t>::type edges_index = get(edge_index, *m_conGraph);
	property_map<Graph, edge_weight_t>::type edges_weight = get(edge_weight, *m_conGraph);
	double* m_inData;
	int m_inDataSize;

	int __initGrid();
	int __initRandom();
	int __initRandomFromFile();
	void __createNeighborGraph();
	void __updateNeighborGraph();
	void __updatePribyLinkNum();
	void __updatePribyLoad();
	void __getNodesLoad();
	bool __getShortestPath(int destId);

public:

	Network();
	~Network();
	void runPOC();
	void runNormal();

	void initGraph();
	void initGraphByFile();
	void printCH();
	void getAllShortestPath();



	string toString(int a);
};

