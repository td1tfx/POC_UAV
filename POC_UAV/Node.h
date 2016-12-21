#pragma once
#include "neural/MatrixFunctions.h"
#include "Config.h"
#include "Package.h"
#include <iostream>
#include <queue>
#include "neural/lib/libconvert.h"
#include "neural/NeuralNet.h"
#include "boost/graph/adjacency_list.hpp"
#include "boost/graph/graph_traits.hpp"
#include "boost/graph/edge_list.hpp"
#include "boost/graph/dijkstra_shortest_paths.hpp"
#include <time.h>
#include "neural/lib/Timer.h"
#include <corecrt_io.h>
#include <direct.h>
#include "venue.h"


#define INFINITY   9999999;
using namespace std;
using namespace boost;

typedef property<edge_index_t, string, property<edge_weight_t, int>> EdgeProperty;
typedef property<vertex_index_t, int> VertexIndexProperty;
typedef adjacency_list<vecS, vecS, undirectedS, VertexIndexProperty, EdgeProperty> Graph;
typedef adjacency_list<vecS, vecS, directedS, VertexIndexProperty, EdgeProperty> DGraph;
typedef graph_traits<Graph>::edge_descriptor Edge;
typedef graph_traits<DGraph>::edge_descriptor DEdge;
typedef graph_traits<Graph>::edge_iterator edge_iter;
typedef graph_traits<DGraph>::edge_iterator Dedge_iter;
typedef graph_traits<Graph>::vertex_descriptor Vertex;
typedef graph_traits<DGraph>::vertex_descriptor DVertex;
typedef graph_traits<Graph>::vertex_iterator vertex_iter;
typedef graph_traits<Graph>::out_edge_iterator out_edge_iter;
typedef graph_traits<Graph>::adjacency_iterator adj_iter;

const static double IR[6] = { 13.26,9.21,7.59,4.69,3.84,0 };
static double ThresholdIF = 1;
using namespace std;
class Compare;

class Node
{
private:

	friend class compare;

	//int* m_CHNum;
	int m_id;
	int m_linkNum;
	int m_packageCount;
	int m_GWNum;
	int m_GWHop;
	int m_random;
	int m_sigCount;
	int m_pacNum = 0;
	int m_speed = 10;
	int m_outputCount;
	float allDelay = 0;
	float allOnehopDelay = 0;
	float m_nodeTime;
	float m_packageRate;
	float m_x, m_y, m_z;
	float m_perTransDelay;
	float m_perTransSignalDelay;
	float m_energy;
	float m_transRange = 70;
	float m_shortestDistance = 999999999;
	d_matrix* m_IMatrix;
	d_matrix* m_IMatrix_copy;
	d_matrix* m_OCMatrix;
	d_matrix* m_routingMatrix;	//routing Matrix with binary
	d_matrix* m_trainRoutingMatrix;   //routing Matrix with binary after training 
	d_matrix* m_shortRouting;   //routing Matrix with decimal
	Graph *m_conGraph;
	vector<Node*> m_neigherNodes;
	vector<Node*> m_nodes;
	vector<Node*> m_outerNodes;
	vector<int*> m_CHMatrix;
	vector<Edge> m_edges;
	vector<float> m_utility;
	std::queue<Package*> m_qServe;
	std::queue<Package*> m_qFinished;
	bool m_isGW;
	bool m_isOuterNode;
	double* m_inData;
	double* m_outData;
	NeuralNet* m_netQ;


	property_map<Graph, vertex_index_t>::type node_index = get(vertex_index, *m_conGraph);
	property_map<Graph, edge_index_t>::type edges_index = get(edge_index, *m_conGraph);
	//property_map<DGraph, edge_index_t>::type edges_index_d = get(edge_index, *m_dGraph);
	property_map<Graph, edge_weight_t>::type edges_weight_channel = get(edge_weight, *m_conGraph);
	//property_map<DGraph, edge_weight_t>::type edges_weight_load = get(edge_weight, *m_dGraph);

	void __init();
	void __updateIMatrix(Edge t_edge);
	void __updateIMatrixCopy(Edge t_edge, int ch);
	void __updateIMatrixNormal(Edge t_edge);
	void __initIMatrix();
	void __initOCMatrix();
	void __updateIF();
	void __updateIFCopy();
	void __CHMatrixTransfer();	
	bool __isCAValid(int ch);
	float __getIF(float dist, int ch1, int ch2);
	int __chooseRadio(Node n);
	int __chooseRadioNormal(Node n);

protected:
	enum type {
		type_node = 0,
		type_UAV = 1,
		type_user = 2,
		type_cloud = 3,
		type_cloudlet = 4
	}p_type;

public:
	
