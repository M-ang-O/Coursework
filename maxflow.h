#pragma once

#include <vector>
#include <string>
#include <fstream>
#include <queue>
#include <iostream>

/**
 * @class MaxFlow
 * @brief Класс для вычисления максимального потока в транспортной сети.
 */
class MaxFlow {
private:

	int numVertices;                          // Количество вершин
	std::vector<std::vector<int>> capacity;  // Матрица пропускных способностей
	std::vector<std::vector<int>> flow;      // Матрица текущего потока

	// BFS для поиска увеличивающего пути
	bool bfs(int source, int sink, std::vector<int>& parent);

public:
	// Конструктор
	explicit MaxFlow(int vertices);

	// Добавление дуги
	void addEdge(int from, int to, int cap);

	// Алгоритм Эдмондса-Карпа для нахождения максимального потока
	int calculateMaxFlow(int source, int sink);

	// Сохранение графа в формате Graphviz
	void saveGraphToFile(const std::string& filename) const;
};