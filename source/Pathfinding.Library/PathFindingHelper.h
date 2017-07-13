#pragma once

namespace Library
{
	class Point;

	class PathFindingHelper final
	{
	public:
		typedef std::function<float(const Point&, const Point&)> HeuristicFunction;

		static float ZeroHeuristic(const Point& from, const Point& to);
		static float ManhattanDistance(const Point& from, const Point& to);
	};
}
