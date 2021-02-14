#pragma once
#include "GraphBase.hpp"
#include "Vertex.hpp"
#include <vector>
#include <map>
#include <iostream>
//The graph has a vector of vertices each of which contain their own vector of edges
class Graph {
private:
	vector <Vertex> vect;
public:
	Graph();
	~Graph();
	//Newly added functions
	bool is_connected(string lbl1, string lbl2, int& flag1, int& flag2);
	bool is_connected(string lbl1, string lbl2);
	void find_edge(string del_v, string lbl2);
	int get_index(string lbl);
	int edge_weight(string lbl1, string lbl2);
	bool has(vector<string> v, string lbl);
	bool is_visited(vector<string> vis, string lbl);
	//Virtual functions
	void addVertex(std::string label);
	void removeVertex(std::string label);
	void addEdge(std::string label1, std::string label2, unsigned long weight);
	void removeEdge(std::string label1, std::string label2);
	unsigned long shortestPath(std::string startLabel, std::string endLabel, std::vector<std::string> &path);
};
 