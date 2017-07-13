#include "pch.h"
#include "BreadthFirst.h"

using namespace std;

namespace Library
{
	deque<shared_ptr<Node>> BreadthFirst::FindPath(shared_ptr<Node> start, shared_ptr<Node> end, set<shared_ptr<Node>>&)
	{
		unordered_set<uint64_t> visited;
		deque<weak_ptr<Node>> frontier;
		frontier.push_back(start);

		deque<shared_ptr<Node>> path;
		while (!frontier.empty())
		{
			const auto& currentNode = frontier.front().lock();
			frontier.pop_front();
			visited.insert(reinterpret_cast<uint64_t>(currentNode.get()));

			if (currentNode == end)
			{
				path.push_back(currentNode);
				break;
			}

			for (auto& neighbor : currentNode->Neighbors())
			{
				const auto& neighborShared = neighbor.lock();
				if (visited.find(reinterpret_cast<uint64_t>(neighborShared.get())) == visited.end())
				{
					neighborShared->SetParent(currentNode);
					frontier.push_back(neighbor);
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
