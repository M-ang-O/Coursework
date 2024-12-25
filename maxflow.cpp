#include "maxflow.h"
#include <limits>
#include <algorithm>
#include <queue>

// Конструктор
MaxFlow::MaxFlow(int vertices) : numVertices(vertices), capacity(vertices, std::vector<int>(vertices, 0)), flow(vertices, std::vector<int>(vertices, 0)) {}

// Добавление дуги
void MaxFlow::addEdge(int from, int to, int cap) {
	capacity[from][to] += cap; // Учитываем многократное добавление ребер
}

// BFS для поиска увеличивающего пути
bool MaxFlow::bfs(int source, int sink, std::vector<int>& parent) {

	std::fill(parent.begin(), parent.end(), -1);

	std::queue<int> q;

	q.push(source);

	parent[source] = source;

	while (!q.empty()) {

		int current = q.front();

		q.pop();

		for (int next = 0; next < numVertices; ++next) {

			if (parent[next] == -1 && capacity[current][next] - flow[current][next] > 0) {

				parent[next] = current;

				if (next == sink) {

					return true;
				}

				q.push(next);

			}
		}
	}

	return false;
}

// Алгоритм Эдмондса-Карпа
int MaxFlow::calculateMaxFlow(int source, int sink) {

	int maxFlow = 0;

	std::vector<int> parent(numVertices);

	while (bfs(source, sink, parent)) {

		int pathFlow = std::numeric_limits<int>::max();

		for (int v = sink; v != source; v = parent[v]) {

			int u = parent[v];

			pathFlow = std::min(pathFlow, capacity[u][v] - flow[u][v]);
		}

		for (int v = sink; v != source; v = parent[v]) {

			int u = parent[v];

			flow[u][v] += pathFlow;
			flow[v][u] -= pathFlow;
		}

		maxFlow += pathFlow;
	}

	return maxFlow;
}

// Сохранение графа в формате Graphviz
void MaxFlow::saveGraphToFile(const std::string& filename) const {

	std::ofstream file(filename);

	if (!file.is_open()) {

		throw std::runtime_error("Не удалось открыть файл для записи: " + filename);
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