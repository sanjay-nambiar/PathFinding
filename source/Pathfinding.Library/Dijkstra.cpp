#include "pch.h"
#include "Dijkstra.h"

using namespace std;

namespace Library
{
	Dijkstra::Dijkstra(const PathFindingHelper::HeuristicFunction& heuristic) :
		AStar(heuristic)
	{
	}
}
