#include "pch.h"
#include "GreedyBestFirst.h"

using namespace std;

namespace Library
{
	deque<shared_ptr<Node>> GreedyBestFirst::FindPath(shared_ptr<Node> start, shared_ptr<Node> end, set<shared_ptr<Node>>& closedSet)
	{
		unordered_set<shared_ptr<Node>> openSetLookup;
		struct ManhattanHeuristic
		{
			bool operator()(const shared_ptr<Node>& lhs, const shared_ptr<Node>& rhs)
			{
				return lhs->Heuristic() > rhs->Heuristic();
			}
		};
		priority_queue<shared_ptr<Node>, vector<shared_ptr<Node>>, ManhattanHeuristic> openSet;
		openSet.push(start);
		openSetLookup.insert(start);
		start->SetHeuristic(PathFindingHelper::ManhattanDistance(start->Location(), end->Location()));
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
					neighbor->SetParent(currentNode);
					if (openSetLookup.find(neighbor) == openSetLookup.end())
					{
						neighbor->SetHeuristic(PathFindingHelper::ManhattanDistance(neighbor->Location(), end->Location()));
						openSet.push(neighbor);
						openSetLookup.insert(neighbor);
					}					
				}
			}
		}
		return deque<shared_ptr<Node>>();
	}
}
