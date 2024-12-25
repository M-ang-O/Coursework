#pragma once

#include <vector>
#include <string>
#include <fstream>
#include <queue>
#include <iostream>

/**
 * @class MaxFlow
 * @brief ����� ��� ���������� ������������� ������ � ������������ ����.
 */
class MaxFlow {
private:

	int numVertices;                          // ���������� ������
	std::vector<std::vector<int>> capacity;  // ������� ���������� ������������
	std::vector<std::vector<int>> flow;      // ������� �������� ������

	// BFS ��� ������ �������������� ����
	bool bfs(int source, int sink, std::vector<int>& parent);

public:
	// �����������
	explicit MaxFlow(int vertices);

	// ���������� ����
	void addEdge(int from, int to, int cap);

	// �������� ��������-����� ��� ���������� ������������� ������
	int calculateMaxFlow(int source, int sink);

	// ���������� ����� � ������� Graphviz
	void saveGraphToFile(const std::string& filename) const;
};