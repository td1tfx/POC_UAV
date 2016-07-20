#pragma once
#include "neural/MatrixFunctions.h"
#include "Config.h"
#include "boost/graph/adjacency_list.hpp"
#include "boost/graph/graph_traits.hpp"
#include "boost/graph/edge_list.hpp"
#include "boost/graph/dijkstra_shortest_paths.hpp"
#include <time.h>

using namespace std;
using namespace boost;

typedef property<edge_index_t, string, property<edge_weight_t, int>> EdgeProperty;
typedef property<vertex_index_t, int> VertexIndexProperty;
typedef adjacency_list<vecS, vecS, directedS, VertexIndexProperty, EdgeProperty> Graph;
typedef graph_traits<Graph>::edge_descriptor Edge;
typedef graph_traits<Graph>::edge_iterator edge_iter;
typedef graph_traits<Graph>::vertex_descriptor Vertex;
typedef graph_traits<Graph>::vertex_iterator vertex_iter;
typedef graph_traits<Graph>::out_edge_iterator out_edge_iter;
typedef graph_traits<Graph>::adjacency_iterator adj_iter;

const static double IR[6] = { 13.26,9.21,7.59,4.69,3.84,0 };
const static double ThresholdIF = 1;
using namespace std;
class Compare;

class Node
{
private:
	friend class compare;
	int m_id;
	int chSpcArray[35][3] = { { 0, 0, 0 },{ 0, 1, 0 },{ 6, 1, 0 },{ 7, 1, 0 },{ 8, 1, 0 },{ 9, 1, 0 },
	{ 10, 1, 0 },{ 11, 1, 0 },{ 0, 2, 0 },{ 7, 2, 0 },{ 8, 2, 0 },{ 9, 2, 0 },
	{ 10, 2, 0 },{ 11, 2, 0 },{ 0, 3, 0 },{ 8, 3, 0 },{ 9, 3, 0 },{ 10, 3, 0 },
	{ 11, 3, 0 },{ 0, 4, 0 },{ 9, 4, 0 },{ 10, 4, 0 },{ 11, 4, 0 },{ 0, 5, 0 },
	{ 10, 5, 0 },{ 11, 5, 0 },{ 0, 6, 0 },{ 11, 6, 0 },{ 0, 7, 0 },
	{ 0, 8, 0 },{ 0, 9, 0 },{ 0, 10, 0 },{ 0, 11, 0 },{ 1, 6, 11 } };
	float m_x, m_y, m_z;
	d_matrix* m_IMatrix;
	Graph *m_conGraph;
	vector<Node*> m_neigherNodes;
	vector<Node*> m_nodes;
	vector<int> m_radios;
	vector<Edge> m_edges;

	property_map<Graph, vertex_index_t>::type node_index = get(vertex_index, *m_conGraph);
	property_map<Graph, edge_index_t>::type edges_index = get(edge_index, *m_conGraph);
	property_map<Graph, edge_weight_t>::type edges_weight = get(edge_weight, *m_conGraph);

	void __updateIMatrix(int ch);
	void __initIMatrix();
	void __updateIF(int ch);
	bool __isCAValid(int ch);
	float __getIF(int dist, int ch1, int ch2);
	int __chooseRadio(Node n);


public:
	
	Node();
	~Node();
	void printIMatrix();

	void setId(int id) { m_id = id; }
	void setPos(int x, int y, int z) { m_x = x; m_y = y; m_z = z; }
	void setRadios(int radios[]) {setRadios(radios[0], radios[1], radios[2]);}
	void setRamdomRadios() {
		int rnd = rand() / sizeof(chSpcArray);
		setRadios(chSpcArray[rnd]);
	}
	void setRadios(int r0, int r1, int r2) {
		m_radios.clear();
		m_radios.push_back(r0);
		m_radios.push_back(r1);
		m_radios.push_back(r2);
	}
	void setMNodes(vector<Node*> t) {
		m_nodes = t;
	}
	void setMcongraph(Graph* t) {
		m_conGraph = t;
	}



	int& getId() { return m_id; }
	vector<int>& getRadios() { return m_radios; }
	float& getX() { return m_x; }
	float& getY() { return m_y; }
	float& getZ() { return m_z; }
	d_matrix* getIMatrix() { return m_IMatrix; }
	vector<Node*>& getNeigherNodes() {
		return m_neigherNodes;
	}
	vector<Edge>& getEdges() {
		return m_edges;
	}
	bool lessthan(const Node* node) const
	{
		return m_neigherNodes.size() < ((Node*)node)->getNeigherNodes().size();
	}

	float getDistance(Node m);
	void channelAssignment();
};

class Compare
{
public:
	bool operator () (Node* t1, Node* t2)
	{
		return t1->lessthan(t2);
	}
};
