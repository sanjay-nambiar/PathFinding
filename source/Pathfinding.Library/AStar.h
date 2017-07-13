#pragma once

#include "IPathFinder.h"
#include <functional>
#include "PathFindingHelper.h"

namespace Library
{
	class AStar final : public IPathFinder
	{
	public:
		AStar(const PathFindingHelper::HeuristicFunction& heuristic = PathFindingHelper::ManhattanDistance);

		std::deque<std::shared_ptr<Node>> FindPath(std::shared_ptr<Node> start, std::shared_ptr<Node> end, std::set<std::shared_ptr<Node>>& closedSet) override;

	private:
		const PathFindingHelper::HeuristicFunction mHeuristic;
	};
}
