#include "pch.h"

using namespace std;
using namespace Library;

bool LoadGrid(Graph& graph, const string& gridFile, uint32_t& gridWidth, uint32_t& gridHeight, const Point& start, const Point& end);
void RunPathFinding(PathFindingType type, const Graph& graph, uint32_t gridWidth, uint32_t gridHeight, const Point& start, const Point& end);
void PrintGrid(const Graph& graph, uint32_t gridWidth, uint32_t gridHeight, const deque<shared_ptr<Node>>& path, bool moreDetails = false);

int main(int argc, char* argv[])
{
	#if defined(DEBUG) | defined(_DEBUG)
		_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	#endif

	if (argc < 6)
	{
		cout << "Usage: " << argv[0] << " <gridfile> <startX> <startY> <endX> <endY>" << endl;
		return 1;
	}	

	string gridFile = argv[1];
	Point start(stoul(argv[2]), stoul(argv[3]));
	Point end(stoul(argv[4]), stoul(argv[5]));
	PathFindingType pathFindingType = PathFindingType::MaxTypes;
	if (argc == 7)
	{
		pathFindingType = static_cast<PathFindingType>(stoul(argv[6]));
	}

	uint32_t gridWidth, gridHeight;
	Graph graph;
	if (!LoadGrid(graph, gridFile, gridWidth, gridHeight, start, end))
	{
		return 1;
	}

	RunPathFinding(pathFindingType, graph, gridWidth, gridHeight, start, end);
	return 0;
}

bool LoadGrid(Graph& graph, const string& gridFile, uint32_t& gridWidth, uint32_t& gridHeight, const Point& start, const Point& end)
{
	int32_t gridW, gridH;
	try
	{
		graph = GridHelper::LoadGridFromFile(gridFile, gridW, gridH);
	}
	catch (exception)
	{
		cout << "Error: Unable to load grid file." << endl;
		return false;
	}

	if (gridW <= 0 || gridH <= 0)
	{
		cout << "Error: Invalid grid size." << endl;
		return false;
	}

	gridWidth = gridW;
	gridHeight = gridH;
	if (static_cast<uint32_t>(start.X()) >= gridWidth || static_cast<uint32_t>(start.Y()) >= gridHeight
		|| static_cast<uint32_t>(end.X()) >= gridWidth || static_cast<uint32_t>(end.Y()) >= gridHeight)
	{
		cout << "Error: start / end point(s) outside grid region" << endl;
		return false;
	}
	return true;
}

void RunPathFinding(PathFindingType runType, const Graph& graph, uint32_t gridWidth, uint32_t gridHeight, const Point& start, const Point& end)
{
	static const unordered_map<PathFindingType, pair<string, shared_ptr<IPathFinder>>> algorithms = {
		{ PathFindingType::BreadthFirst,{ "BreadthFirst", make_shared<BreadthFirst>() } },
		{ PathFindingType::DepthFirst,{ "DepthFirst", make_shared<DepthFirst>() } },
		{ PathFindingType::GreedyBestFirst,{ "GreedyBestFirst", make_shared<GreedyBestFirst>() } },
		{ PathFindingType::Dijkstra,{ "Dijkstra", make_shared<Dijkstra>() } },
		{ PathFindingType::AStar,{ "AStar", make_shared<AStar>() } },
		{ PathFindingType::MaxTypes,{ "", shared_ptr<IPathFinder>() } }
	};

	StopWatch watch;
	uint32_t startType = (runType == PathFindingType::MaxTypes) ? static_cast<uint32_t>(PathFindingType::BreadthFirst) : static_cast<uint32_t>(runType);
	uint32_t endType = (runType == PathFindingType::MaxTypes) ? static_cast<uint32_t>(PathFindingType::MaxTypes) : static_cast<uint32_t>(runType) + 1;
	for (uint32_t type = startType; type < endType; ++type)
	{
		for (auto& entry : graph.Nodes())
		{
			entry.second->SetHeuristic(0.0f);
			entry.second->SetPathCost(0.0f);
		}

		const auto& algorithm = algorithms.at(static_cast<PathFindingType>(type));
		set<shared_ptr<Node>> closedSet;
		watch.Reset();
		watch.Start();
		const auto path = algorithm.second->FindPath(graph.At(start), graph.At(end), closedSet);
		watch.Stop();

		if (runType == PathFindingType::MaxTypes)
		{
			cout << algorithm.first;
			if (path.empty())
			{
				cout << " : No path found!";
			}
			cout << "\n=======================================" << endl;
		}
		else
		{
			cout << !path.empty() << endl;
		}

		PrintGrid(graph, gridWidth, gridHeight, path, (runType != PathFindingType::MaxTypes));

		if (runType == PathFindingType::MaxTypes)
		{
			cout << "=======================================" << endl;
			cout << "Elapsed Time = " << watch.Elapsed().count() << " micro seconds, Path Length = " << path.size()
				<< ", Nodes visited = " << closedSet.size() << endl;
			cout << endl;
		}
		else
		{
			cout << watch.Elapsed().count();
		}
	}
}

void PrintGrid(const Graph& graph, uint32_t gridWidth, uint32_t gridHeight, const deque<shared_ptr<Node>>& path, bool moreDetails)
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
			if (moreDetails)
			{
				cout << node->Heuristic() << " " << node->PathCost() << " " << node->TotalCost() << " ";
			}
		}
		cout << endl;
	}
}
