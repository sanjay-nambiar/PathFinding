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

	bool PathFindingHelper::ManhattanDistanceSort(const weak_ptr<Node>& from, const weak_ptr<Node>& to)
	{
		const auto& fromLocation = from.lock()->Location();
		const auto& toLocation = to.lock()->Location();
		return (fromLocation.X() + fromLocation.Y()) < (toLocation.X() + toLocation.Y());
	}
}
