#pragma once
#include "Node.h"
#include <vector>
#include <queue> 
#include "Cloudlet.h"
#include "Cloud.h"
#include "User.h"
#include "UAV.h"



using namespace std;
class Network
{
private:
	vector<Node*> m_nodes;
	vector<Node*> m_outerNodes;
	vector<UAV*> m_UAVs;
	vector<Cloudlet*> m_cloudlets;
	vector<User*> m_users;
	Cloud* m_cloud;
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
	float m_cuTime;

	int __initCloudletFrame();
	int __initCloudFrame();
	int __initGrid();
	int __initRandom();
	int __initRandomFromFile();
	void __createNeighborGraph();
	void __createCloudletGraph();
	void __updateNeighborGraph();
	void __updatePribyLinkNum();
	void __getNodesLoad();
	bool __getShortestPath(int destId, int type = 0);
	bool __getShortestPath(Node* t_node, int type = 0);
	void __runOneRound();
	void __runOneCloudRound(int cloudType = 0);

public:

	int totalPCount;
	int wrongPCount;
	int iterTimes;
	Network();
	~Network();

	float getCuTime() { return m_cuTime; }

	void runPOC(int num);
	void runNormal();
	float runPOCGame(int num, bool isSave = true);

	void initGraph();
	void initGraphByFile();
	void initCloudletGraph();
	void printCH();
	void printPath();
	void printWrongCount();
	void getAllShortestPath();
	void getAllTypeShortestPath();
	void initTrainNet();
	void getAllCHbyDP();
	void runRounds(int num);
	void runCloudRounds(int num);
	void runCloudletRounds(int num);
	void updatePribyLoad();
	void updatePribyRandom();
	void saveDelay(bool isTrained = false, double genarateRate = 1);
	int trainNet();



	string toString(int a);
};

