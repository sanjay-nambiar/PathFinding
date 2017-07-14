#include "pch.h"
#include "DepthFirst.h"

using namespace std;

namespace Library
{
	deque<shared_ptr<Node>> DepthFirst::FindPath(shared_ptr<Node> start, shared_ptr<Node> end, set<shared_ptr<Node>>& closedSet)
	{
		stack<weak_ptr<Node>> frontier;
		frontier.push(start);

		while (!frontier.empty())
		{
			const auto& currentNode = frontier.top().lock();
			frontier.pop();
			closedSet.insert(currentNode);

			if (currentNode == end)
			{
				return PathFindingHelper::ExpandPath(currentNode);
			}

			for (auto& neighbor : currentNode->Neighbors())
			{
				const auto& neighborShared = neighbor.lock();
				if (closedSet.find(neighborShared) == closedSet.end())
				{
					neighborShared->SetParent(currentNode);
					frontier.push(neighbor);
				}
			}
		}
		return deque<shared_ptr<Node>>();
	}
}
