#include "maxflow.h"
#include <limits>
#include <algorithm>
#include <queue>
#include <fstream>
#include <stdexcept>

/**
 * @file maxflow.cpp
 * @brief Implementation of the MaxFlow class for computing maximum flow in a flow network.
 */

 /**
  * @brief Constructs a graph with the specified number of vertices.
  * @param vertices Number of vertices in the graph.
  */
MaxFlow::MaxFlow(int vertices) : numVertices(vertices), capacity(vertices, std::vector<int>(vertices, 0)), flow(vertices, std::vector<int>(vertices, 0)) {}

/**
 * @brief Adds an edge to the flow network.
 * @param from The starting vertex of the edge.
 * @param to The ending vertex of the edge.
 * @param cap The capacity of the edge.
 */
void MaxFlow::addEdge(int from, int to, int cap) {
	capacity[from][to] += cap; // Handles multiple edges between the same nodes
}

/**
 * @brief Performs a Breadth-First Search to find an augmenting path.
 * @param source The source vertex.
 * @param sink The sink vertex.
 * @param parent Vector to store the path.
 * @return True if an augmenting path is found, false otherwise.
 */
bool MaxFlow::bfs(int source, int sink, std::vector<int>& parent) {

	std::fill(parent.begin(), parent.end(), -1);

	std::queue<int> q;

	q.push(source);

	parent[source] = source;

	while (!q.empty()) {

		int current = q.front();

		q.pop();

		for (int next = 0; next < numVertices; ++next) {

			// Check for residual capacity and unvisited nodes
			if (parent[next] == -1 && capacity[current][next] - flow[current][next] > 0) {

				parent[next] = current;

				// If sink is reached, return true
				if (next == sink) {

					return true;
				}

				q.push(next);
			}

		}

	}

	return false; // No augmenting path found
}

/**
 * @brief Calculates the maximum flow between the source and sink vertices using the Edmonds-Karp algorithm.
 * @param source The source vertex.
 * @param sink The sink vertex.
 * @return The maximum flow value.
 */
int MaxFlow::calculateMaxFlow(int source, int sink) {

	int maxFlow = 0;

	std::vector<int> parent(numVertices);

	// While there is an augmenting path
	while (bfs(source, sink, parent)) {

		// Find the minimum residual capacity in the path
		int pathFlow = std::numeric_limits<int>::max();

		for (int v = sink; v != source; v = parent[v]) {

			int u = parent[v];

			pathFlow = std::min(pathFlow, capacity[u][v] - flow[u][v]);
		}

		// Update residual capacities along the path
		for (int v = sink; v != source; v = parent[v]) {

			int u = parent[v];

			flow[u][v] += pathFlow;
			flow[v][u] -= pathFlow;
		}

		// Add the flow of this path to the total flow
		maxFlow += pathFlow;
	}
	return maxFlow;
}

/**
 * @brief Saves the graph in Graphviz format to a file.
 * @param filename The name of the file to save the graph.
 * @throws std::runtime_error If the file cannot be opened for writing.
 */
void MaxFlow::saveGraphToFile(const std::string& filename) const {

	std::ofstream file(filename);

	if (!file.is_open()) {

		throw std::runtime_error("Failed to open file for writing: " + filename);
	}

	file << "digraph FlowNetwork {\n";

	for (int u = 0; u < numVertices; ++u) {

		for (int v = 0; v < numVertices; ++v) {

			if (capacity[u][v] > 0) {

				file << "    " << u << " -> " << v
					<< " [label=\"" << flow[u][v] << "/" << capacity[u][v] << "\"];\n";
			}

		}

	}

	file << "}\n";
	file.close();
}