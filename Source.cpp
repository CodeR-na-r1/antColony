#include "GraphAlgorithms/Edge.hpp"
#include "GraphAlgorithms/AntColony.hpp"

#include <iostream>
#include <fstream>

#include <map>
#include <vector>
#include <string>

using namespace std;

using TGraph = std::map<std::string, std::vector<Graph::Edge>>;

int main() {

	TGraph graph;

	{	// get graph from file

		ifstream fileIn("GraphData1.txt");

		if (!fileIn.is_open()) {
			cout << "File no open!" << endl;
			return -1;
		}

		std::string from, to;
		uint32_t weight;

		for (; fileIn >> from >> to >> weight;) {
			graph[from].push_back({ to, weight });
		}

		fileIn.close();
	}

	auto [distance, path] = Algorithms::Graph::AntColony::foo(graph, 1000);

	cout << "Distance -> " << distance << endl;

	cout << "Path { ";
	for (const auto& elem : path) {

		cout << elem << " ";
	}
	cout << "}" << endl;

	return 0;

}