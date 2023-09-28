#pragma once

#include "Edge.hpp"

#include <cmath>
#include <ctime>
#include <random>

#include <map>
#include <string>
#include <vector>
#include <queue>

using TGraph = std::map<std::string, std::vector<Graph::Edge>>;
using TPath = std::map<std::string, std::string>;

namespace Algorithms {

	namespace Graph {

		class AntColony {

			struct EdgeAnt
			{
				std::string to;
				uint32_t weight;

				float qPheromones;
				float attraction;

				EdgeAnt(const std::string to, const uint32_t weight, const float qPheromones, const float attraction)
					:to(to), weight(weight), qPheromones(qPheromones), attraction(attraction) {}
			};

			using TGraphAnt = std::map<std::string, std::vector<EdgeAnt>>;

			struct intervalInformation {
				std::string vertex;
				uint32_t weight;
				float k;
			};

		public:
			// parameters for set-up algorithm

			static constexpr int a = 1;
			static constexpr int b = 1;
			static constexpr float p = 0.05;
			static constexpr float pheromonesInit = 1.0;

			// algorithm implement

			static std::pair<uint32_t, std::vector<std::string>> foo(const TGraph& graph, const uint32_t qIterations) {

				if (static bool isInitRand(false); !isInitRand) {

					std::srand(std::time(nullptr));
					isInitRand = true;
				}

				uint32_t bestDistance{ 0xFFFFFFFF };
				TPath bestPath;
				std::string pathStart;

				TGraphAnt graphAnt;
				std::queue<std::string> vertexesQueue;

				// adaptive graph to graph with ant
				for (auto itVertex = graph.begin(); itVertex != graph.end(); ++itVertex) {

					vertexesQueue.push(itVertex->first);
					for (auto itNode = itVertex->second.begin(); itNode != itVertex->second.end(); ++itNode) {

						graphAnt[itVertex->first].push_back({ itNode->to, itNode->weight, pheromonesInit, static_cast<float>(1) / itNode->weight });
					}
				}

				size_t qVertexes = vertexesQueue.size();
				
				uint32_t iterationCounter{ 0 };
				while (iterationCounter++ < qIterations) {

					std::queue<std::string> vertexesQueueCopy = vertexesQueue;

					while (!vertexesQueueCopy.empty()) {

						std::string from = vertexesQueueCopy.front();
						TPath path;
						uint32_t distance{ 0 };

						std::string current = from;
						do {	// строим путь

							float sum{ 0.0 };
							std::vector<intervalInformation> vertexProbabilities;
							for (auto it = graphAnt[current].begin(); it != graphAnt[current].end(); ++it) {

								if (!path.contains(it->to)) {

									float temp = std::powf(it->qPheromones, a) * std::powf(it->attraction, b);
									vertexProbabilities.push_back({ it->to, it->weight, temp + sum });
									sum += temp;
								}
							}

							float probability = rand() / (RAND_MAX - 1.);

							for (auto it = vertexProbabilities.begin(); it != vertexProbabilities.end(); ++it) {

								if (probability <= (it->k / sum)) {

									distance += it->weight;
									path[current] = it->vertex;
									current = it->vertex;
									break;
								}
							}
							if (vertexProbabilities.empty())
								current = "";

						} while (current != "");

						if (path.size() == qVertexes - 1) {

							for (auto itVertex = graphAnt.begin(); itVertex != graphAnt.end(); ++itVertex) {

								for (auto itNode = itVertex->second.begin(); itNode != itVertex->second.end(); ++itNode) {

									itNode->qPheromones = itNode->qPheromones * (1 - p);

									if (path[itVertex->first] == itNode->to) {

										itNode->qPheromones += (static_cast<float>(1) / distance);
									}
								}
							}

							if (distance < bestDistance) {

								bestDistance = distance;
								bestPath = std::move(path);
								pathStart = from;
								path = {};
							}
						}

						vertexesQueueCopy.pop();
					}
				}

				std::vector<std::string> res;
				std::string it = pathStart;

				while (res.push_back(it), bestPath.contains(it) && bestPath[it] != "") {

					it = bestPath[it];
				}

				return { bestDistance, {res.begin(), res.end()} };

			}

		private:

		};

	}

}