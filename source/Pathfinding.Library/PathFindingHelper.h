#pragma once

#include <memory>
#include <deque>

namespace Library
{
	class Point;
	class Node;

	class PathFindingHelper final
	{
	public:
		typedef std::function<float(const Point&, const Point&)> HeuristicFunction;

		static float ZeroHeuristic(const Point& from, const Point& to);
		static float ManhattanDistance(const Point& from, const Point& to);
		static bool ManhattanDistanceSort(const std::weak_ptr<Node>& from, const std::weak_ptr<Node>& to);
		static std::deque<std::shared_ptr<Node>> ExpandPath(const std::shared_ptr<Node>& end);
	};
}

