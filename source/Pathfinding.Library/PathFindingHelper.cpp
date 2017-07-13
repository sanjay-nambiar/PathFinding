#include "pch.h"
#include "PathFindingHelper.h"

using namespace std;

namespace Library
{
	float PathFindingHelper::ZeroHeuristic(const Point&, const Point&)
	{
		return 0.0f;
	}

	float PathFindingHelper::ManhattanDistance(const Point& from, const Point& to)
	{
		return static_cast<float>(abs(to.X() - from.X()) + abs(to.Y() - from.Y()));
	}
}
