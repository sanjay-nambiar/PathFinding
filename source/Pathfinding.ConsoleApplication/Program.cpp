#include "pch.h"

using namespace std;
using namespace Library;

void PrintGrid(const Graph& graph, const deque<shared_ptr<Node>>& path);

int main(int argc, char* argv[])
{
#if defined(DEBUG) | defined(_DEBUG)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	if (argc != 2)
	{
		cout << "Usage: " << argv[0] << " <gridfile>" << endl;
		return 1;
	}

	Graph graph = GridHelper::LoadGridFromFile(argv[1]);

	static const unordered_map<PathFindingType, shared_ptr<IPathFinder>> algorithms =  {
		{ PathFindingType::BreadthFirst, make_shared<BreadthFirst>()},
		{ PathFindingType::DepthFirst, make_shared<DepthFirst>() },
		{ PathFindingType::GreedyBestFirst, make_shared<GreedyBestFirst>() },
		{ PathFindingType::Dijkstra, make_shared<Dijkstra>() },
		{ PathFindingType::AStar, make_shared<AStar>() }
	};

	Point start(0, 0), end(2, 2);
	const auto path = algorithms.at(PathFindingType::BreadthFirst)->FindPath(graph.At(start), graph.At(end));
	PrintGrid(graph, path);

	cin.get();
	return 0;
}


void PrintGrid(const Graph& graph, const deque<shared_ptr<Node>>& path)
{
	static char grid[100][100];
	static int32_t gridWidth = 0, gridHeight = 0;

	enum class NodeGlyph
	{
		Normal,
		Wall,
		Start,
		End,
		Visited
	};

	static const unordered_map<NodeType, NodeGlyph> GridTypeToVisualType = {
		{ NodeType::Normal, NodeGlyph::Normal },
		{ NodeType::Wall, NodeGlyph::Wall }
	};

	static const unordered_map<NodeGlyph, char> GridTypeVisuals = {
		{ NodeGlyph::Normal, '-' },
		{ NodeGlyph::Wall, '|' },
		{ NodeGlyph::Start, 'S' },
		{ NodeGlyph::End, 'E' },
		{ NodeGlyph::Visited, 'X' }
	};

	for (const auto& entry : graph.Nodes())
	{
		const auto& point = entry.first;
		const auto& node = entry.second;

		grid[point.Y()][point.X()] = GridTypeVisuals.at(GridTypeToVisualType.at(node->Type()));
		
		if (gridWidth < point.X())
		{
			gridWidth = point.X();
		}

		if (gridHeight < point.Y())
		{
			gridHeight = point.Y();
		}

		if (!path.empty())
		{
			if (node == path.front())
			{
				grid[point.Y()][point.X()] = GridTypeVisuals.at(NodeGlyph::Start);
			}
			else if (node == path.back())
			{
				grid[point.Y()][point.X()] = GridTypeVisuals.at(NodeGlyph::End);
			}
			else if (find(path.begin(), path.end(), node) != path.end())
			{
				grid[point.Y()][point.X()] = GridTypeVisuals.at(NodeGlyph::Visited);
			}
		}
	}

	cout << "Grid\n=======================================" << endl;
	for (int32_t y = 0; y < gridHeight; ++y)
	{
		for (int32_t x = 0; x < gridWidth; ++x)
		{
			cout << grid[y][x] << " ";
		}
		cout << endl;
	}
	cout << "=======================================" << endl;
}
