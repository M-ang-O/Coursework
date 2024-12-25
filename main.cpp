#include <iostream>
#include <chrono>
#include <fstream>
#include <cstdlib>
#include "maxflow.h"
#include <sciplot/sciplot.hpp>

using namespace sciplot;

// Утилитарная функция
template <typename T> Vec toVec(const std::vector<T>& v) {

	Vec result(v.size());

	for (size_t i = 0; i < v.size(); ++i) {

		result[i] = v[i];
	}

	return result;
}

void generateGraph(MaxFlow& graph, int vertices, int density) {

	srand(time(nullptr)); // Генерация случайных чисел

	for (int i = 0; i < vertices; ++i) {

		for (int j = 0; j < vertices; ++j) {

			if (i != j && rand() % 100 < density) { // density - вероятность ребра в процентах

				int capacity = rand() % 20 + 1; // Пропускная способность от 1 до 20

				graph.addEdge(i, j, capacity);
			}

		}

	}

}

double measureExecutionTime(int vertices, int density, const std::string& filename) {

	MaxFlow graph(vertices);

	generateGraph(graph, vertices, density);

	int source = 0; // Источник
	int sink = vertices - 1; // Сток

	std::cout << "Вычисляем максимальный поток для графа с " << vertices << " вершинами...\n";

	auto start = std::chrono::high_resolution_clock::now();
	int maxFlow = graph.calculateMaxFlow(source, sink);
	auto end = std::chrono::high_resolution_clock::now();

	std::chrono::duration<double> elapsed = end - start;
	std::cout << "Максимальный поток: " << maxFlow << "\n";
	std::cout << "Время выполнения: " << elapsed.count() << " секунд\n";

	graph.saveGraphToFile(filename);

	std::cout << "Граф сохранён в файл: " << filename << "\n";

	return elapsed.count();
}

void saveDataToFile(const std::string& filename, const std::vector<std::pair<int, double>>& data) {

	std::ofstream file(filename);

	if (!file.is_open()) {

		throw std::runtime_error("Не удалось открыть файл для записи: " + filename);
	}

	for (const auto& pair : data) { // Используем auto для работы с элементами вектора

		file << pair.first << " " << pair.second << "\n"; // Запись данных: first - вершины, second - время
	}

	file.close();
}

// Построение графика зависимости времени от количества вершин
void plotTimeVsVertices(const std::vector<int>& vertices, const std::vector<double>& times) {

	// Создаём вектор x из количества вершин
	// Создаём вектор y из времени выполнения

	Vec x = toVec(vertices);
	Vec y = toVec(times);

	// Первый график
	Plot2D plot1;

	plot1.palette("paired");
	plot1.xlabel("Number of Vertices");
	plot1.ylabel("Execution Time (seconds)");
	plot1.drawCurve(x, y).label("Execution Time").lineWidth(4);

	// Второй график (пример другой функции зависимости)
	Plot2D plot2;

	plot2.palette("paired");
	plot2.palette("paired");
	plot2.xlabel("Number of Vertices");
	plot2.ylabel("Execution Time (seconds)");

	plot2.drawCurve(x, y * 0.5).label("Scaled Execution Time").lineWidth(2);

	// Объединяем оба графика в фигуру
	Figure figure = { {plot1, plot2} };

	// Создаём холст для вывода
	Canvas canvas = { {figure} };

	canvas.defaultPalette("set1");

	canvas.size(1280, 720);

	// Сохраняем график
	canvas.save("time_vs_vertices.png");

	std::cout << "Графики сохранены в файл: time_vs_vertices.png\n";

}

int main() {

	setlocale(LC_ALL, "ru");

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

	std::cout << "Максимальный поток: " << graph.calculateMaxFlow(source, sink) << std::endl;

	try {

		std::vector<int> vertices = { 10, 100 };        // Количество вершин для двух графов
		std::vector<double> times;                   // Время выполнения для каждого графа

		// Первый граф
		times.push_back(measureExecutionTime(10, 30, "graph1.dot"));

		// Второй граф (увеличенный)
		times.push_back(measureExecutionTime(100, 30, "graph2.dot"));

		// Построение графика
		plotTimeVsVertices(vertices, times);
	}
	catch (const std::exception& ex) {
		std::cerr << "Ошибка: " << ex.what() << "\n";
	}

	return 0;
}
