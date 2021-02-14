#pragma once
/*
Each vertex will contain a vector of pointers to Edges
Each edge has data members containing labels of the vertices it connects
*/

#include <string>
using namespace std;

//I left the functions here because they are really small and probably could do without a whole new file
class Edge {
public:

	Edge(unsigned long weight, string s1, string s2) {
		this->weight = weight;
		sv1 = s1;
		sv2 = s2;
	}

	unsigned long weight;
	string sv1;
	string sv2;
};
class Vertex {
public:
	Vertex(string label) {
		this->label = label;
		dist = 0;
	}
	string get_lbl() {
		return label;
	}
	string label;
	unsigned long dist;
	vector<Edge*> my_edges;
	string updated_by;
	
};
