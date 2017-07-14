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
		unordered_set<shared_ptr<Node>> openSetLookup;
		struct ManhattanHeuristic
		{
			bool operator()(const shared_ptr<Node>& lhs, const shared_ptr<Node>& rhs)
			{
				return lhs->TotalCost() < rhs->TotalCost();
			}
		};
		priority_queue<shared_ptr<Node>, vector<shared_ptr<Node>>, ManhattanHeuristic> openSet;
		openSet.push(start);
		openSetLookup.insert(start);
		start->SetPathCost(0.0f);
		closedSet.clear();

		while (!openSet.empty())
		{
			const shared_ptr<Node>& currentNode = openSet.top();
			if (currentNode == end)
			{
				return PathFindingHelper::ExpandPath(currentNode);
			}

			openSet.pop();
			openSetLookup.erase(currentNode);
			closedSet.insert(currentNode);
			for (auto& neighborWeak : currentNode->Neighbors())
			{
				const auto& neighbor = neighborWeak.lock();
				if (closedSet.find(neighbor) == closedSet.end())
				{
					float pathCost = currentNode->PathCost() + 1;
					if (openSetLookup.find(neighbor) != openSetLookup.end())
					{
						if (pathCost < neighbor->PathCost())
						{
							neighbor->SetParent(currentNode);
							neighbor->SetPathCost(pathCost);
						}
					}
					else
					{
						neighbor->SetHeuristic(mHeuristic(start->Location(), neighbor->Location()));
						neighbor->SetParent(currentNode);
						neighbor->SetPathCost(pathCost);
						openSet.push(neighbor);
						openSetLookup.insert(neighbor);
					}
				}
			}
		}
		return deque<shared_ptr<Node>>();
	}
}
