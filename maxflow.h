#pragma once

#include <vector>
#include <string>
#include <fstream>
#include <queue>
#include <iostream>

/**
 * @class MaxFlow
 * @brief Class for computing the maximum flow in a flow network.
 */
class MaxFlow {
private:
	int numVertices;                          ///< Number of vertices in the graph
	std::vector<std::vector<int>> capacity;   ///< Capacity matrix
	std::vector<std::vector<int>> flow;       ///< Flow matrix

	/**
	 * @brief Breadth-First Search to find an augmenting path.
	 * @param source The source vertex.
	 * @param sink The sink vertex.
	 * @param parent Vector to store the path.
	 * @return True if an augmenting path is found, false otherwise.
	 */
	bool bfs(int source, int sink, std::vector<int>& parent);

public:
	/**
	 * @brief Constructor to initialize the flow network with a given number of vertices.
	 * @param vertices Number of vertices in the graph.
	 */
	explicit MaxFlow(int vertices);

	/**
	 * @brief Adds a directed edge with a specified capacity to the flow network.
	 * @param from The starting vertex of the edge.
	 * @param to The ending vertex of the edge.
	 * @param cap The capacity of the edge.
	 */
	void addEdge(int from, int to, int cap);

	/**
	 * @brief Computes the maximum flow from source to sink using the Edmonds-Karp algorithm.
	 * @param source The source vertex.
	 * @param sink The sink vertex.
	 * @return The value of the maximum flow.
	 */
	int calculateMaxFlow(int source, int sink);

	/**
	 * @brief Saves the flow network to a file in Graphviz format.
	 * @param filename The name of the file to save the graph.
	 */
	void saveGraphToFile(const std::string& filename) const;
};