	Node();
	~Node();
	void printIMatrix();
	void printOCMatrix();

	void setId(int id) { m_id = id; }
	void setPos(int x, int y, int z) { m_x = x; m_y = y; m_z = z; }
	void setRandom(int random) { m_random = random; }
	void setMNodes(vector<Node*> t) { m_nodes = t; }
	void setOuterNodes(vector<Node*> t) { m_outerNodes = t; }
	void setMcongraph(Graph* t) {
		m_conGraph = t;
	}


	int& getGWNum() { return m_GWNum; }
	int& getId() { return m_id; }
	int& getLinkNum() { return m_linkNum; }
	int& getGWHop() { return m_GWHop; }
	int getRandom() { return m_random; }
	int getPackageNum() { return m_qServe.size();}
	int getNextNode(int dest) {	return m_shortRouting->getData(dest, 0); }
	int getFinalPacNum() { return m_pacNum;	}
	int getOutputCount() { return m_outputCount; }
	bool& getIsGW() { return m_isGW; }
	bool& isOuterNode() { return m_isOuterNode; }
	bool isQueueEmpty() { return m_qServe.empty(); }
	float& getX() { return m_x; }
	float& getY() { return m_y; }
	float& getZ() { return m_z; }
	float& getPackageRate() { return m_packageRate; }
	float& getNodeTime() { return m_nodeTime; }
	float& getEnergy() { return m_energy; }
	float& getShortestDistance() { return m_shortestDistance; }
	float getAllDelay() { return allDelay; }
	float getAllOnehopDelay() {	return allOnehopDelay; }
	virtual float& getTransRange() { return m_transRange; }
	float getPerTransDelay() { return m_perTransDelay; }
	float getPerTransSignalDelay() { return m_perTransSignalDelay; }
	double*& getInData() { return m_inData; }
	d_matrix* getIMatrix() { return m_IMatrix; }
	d_matrix* getIMatrixCopy() { return m_IMatrix_copy; }
	d_matrix* getOCMatrix() { return m_OCMatrix; }
	d_matrix*& getRoutingMatrix() { return m_routingMatrix; }
	d_matrix*& getShortPath() {	return m_shortRouting;	}
	d_matrix*& getTrainPath() { return m_trainRoutingMatrix; }
	vector<Node*>& getNeigherNodes() { return m_neigherNodes; }
	vector<Edge>& getEdges() { return m_edges; }
	vector<int*>& getCHMatrix() { return m_CHMatrix; }
	NeuralNet& getNet(int i) { return m_netQ[i]; }
	void copyIMatrix() {d_matrix::cpyData(m_IMatrix_copy, m_IMatrix);}
	bool lessthan(const Node* node) const
	{
		return m_qServe.size() < ((Node*)node)->getPackageNum();
	}
	bool lessthanRandom(const Node* node) const
	{
		return m_random < ((Node*)node)->m_random;
	}

	float getDistance(Node m);
	float calculateUtilityInCoCAG();
	float calculateUtilityInAoCAG();
	float bestResponseInCoCAG();
	float bestResponseInAoCAG();
	void channelAssignment();
	void channelAssignmentNormal();
	void generatePackage(int destType = 0, int dest = -1, float nodeTime = -1); //0: GWdest, 1:randomOuterDest
	void generatePaPerRound(int destType = 0); //0: GWdest, 1:randomOuterDest
	void channelAssignmentCopy(int ch);
	void saveLinkData(int inDataSize, bool clean, int dataType = 0);
	void saveRoutingData(int inDataSize, bool clean, int dataType = 0, int dest = -1);
	void initNerualNet();
	void CHMatrixTransferBack();
	void inPackage(Package* in_package,int recType = 0); //type: 0, cloudlet, 1, cloud 
	void initCHMatrix();
	void initMatrix();
	void initialPackage();
	void calculateDelay(bool isTrained = false);
	void moveRandom();
	Package* outPackage();


};

class Compare
{
public:
	bool operator () (Node* t1, Node* t2)
	{
		return t1->lessthan(t2);
	}
};

class CompareRandom
{
public:
	bool operator () (Node* t1, Node* t2)
	{
		return t1->lessthanRandom(t2);
	}
};