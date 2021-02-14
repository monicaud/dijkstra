#include "Graph.hpp"
#include <climits>
/*
	This program uses Dijkstra's algorithm to find the shortest path between nodes/vertices of a graph
	-Monica Ulloa Diaz
*/
Graph::Graph() {

}
Graph::~Graph() {

}
void Graph::addVertex(std::string label) {
	//Creates and adds a vertex to the graph with label. No two vertices should have the same label. 
	bool already_exists = false;
	for (auto v : vect) {
		if (v.get_lbl() == label) {
			already_exists = true;
		}
	}
	if (!already_exists) {
		vect.push_back(Vertex(label));
	}
	
}
void Graph::removeVertex(std::string label) {
	//Removes the vertex with label from the graph. Also removes the edges between that vertex and
	//the other vertices of the graph.
	int ind = 0;
	int count = 0;
	for (auto v : vect) { //find the vertex
		if (v.get_lbl() == label) {
			ind = count; //get the index
		}
		count++;
	}

	vector<Vertex>::iterator iter = vect.begin() + ind; //make an iterator for said index
	vector<Edge*> copy_e = iter->my_edges; //make a copy of what edges need to be deleted
	vect.erase(iter); //erase vertex from vector
	
	for (auto e : copy_e) {
		if (e->sv1 == label) {//then sv2 is the label of the other vertex
			find_edge(label, e->sv2); //find_edge takes care of deletion of edge from the other vertex
		}
		if (e->sv2 == label) { //then sv1 is the label of other vertex
			find_edge(label, e->sv1);
		}
	}	
}
void Graph::addEdge(std::string label1, std::string label2, unsigned long weight) {
	/*	Adds an edge of value weight to the graph between the vertex with label1 and the vertex with
		label2. A vertex with label1 and a vertex with label2 must both exist, there must not already be
		an edge between those vertices, and a vertex cannot have an edge to itself. 
	*/
	int iv1 = 0; //to keep track of where we find first vertex
	int iv2 = 0; //second vertex
	int count = 0;
	
	bool v1_exists = false;
	bool v2_exists = false;
	
	for (auto v : vect) {//gotta find whether the vertices exist
		if (v.get_lbl() == label1) {
			iv1 = count; //store the index of the vertex
			v1_exists = true;
		}
		if (v.get_lbl() == label2) {
			iv2 = count;
			v2_exists = true;
		}
		count++;
	}
	//can create new edge when the edge doesn't already exist and the vertices exist
	if (!is_connected(label1, label2) && v1_exists && v2_exists) {
		Edge* e = new Edge(weight, label1, label2); //create new edge
		vect.at(iv1).my_edges.push_back(e); //add it to respective vertices
		vect.at(iv2).my_edges.push_back(e);
	}
	else {
		cout << "this edge couldn't be added";
	}
	
	
}
void Graph::removeEdge(std::string label1, std::string label2) {
	/*	Removes the edge from the graph between the vertex with label1 and the vertex with label2. A
		vertex with label1 and a vertex with label2 must both exist and there must be an edge between
		those vertices 
	*/
	for (vector<Vertex>::iterator it = vect.begin(); it != vect.end(); it++) { //go thru vertices
		for (vector<Edge*>::iterator it2 = it->my_edges.begin(); it2 != it->my_edges.end();) { //go thru edges of those vertices
			if (((*it2)->sv1 == label1 && (*it2)->sv2 == label2) || ((*it2)->sv2 == label1 && (*it2)->sv1 == label2)) {
				//once we find the desired edge
				it2 =  it->my_edges.erase(it2); //remove it
			}
			else {
				it2++; //if we don't find it here, move on
			}
		}
	}	
}
unsigned long Graph::shortestPath(std::string startLabel, std::string endLabel, std::vector<std::string> &path) {
	/*Calculates the shortest path between the vertex with startLabel and the vertex with endLabel
	using Dijkstra's Algorithm. A vector is passed into the method that stores the shortest path
	between the vertices. The return value is the sum of the edges between the start and end
	vertices on the shortest path.*/

	/*
	1. set dist at startLabel to 0
	2. have a vector of visited nodes
	3. set all other distances to max of long
	4. visit neighbor nodes
	5. update their distance based on edge weight
	6. go to smallest dist
	7. check adjacent nodes
	8. if smaller distance found, update said distance (if dist[v] + edge(v,u) < dist[u]: update)
	*/
	int index_end = 0; //index of begin and ending vertex
	int index_str = 0;
	int count = 0;
	int ind_count = 0;
	
	/*********setting all distances to max except starting label*****************/
	for (vector<Vertex>::iterator it = vect.begin(); it != vect.end(); it++) {//use this loop to populate the map with the string keys
		it->dist = ULONG_MAX;
		
		if (it->get_lbl() == startLabel) {
			it->dist = 0;
			index_str = ind_count;
		}
		
		if (it->get_lbl() == endLabel) {
			index_end = ind_count;
		}
		ind_count++;
	}

	ind_count = 0; //reset it to reuse it
	
	/**************start visited vector*****************/
	vector<string> visited;

	visited.push_back(startLabel);
	for (vector<Vertex>::iterator it = vect.begin(); it != vect.end(); it++) {
		
		if (is_connected(startLabel, it->get_lbl())) {
			//if this node is connected to start, update its distance
			it->dist = edge_weight(startLabel, it->get_lbl());
			
			it->updated_by = startLabel; //each node will know which node it got updated by
		}
	}

	count = 0;
	/*REPEATING STEPS-----------------------------------------------------*/
	while (!is_visited(visited, endLabel)) { //while the visited vect doesnt include endlabel
		unsigned long min = ULONG_MAX;
		
		string loc = ""; //holds the label of the current node with smallest distance
		
		for (auto v : vect) { //finding the minimum distance
			
			if( (v.dist < min) && !is_visited(visited,v.get_lbl())) {
				//if we find a value that is smaller than min and isn't visited...
				min = v.dist;
				loc = v.get_lbl();
				ind_count = count; //get the index of minimum within vect
			
			}
			count++;
		}
		count = 0;
		for (vector<Vertex>::iterator v = vect.begin(); v != vect.end(); v++) { 
			//go thru vect, try to find which are connected to current vertex (loc)
			
			//if current vertex is connected to desired node, and hasnt been visited
			if (is_connected(loc, v->get_lbl()) && !is_visited(visited,v->get_lbl())) { 
				//RELAX that node:
				
				//if the distance is greater than what it could be, update it
				if (v->dist > vect.at(ind_count).dist + edge_weight(vect.at(ind_count).get_lbl(), v->get_lbl())) {
					v->dist = vect.at(ind_count).dist + edge_weight(vect.at(ind_count).get_lbl(), v->get_lbl());
					
					v->updated_by = vect.at(ind_count).get_lbl();
				
				}
			}
		}
		//add the visited node into the visited vector
		visited.push_back(loc);
	}
	
	//pointer vertex to look at prev vertex which updated the distance of current vertex
	Vertex* pv = &vect.at(index_end);
	vector<string> temp_p; //holds the path but backwards
	
	while (!has(temp_p, startLabel)) { //while start label hasnt been pushed into the vector
		temp_p.push_back(pv->get_lbl()); //push pv into path vector
		pv = &vect.at(get_index(pv->updated_by)); //get the vertex which updated this vertex's distance
	}
	//push the path into the correct vector in the correct order
	for (vector<string>::iterator it = temp_p.end(); it != temp_p.begin(); ) {
		it--;
		path.push_back(*it);
	}
	
	return vect.at(index_end).dist;
	return 1;
}
bool Graph:: is_connected(string lbl1, string lbl2, int& flag1, int& flag2) {
	//this version of the function keeps track of where the desired edge appears 
	//within each vertex's vector as they may be at different indices

	int index = 0;
	for (auto v : vect) { //going thru the vertices in my vector
		if (v.get_lbl() == lbl1) { //if we find the first label
			for (auto e : v.my_edges) { //go thru its edge vector
				if ((e->sv1 == lbl1 && e->sv2 == lbl2) || (e->sv2 == lbl1 && e->sv1 == lbl2)) {
					//if we found the correct edge in the first vertex's edge vector
					return true;
					flag1 = index;
				}
			}
		}
		if (v.get_lbl() == lbl2) { //if we find the second label
			for (auto e : v.my_edges) { //go thru its edge vector
				if ((e->sv1 == lbl1 && e->sv2 == lbl2) || (e->sv2 == lbl1 && e->sv1 == lbl2)) {
					//if we found the correct edge in the first vertex's edge vector
					return true;
					flag2 = index;
				}
			}
		}
		index++;
	}
	return false;
}
bool Graph::is_connected(string lbl1, string lbl2){ //one without a flag

	for (auto v : vect) { //going thru the vertices in my vector
		if (v.get_lbl() == lbl1) { //if we find the first label
			for (auto e : v.my_edges) { //go thru its edge vector
				if ((e->sv1 == lbl1 && e->sv2 == lbl2) || (e->sv2 == lbl1 && e->sv1 == lbl2)) {
					//if we found the correct edge
					return true;
				}
			}
		}
	}
	return false;
}
void Graph::find_edge(string del_v, string lbl2) {
	//this function is used by the remove vertex function
	int pos = 0;
	int count = 0;
	int pos_e;
	for (auto v : vect) {
		if (v.get_lbl() == lbl2) {
			pos = count; //we found position of desired vertex
		}
		count++;
	}
	count = 0;

	for (auto e : vect.at(pos).my_edges) { 
		//look through the edges of vertex to delete that desired edge connected to old v
		if (e->sv1 == del_v || e->sv2 == del_v) {
			pos_e = count;
		}
		count++;
	}
	//get desired position of edge to be deleted in that v's vector
	vector<Edge*>::iterator iter = vect.at(pos).my_edges.begin() + pos_e;
	vect.at(pos).my_edges.erase(iter); //erase said vector
}
int Graph::get_index(string lbl) {
	//return the index in vect for desired label
	int count = 0;
	int found = 0;
	for (auto v : vect) {
		if (v.get_lbl() == lbl) {
			found = count;
		}
		count++;
	}
	return found;
}
int Graph::edge_weight(string lbl1, string lbl2) {
	//find the weight between desired vertices
	for (vector<Vertex>::iterator it = vect.begin(); it != vect.end(); it++) {
		for (vector<Edge*>::iterator it2 = it->my_edges.begin(); it2 != it->my_edges.end();) {
			if (((*it2)->sv1 == lbl1 && (*it2)->sv2 == lbl2) || ((*it2)->sv2 == lbl1 && (*it2)->sv1 == lbl2)) {
				return (*it2)->weight;
			}
			else {
				it2++;
			}
		}
	}
	return 0;
}
bool Graph::has(vector<string> v, string lbl) {
	//does this vector have this label
	for (auto it : v) {
		if (it == lbl) {
			return true;
		}
	}
	return false;
}
bool Graph::is_visited(vector<string> vis, string lbl) {
	for (auto v : vis) { //look thru vector of visited vertices
		if (v == lbl) { //if label is found in vector
			return true; //that label has been visited
		}
	}
	return false;
}
