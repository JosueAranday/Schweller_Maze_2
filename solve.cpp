#include <iostream>
#include <vector>
#include <string>
#include <unordered_set>
#include <queue>
#include <unordered_map>
#include "solve.h"
#include "vertex.h"
#include "minpriorityqueue.h"

// Trying to visualize the maze solving process
#include <thread>   // For std::this_thread::sleep_for
#include <chrono>   // For std::chrono::seconds and milliseconds
#include <cstdlib>  // For system()

using namespace std;

// Function to clear the console screen
void clearScreen() {
#ifdef _WIN32
	system("cls");
#else
	system("clear");
#endif
}

string solve(string maze) {
	unordered_map<int, Vertex*> vertexMap;
	unordered_map< char, int > portalMap;	// Keep track of portals, number and counter 
	size_t rowLength = maze.find('\n') + 1;
	int counter = 0;
	int xCoor = 0;
	int yCoor = 0;
	int xWidth = rowLength - 1;
	int yHeight = 0;
	int stdCost = 1;
	int portalCost;

	while (counter < maze.size()) {		// Runs in O(s) time
		// Create vertices for each open space in the maze
		portalCost = 1; // Reset portal cost
		char curr = maze[counter];
		if (curr == ' ') {
			vertexMap[counter] = new Vertex(yCoor, xCoor);
			if (xCoor > 0){
				if (maze[counter - 1] == ' ') {		// Left neighbor
					vertexMap[counter]->neighs.push_back({vertexMap[counter - 1], stdCost });
					vertexMap[counter - 1]->neighs.push_back({vertexMap[counter], stdCost });
				}
				else if (maze[counter - 1] >= '0' && maze[counter - 1] <= '9') {
					// Left neighbor is a number
					vertexMap[counter]->neighs.push_back({ vertexMap[counter - 1], stdCost });
					vertexMap[counter - 1]->neighs.push_back({ vertexMap[counter], stdCost });
				}
			}
			if (yCoor > 0) {
				if (maze[counter - rowLength] == ' ') {		// Top neighbor
					vertexMap[counter]->neighs.push_back({ vertexMap[counter - rowLength], stdCost });
					vertexMap[counter - rowLength]->neighs.push_back({ vertexMap[counter], stdCost });
				}
				else if (maze[counter - rowLength] >= '0' && maze[counter - rowLength] <= '9') {
					// Top neighbor is a number
					vertexMap[counter]->neighs.push_back({ vertexMap[counter - rowLength], stdCost });
					vertexMap[counter - rowLength]->neighs.push_back({ vertexMap[counter], stdCost });
				}
			}
			xCoor++;
		}
		else if (curr >= '0' && curr <= '9') {
			vertexMap[counter] = new Vertex(yCoor, xCoor);
			portalCost = curr - '0'; // Portal cost based on number
			if (xCoor > 0) {
				if (maze[counter - 1] == ' ') {		// Left neighbor
					vertexMap[counter]->neighs.push_back({ vertexMap[counter - 1], stdCost });
					vertexMap[counter - 1]->neighs.push_back({ vertexMap[counter], stdCost });
				}
				else if (maze[counter - 1] >= '0' && maze[counter - 1] <= '9') {
					// Left neighbor is a number
					vertexMap[counter]->neighs.push_back({ vertexMap[counter - 1], stdCost });
					vertexMap[counter - 1]->neighs.push_back({ vertexMap[counter], stdCost });
				}
			}
			if (yCoor > 0) {
				if (maze[counter - rowLength] == ' ') {		// Top neighbor
					vertexMap[counter]->neighs.push_back({ vertexMap[counter - rowLength], stdCost });
					vertexMap[counter - rowLength]->neighs.push_back({ vertexMap[counter], stdCost });
				}
				else if (maze[counter - rowLength] >= '0' && maze[counter - rowLength] <= '9') {
					// Top neighbor is a number
					vertexMap[counter]->neighs.push_back({ vertexMap[counter - rowLength], stdCost });
					vertexMap[counter - rowLength]->neighs.push_back({ vertexMap[counter], stdCost });
				}
			}
			if (portalMap.find(curr) != portalMap.end()) {
				// Connect portals
				int otherPortalIndex = portalMap[curr];	// Get the index of the other portal
				vertexMap[counter]->neighs.push_back({ vertexMap[otherPortalIndex], portalCost });
				vertexMap[otherPortalIndex]->neighs.push_back({ vertexMap[counter], portalCost });
			}
			portalMap[curr] = counter; // Record the position of this portal only after checkin
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

	Vertex* startVertex = nullptr;
	Vertex* endVertex = nullptr;
	unordered_map<Vertex*, Vertex*> breadCrumbs;
	unordered_set<Vertex*> marked;
	MinPriorityQueue<Vertex*> PQ;

	// ********************************************************
	// **************** Dijkstra's Algorithm ******************
	// ********************************************************
	
	// Step 0, find start and end vertices:
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

	const int infCost = 100000;
	unordered_map<Vertex*, int> dist;
	marked.insert(startVertex);

	// Step 0.1, Initialize all distances to infinity and insert into PQ:
	for (const auto& pair : vertexMap) {		// Runs in O(V) time
		dist[pair.second] = infCost;
		PQ.push(pair.second, infCost);
	}
	dist[startVertex] = 0; // Distance to start vertex is 0
	PQ.decrease_key(startVertex, 0);

	// Step 1: Explore the graph using a priority queue

	while (!PQ.empty()) {		// Runs in O(V log V) time
		Vertex* x = PQ.front();
		PQ.pop();
		marked.insert(x);
		for (const auto& y : x->neighs) {
			Vertex* neighbor = y.first;
			int weight = y.second;
			// Relaxation step
			if (marked.find(neighbor) == marked.end()){
				if (dist[x] + weight < dist[neighbor]){
					// Update weight
					dist[neighbor] = dist[x] + weight;
					PQ.decrease_key(neighbor, dist[x] + weight);
					breadCrumbs[neighbor] = x;
				}
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
		//std::this_thread::sleep_for(std::chrono::microseconds(100)); // Wait 0.1 second
	}

	// Mark the start vertex as well
	int startIndex = startVertex->row * rowLength + startVertex->col;
	maze[startIndex] = 'o';
	// Optional: Print out the solved maze at each step
	//clearScreen();
	//cout << maze << endl;
	//std::this_thread::sleep_for(std::chrono::microseconds(1000)); // Wait 1 second


	for (auto& pair : vertexMap) {
		delete pair.second; // Free the memory
	}

	// Step 3:
	return maze;
}