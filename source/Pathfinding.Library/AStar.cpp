#include "pch.h"
#include "AStar.h"

using namespace std;

namespace Library
{
	AStar::AStar(const PathFindingHelper::HeuristicFunction& heuristic) :
		mHeuristic(heuristic)
	{
	}

	deque<shared_ptr<Node>> AStar::FindPath(shared_ptr<Node> start, shared_ptr<Node> end, set<shared_ptr<Node>>& closedSet)
	{
		start;
		end;
		closedSet;
		return deque<shared_ptr<Node>>();
	}
}
