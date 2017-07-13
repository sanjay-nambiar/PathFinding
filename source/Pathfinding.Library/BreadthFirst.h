#pragma once

#include "IPathFinder.h"

namespace Library
{
	class BreadthFirst final : public IPathFinder
	{
	public:
		std::deque<std::shared_ptr<Node>> FindPath(std::shared_ptr<Node> start, std::shared_ptr<Node> end, std::set<std::shared_ptr<Node>>& closedSet) override;
	};
}
