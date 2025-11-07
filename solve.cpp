#include <iostream>
#include <vector>
#include <string>
#include <unordered_set>
#include <queue>
#include <unordered_map>
#include "solve.h"
#include "vertex.h"

using namespace std;

string solve(string maze) {
	unordered_map<int, Vertex*> vertexMap;
	size_t rowLength = maze.find('\n') + 1;
	int counter = 0;
	int xCoor = 0;
	int yCoor = 0;
	int xWidth = rowLength - 1;
	int yHeight = 0;

	while (counter < maze.size()) {		// Runs in O(s) time
		// Create vertices for each open space in the maze
		char curr = maze[counter];
		if (curr == ' ') {
			vertexMap[counter] = new Vertex(yCoor, xCoor);
			if (xCoor > 0 && maze[counter - 1] == ' ') {		// Left neighbor
				vertexMap[counter]->neighs.push_back(vertexMap[counter - 1]);
				vertexMap[counter - 1]->neighs.push_back(vertexMap[counter]);

			}
			if (yCoor > 0 && maze[counter - rowLength] == ' ') {		// Top neighbor
				vertexMap[counter]->neighs.push_back(vertexMap[counter - rowLength]);
				vertexMap[counter - rowLength]->neighs.push_back(vertexMap[counter]);
			}
			//solvedMaze += ' ';
			xCoor++;
		}
		else if (curr == '\n') {
			yCoor++;
			xCoor = 0;
			//solvedMaze += curr;
		}
		else {
			xCoor++;
			//solvedMaze += curr;
		}
		counter++;
	}
	yHeight = yCoor;

	//cout << "Map is width: " << xWidth << " and Height: " << yHeight << endl;

	Vertex* startVertex = nullptr;
	Vertex* endVertex = nullptr;
	unordered_map<Vertex*, Vertex*> breadCrumbs;
	unordered_set<Vertex*> marked;
	queue<Vertex*> Q;

	//	Step 0: Find start and end vertices
	for (const auto& pair : vertexMap) {		// Runs in O(s) time
		if (pair.second->row == 0 || pair.second->row == yHeight - 1 ||
			pair.second->col == 0 || pair.second->col == xWidth - 1) {
			if (startVertex == nullptr) {
				startVertex = pair.second;
			}
			else {
				endVertex = pair.second;
			}
		}
	}

	// Step 1: BFS to find path from start to all vertices
	// Step 1a: Instead of calling breadthFirstSearch(s, breadCrumbs);
	// startVertex is s
	// breadCrumbs is bc
	// marked is marked
	// Q is Q

	marked.insert(startVertex);
	Q.push(startVertex);

	while (!Q.empty()) {		// Runs in O(V + E) time
		Vertex* x = Q.front();
		Q.pop();

		for (Vertex* y : x->neighs) {
			if (marked.find(y) == marked.end()) {
				marked.insert(y);
				breadCrumbs[y] = x;
				Q.push(y);
			}
		}
	}

	// Just in case, check if endVertex was reached
	if (breadCrumbs.find(endVertex) == breadCrumbs.end()) {
		// No path was found!
		return "No solution"; // Or return the original maze
	}

	// Step 2: Trace breadCrumbs from end to start to mark path
	// from endVertex to startVertex

	Vertex* current = endVertex;
	while (current != startVertex) {		// Runs in O(d) time, d = distance from start to end
		int index = current->row * rowLength + current->col;	// Nice calculation to find its position in the original index
		maze[index] = 'o';
		current = breadCrumbs[current];
		//clearScreen();
		//cout << maze << endl;
		//std::this_thread::sleep_for(std::chrono::seconds(1)); // Wait 1 second
	}

	// Mark the start vertex as well
	int startIndex = startVertex->row * rowLength + startVertex->col;
	maze[startIndex] = 'o';
	// Optional: Print out the solved maze at each step
	//clearScreen();
	//cout << maze << endl;
	//std::this_thread::sleep_for(std::chrono::seconds(1)); // Wait 1 second


	for (auto& pair : vertexMap) {
		delete pair.second; // Free the memory
	}

	// Step 3:
	return maze;
}