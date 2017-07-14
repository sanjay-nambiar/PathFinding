#pragma once

#include "AStar.h"

namespace Library
{
	class Dijkstra final : public AStar
	{
	public:
		Dijkstra(const PathFindingHelper::HeuristicFunction& heuristic = PathFindingHelper::ZeroHeuristic);
	};
}
