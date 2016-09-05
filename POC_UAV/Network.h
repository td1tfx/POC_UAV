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
	priority_queue<Node*, vector<Node*>, CompareRandom> m_priNodesRandom;
	Graph *m_conGraph;		//graph of channel assignment
	DGraph *m_dGraph;		//graph of routing
	property_map<Graph, vertex_index_t>::type node_index = get(vertex_index, *m_conGraph);
	property_map<Graph, edge_index_t>::type edges_index = get(edge_index, *m_conGraph);
	property_map<DGraph, edge_index_t>::type edges_index_d = get(edge_index, *m_dGraph);
	property_map<Graph, edge_weight_t>::type edges_weight_channel = get(edge_weight, *m_conGraph);
	property_map<DGraph, edge_weight_t>::type edges_weight_load = get(edge_weight, *m_dGraph);
	double* m_inData;
	int m_inDataSize;

	int __initGrid();
	int __initRandom();
	int __initRandomFromFile();
	void __createNeighborGraph();
	void __updateNeighborGraph();
	void __updatePribyLinkNum();
	void __updatePribyLoad();
	void __updatePribyRandom();
	void __getNodesLoad();
	bool __getShortestPath(int destId);

public:

	int totalPCount;
	int wrongPCount;
	int iterTimes;
	Network();
	~Network();
	void runPOC(int num);
	void runNormal();
	float runPOCGame(int num);

	void initGraph();
	void initGraphByFile();
	void printCH();
	void printPath();
	void getAllShortestPath();
	void initTrainNet();
	void getAllTrainedPath();
	int trainNet();


	string toString(int a);
};

