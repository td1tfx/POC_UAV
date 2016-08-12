#pragma once
#include "neural/MatrixFunctions.h"
#include "Config.h"
#include "Package.h"
#include <queue>
#include "neural/lib/libconvert.h"
#include "boost/graph/adjacency_list.hpp"
#include "boost/graph/graph_traits.hpp"
#include "boost/graph/edge_list.hpp"
#include "boost/graph/dijkstra_shortest_paths.hpp"
#include <time.h>

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
	int m_id;
	int m_linkNum;
	int m_packageCount;
	int m_GWNum;
	int m_GWHop;
	float m_nodeTime;
	float m_packageRate;
	float m_x, m_y, m_z;
	d_matrix* m_IMatrix;
	d_matrix* m_IMatrix_copy;
	d_matrix* m_OCMatrix;
	d_matrix* m_routingMatrix;	//routing Matrix with binary
	d_matrix* m_shortRouting;   //routing Matrix with decimal
	Graph *m_conGraph;
	vector<Node*> m_neigherNodes;
	vector<Node*> m_nodes;
	vector<Node*> m_outerNodes;
	vector<Edge> m_edges;
	std::queue<Package*> m_qServe;
	std::queue<Package*> m_qFinished;
	bool m_isGW;
	bool m_isOuterNode;
	double* m_inData;
	double* m_outData;


	property_map<Graph, vertex_index_t>::type node_index = get(vertex_index, *m_conGraph);
	property_map<Graph, edge_index_t>::type edges_index = get(edge_index, *m_conGraph);
	//property_map<DGraph, edge_index_t>::type edges_index_d = get(edge_index, *m_dGraph);
	property_map<Graph, edge_weight_t>::type edges_weight_channel = get(edge_weight, *m_conGraph);
	//property_map<DGraph, edge_weight_t>::type edges_weight_load = get(edge_weight, *m_dGraph);

	void __init();
	void __updateIMatrix(Edge t_edge);
	void __updateIMatrixNormal(Edge t_edge);
	void __initIMatrix();
	void __initOCMatrix();
	void __updateIF();
	bool __isCAValid(int ch);
	float __getIF(float dist, int ch1, int ch2);
	float __calculateUtility();
	int __chooseRadio(Node n);
	int __chooseRadioNormal(Node n);


public:
	
	Node();
	~Node();
	void printIMatrix();
	void printOCMatrix();

	void setId(int id) { m_id = id; }
	void setPos(int x, int y, int z) { m_x = x; m_y = y; m_z = z; }
	void setMNodes(vector<Node*> t) { m_nodes = t; }
	void setOuterNodes(vector<Node*> t) { m_outerNodes = t; }
	void setMcongraph(Graph* t) {
		m_conGraph = t;
	}


	int& getGWNum() { return m_GWNum; }
	int& getId() { return m_id; }
	int& getLinkNum() { return m_linkNum; }
	int& getGWHop() { return m_GWHop; }
	int getPackageNum() { return m_qServe.size();}
	bool& getIsGW() { return m_isGW; }
	bool& isOuterNode() { return m_isOuterNode; }
	float& getX() { return m_x; }
	float& getY() { return m_y; }
	float& getZ() { return m_z; }
	float& getPackageRate() { return m_packageRate; }
	double*& getInData() { return m_inData; }
	d_matrix* getIMatrix() { return m_IMatrix; }
	d_matrix* getOCMatrix() { return m_OCMatrix; }
	d_matrix*& getRoutingMatrix() { return m_routingMatrix; }
	d_matrix*& getShortPath() {	return m_shortRouting;	}
	vector<Node*>& getNeigherNodes() { return m_neigherNodes; }
	vector<Edge>& getEdges() { return m_edges; }
	bool lessthan(const Node* node) const
	{
		return m_neigherNodes.size() < ((Node*)node)->getNeigherNodes().size();
	}

	float getDistance(Node m);
	void channelAssignment();
	void channelAssignmentNormal();
	void generatePackage();
	void generatePaPerRound();
	int bestResponse();


};

class Compare
{
public:
	bool operator () (Node* t1, Node* t2)
	{
		return t1->lessthan(t2);
	}
};
