#pragma once

namespace Library
{
	class Node;

	class PathFindingHelper final
	{
	public:
		typedef std::function<float(const Node&, const Node&)> HeuristicFunction;

		static float ZeroHeuristic(const Node& from, const Node& to);
		static float ManhattanDistance(const Node& from, const Node& to);
	};
}
