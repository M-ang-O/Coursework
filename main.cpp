#include <iostream>
#include <chrono>
#include <fstream>
#include <cstdlib>
#include "maxflow.h"
#include <sciplot/sciplot.hpp>

using namespace sciplot;

/**
 * @brief Utility function to convert a std::vector to a sciplot::Vec.
 * @tparam T Type of elements in the input vector.
 * @param v Input vector.
 * @return sciplot::Vec containing the same elements as the input vector.
 */
template <typename T>
Vec toVec(const std::vector<T>& v) {

	Vec result(v.size());

	for (size_t i = 0; i < v.size(); ++i) {

		result[i] = v[i];
	}

	return result;
}

/**
 * @brief Generates a random flow network graph.
 * @param graph Reference to a MaxFlow object where the graph will be stored.
 * @param vertices Number of vertices in the graph.
 * @param density Probability (in percentage) of an edge existing between any two vertices.
 */
void generateGraph(MaxFlow& graph, int vertices, int density) {

	srand(time(nullptr)); // Seed for random number generation

	for (int i = 0; i < vertices; ++i) {

		for (int j = 0; j < vertices; ++j) {

			if (i != j && rand() % 100 < density) { // density - probability of an edge in percentage

				int capacity = rand() % 20 + 1; // Capacity between 1 and 20

				graph.addEdge(i, j, capacity);
			}

		}

	}

}

/**
 * @brief Measures the execution time of the max flow calculation for a randomly generated graph.
 * @param vertices Number of vertices in the graph.
 * @param density Probability (in percentage) of an edge existing between any two vertices.
 * @param filename Name of the file to save the graph in Graphviz format.
 * @return Execution time in seconds.
 */
double measureExecutionTime(int vertices, int density, const std::string& filename) {

	MaxFlow graph(vertices);

	generateGraph(graph, vertices, density);

	int source = 0; // Source vertex

	int sink = vertices - 1; // Sink vertex

	std::cout << "Calculating maximum flow for a graph with " << vertices << " vertices...\n";

	auto start = std::chrono::high_resolution_clock::now();
	int maxFlow = graph.calculateMaxFlow(source, sink);
	auto end = std::chrono::high_resolution_clock::now();

	std::chrono::duration<double> elapsed = end - start;
	std::cout << "Maximum flow: " << maxFlow << "\n";
	std::cout << "Execution time: " << elapsed.count() << " seconds\n";

	graph.saveGraphToFile(filename);

	std::cout << "Graph saved to file: " << filename << "\n";

	return elapsed.count();
}

/**
 * @brief Saves a vector of data pairs to a file.
 * @param filename Name of the file to save the data.
 * @param data Vector of pairs, where each pair consists of an integer and a double.
 * @throws std::runtime_error If the file cannot be opened for writing.
 */
void saveDataToFile(const std::string& filename, const std::vector<std::pair<int, double>>& data) {

	std::ofstream file(filename);

	if (!file.is_open()) {

		throw std::runtime_error("Failed to open file for writing: " + filename);
	}
	for (const auto& pair : data) {

		file << pair.first << " " << pair.second << "\n";
	}
	file.close();
}

/**
 * @brief Plots the execution time against the number of vertices using sciplot.
 * @param vertices Vector containing the number of vertices.
 * @param times Vector containing the execution times corresponding to the number of vertices.
 */
void plotTimeVsVertices(const std::vector<int>& vertices, const std::vector<double>& times) {

	Vec x = toVec(vertices);
	Vec y = toVec(times);

	// First plot
	Plot2D plot1;

	plot1.palette("paired");
	plot1.xlabel("Number of Vertices");
	plot1.ylabel("Execution Time (seconds)");
	plot1.drawCurve(x, y).label("Execution Time").lineWidth(4);

	// Second plot (example of another dependency function)
	Plot2D plot2;

	plot2.palette("paired");
	plot2.xlabel("Number of Vertices");
	plot2.ylabel("Execution Time (seconds)");
	plot2.drawCurve(x, y * 0.5).label("Scaled Execution Time").lineWidth(2);

	// Combine both plots into a figure
	Figure figure = { {plot1, plot2} };

	// Create a canvas for output
	Canvas canvas = { {figure} };

	canvas.defaultPalette("set1");
	canvas.size(1280, 720);

	// Save the plot
	canvas.save("time_vs_vertices.png");

	std::cout << "Plots saved to file: time_vs_vertices.png\n";
}

/**
 * @brief Main function demonstrating the max flow calculation and plotting execution times.
 * @return Exit status code.
 */
int main() {

	setlocale(LC_ALL, "ru");

	// Example graph with 6 vertices
	MaxFlow graph(6);

	graph.addEdge(0, 1, 16);
	graph.addEdge(0, 2, 13);
	graph.addEdge(1, 2, 10);
	graph.addEdge(1, 3, 12);
	graph.addEdge(2, 1, 4);
	graph.addEdge(2, 4, 14);
	graph.addEdge(3, 2, 9);
	graph.addEdge(3, 5, 20);
	graph.addEdge(4, 3, 7);
	graph.addEdge(4, 5, 4);

	int source = 0, sink = 5;

	std::cout << "Maximum flow: " << graph.calculateMaxFlow(source, sink) << std::endl;

	try {

		std::vector<int> vertices = { 10, 20 }; // Number of vertices for two graphs
		std::vector<double> times; // Execution times for each graph

		// First graph
		times.push_back(measureExecutionTime(10, 30, "graph1.dot"));

		// Second graph (larger)
		times.push_back(measureExecutionTime(20, 30, "graph2.dot"));

		// Plotting the graph
		plotTimeVsVertices(vertices, times);

	}
	catch (const std::exception& ex) {
		std::cerr << "Error: " << ex.what() << "\n";
	}

	return 0;
}