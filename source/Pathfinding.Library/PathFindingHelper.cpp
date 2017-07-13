#include "pch.h"
#include "PathFindingHelper.h"

using namespace std;

namespace Library
{
	float PathFindingHelper::ZeroHeuristic(const Node&, const Node&)
	{
		return 0.0f;
	}

	float PathFindingHelper::ManhattanDistance(const Node& from, const Node& to)
	{
		from;
		to;
		return 0.0f;
	}
}
