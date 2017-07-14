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
				return lhs->Heuristic() < rhs->Heuristic();
			}
		};
		priority_queue<shared_ptr<Node>, vector<shared_ptr<Node>>, ManhattanHeuristic> openSet;
		openSet.push(start);
		openSetLookup.insert(start);
		closedSet.clear();
		
		deque<shared_ptr<Node>> path;
		while (!openSet.empty())
		{
			const auto& currentNode = openSet.top();
			if (currentNode == end)
			{
				path.push_back(currentNode);
				break;
			}

			openSet.pop();
			openSetLookup.erase(currentNode);
			closedSet.insert(currentNode);
			for (auto& neighbor : currentNode->Neighbors())
			{
				const auto& neighborShared = neighbor.lock();
				if (closedSet.find(neighborShared) == closedSet.end())
				{
					neighborShared->SetParent(currentNode);
					if (openSetLookup.find(neighborShared) == openSetLookup.end())
					{
						neighborShared->SetHeuristic(PathFindingHelper::ManhattanDistance(start->Location(), neighborShared->Location()));
						openSet.push(neighborShared);
						openSetLookup.insert(neighborShared);
					}
				}
			}
		}

		if (!path.empty())
		{
			shared_ptr<Node> parent = path.front()->Parent().lock();
			while (parent)
			{
				path.push_front(parent);
				parent = parent->Parent().lock();
			}
		}
		return path;
	}
}
