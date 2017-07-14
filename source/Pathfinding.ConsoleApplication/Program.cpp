#include "pch.h"

using namespace std;
using namespace Library;

void PrintGrid(const Graph& graph, uint32_t gridWidth, uint32_t gridHeight, const deque<shared_ptr<Node>>& path);

int main(int argc, char* argv[])
{
#if defined(DEBUG) | defined(_DEBUG)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	if (argc != 6)
	{
		cout << "Usage: " << argv[0] << " <gridfile> <startX> <startY> <endX> <endY>" << endl;
		return 1;
	}

	int32_t gridW, gridH;
	Graph graph = GridHelper::LoadGridFromFile(argv[1], gridW, gridH);
	assert(gridW > 0 && gridH > 0);
	uint32_t gridWidth = gridW;
	uint32_t gridHeight = gridH;

	uint32_t startX = stoul(argv[2]);
	uint32_t startY = stoul(argv[3]);
	uint32_t endX = stoul(argv[4]);
	uint32_t endY = stoul(argv[5]);
	if (startX >= gridWidth || startY >= gridHeight || endX >= gridWidth || endY >= gridHeight)
	{
		cout << "Error: start / end point(s) outside grid region" << endl;
		return 1;
	}

	static const unordered_map<PathFindingType, pair<string, shared_ptr<IPathFinder>>> algorithms =  {
		{ PathFindingType::BreadthFirst, { "BreadthFirst", make_shared<BreadthFirst>() } },
		{ PathFindingType::DepthFirst, { "DepthFirst", make_shared<DepthFirst>() } },
		{ PathFindingType::GreedyBestFirst, { "GreedyBestFirst", make_shared<GreedyBestFirst>() } },
		{ PathFindingType::Dijkstra, { "Dijkstra", make_shared<Dijkstra>() } },
		{ PathFindingType::AStar, { "AStar", make_shared<AStar>() } },
		{ PathFindingType::MaxTypes, { "", shared_ptr<IPathFinder>() } }
	};

	Point start(startX, startY), end(endX, endY);
	StopWatch watch;
	for (uint32_t type = static_cast<uint32_t>(PathFindingType::BreadthFirst); type < static_cast<uint32_t>(PathFindingType::MaxTypes); ++type)
	{
		const auto& algorithm = algorithms.at(static_cast<PathFindingType>(type));
		set<shared_ptr<Node>> closedSet;
		watch.Reset();
		watch.Start();
		const auto path = algorithm.second->FindPath(graph.At(start), graph.At(end), closedSet);
		watch.Stop();

		cout << algorithm.first;
		if (path.empty())
		{
			cout << " : No path found!";
		}
		PrintGrid(graph, gridWidth, gridHeight, path);
		cout << "Elapsed Time = " << watch.Elapsed().count() << " micro seconds, Path Length = " << path.size()
			<< ", Nodes visited = " << closedSet.size() << endl;
		cout << endl;
	}

	cin.get();
	return 0;
}


void PrintGrid(const Graph& graph, uint32_t gridWidth, uint32_t gridHeight, const deque<shared_ptr<Node>>& path)
{
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

	cout << "\n=======================================" << endl;
	for (uint32_t y = 0; y < gridHeight; ++y)
	{
		for (uint32_t x = 0; x < gridWidth; ++x)
		{
			const auto& node = graph.At(x, y);
			
			char glyph = GridTypeVisuals.at(GridTypeToVisualType.at(node->Type()));

			if (!path.empty())
			{
				if (node == path.front())
				{
					glyph = GridTypeVisuals.at(NodeGlyph::Start);
				}
				else if (node == path.back())
				{
					glyph = GridTypeVisuals.at(NodeGlyph::End);
				}
				else if (find(path.begin(), path.end(), node) != path.end())
				{
					glyph = GridTypeVisuals.at(NodeGlyph::Visited);
				}
			}

			cout << glyph << " ";
		}
		cout << endl;
	}
	cout << "=======================================" << endl;
}
