#include <iostream>
#include <vector>
#include <string>
#include <list>
#include <limits>
#include <set>
#include <utility>
#include <algorithm>
#include <iterator>
#include <ctime>
#include <thread>
#include <chrono>

using namespace std;

const int ROWS = 20;
const int COLUMNS = 40;
const int WALLS = 30;

const char EMPTY = '-';
const char WALL = '#';
const char POSITION = 'x';
const char START = 'A';
const char END = 'B';
const char TRAIL = '*';
const bool LEAVE_TRAIL = true;

const int SEED = time(nullptr);
const int INTERVAL_MS = 100;


typedef int vertex_t;
typedef double weight_t;

const weight_t max_weight = std::numeric_limits<double>::infinity();

struct neighbor {
    vertex_t target;
    weight_t weight;
    neighbor(vertex_t arg_target, weight_t arg_weight)
        : target(arg_target), weight(arg_weight) { }
};

typedef std::vector<std::vector<neighbor> > adjacency_list_t;


void DijkstraComputePaths(vertex_t source,
    const adjacency_list_t& adjacency_list,
    std::vector<weight_t>& min_distance,
    std::vector<vertex_t>& previous)
{
    int n = adjacency_list.size();
    min_distance.clear();
    min_distance.resize(n, max_weight);
    min_distance[source] = 0;
    previous.clear();
    previous.resize(n, -1);
    std::set<std::pair<weight_t, vertex_t> > vertex_queue;
    vertex_queue.insert(std::make_pair(min_distance[source], source));

    while (!vertex_queue.empty())
    {
        weight_t dist = vertex_queue.begin()->first;
        vertex_t u = vertex_queue.begin()->second;
        vertex_queue.erase(vertex_queue.begin());

        // Visit each edge exiting u
        const std::vector<neighbor>& neighbors = adjacency_list[u];
        for (std::vector<neighbor>::const_iterator neighbor_iter = neighbors.begin();
            neighbor_iter != neighbors.end();
            neighbor_iter++)
        {
            vertex_t v = neighbor_iter->target;
            weight_t weight = neighbor_iter->weight;
            weight_t distance_through_u = dist + weight;
            if (distance_through_u < min_distance[v]) {
                vertex_queue.erase(std::make_pair(min_distance[v], v));

                min_distance[v] = distance_through_u;
                previous[v] = u;
                vertex_queue.insert(std::make_pair(min_distance[v], v));

            }

        }
    }
}


std::list<vertex_t> DijkstraGetShortestPathTo(
    vertex_t vertex, const std::vector<vertex_t>& previous)
{
    std::list<vertex_t> path;
    for (; vertex != -1; vertex = previous[vertex])
        path.push_front(vertex);
    return path;
}


void generateWalls(vector<vector<char>>& grid)
{
    srand(SEED);

    int wallCount = 0;

    while (wallCount < WALLS)
    {
        int x = rand() % ROWS;
        int y = rand() % COLUMNS;

        if (grid[x][y] == EMPTY)
        {
            grid[x][y] = WALL;
            wallCount++;
        }
    }
}

void drawGrid(const vector<vector<char>>& grid)
{
    system("cls");

    for (int i = 0; i < ROWS; ++i)
    {
        for (int j = 0; j < COLUMNS; ++j)
        {
            cout << grid[i][j];
        }
        cout << endl;
    }
}

void findPath(int startX, int startY, int endX, int endY)
{
    vector<vector<char>> grid(ROWS, vector<char>(COLUMNS, EMPTY));

    generateWalls(grid);

    grid[startX][startY] = START;
    grid[endX][endY] = END;

    adjacency_list_t adjacencyList(ROWS * COLUMNS);

    for (int i = 0; i < ROWS; ++i)
    {
        for (int j = 0; j < COLUMNS; ++j)
        {
            if (i > 0 && grid[i - 1][j] != WALL) // Gore
            {
                adjacencyList[i * COLUMNS + j].push_back(neighbor((i - 1) * COLUMNS + j, 1));
            }
            if (i < ROWS - 1 && grid[i + 1][j] != WALL) // Dolje
            {
                adjacencyList[i * COLUMNS + j].push_back(neighbor((i + 1) * COLUMNS + j, 1));
            }
            if (j > 0 && grid[i][j - 1] != WALL) // Lijevo
            {
                adjacencyList[i * COLUMNS + j].push_back(neighbor(i * COLUMNS + j - 1, 1));
            }
            if (j < COLUMNS - 1 && grid[i][j + 1] != WALL) // Desno
            {
                adjacencyList[i * COLUMNS + j].push_back(neighbor(i * COLUMNS + j + 1, 1));
            }
        }
    }

    vector<weight_t> min_distance;
    vector<vertex_t> previous;

    DijkstraComputePaths(startX * COLUMNS + startY, adjacencyList, min_distance, previous);

    list<vertex_t> path = DijkstraGetShortestPathTo(endX * COLUMNS + endY, previous);

    for (vertex_t vertex : path)
    {
        int x = vertex / COLUMNS;
        int y = vertex % COLUMNS;

        if (x != startX || y != startY)
        {
            grid[x][y] = POSITION;
        }

        drawGrid(grid);

        if (LEAVE_TRAIL && (x != startX || y != startY))
        {
            grid[x][y] = TRAIL;
        }
        else if (x != startX || y != startY)
        {
            grid[x][y] = EMPTY;
        }

        this_thread::sleep_for(chrono::milliseconds(INTERVAL_MS));
    }

    if (path.size() <=1)
    {
        cout << endl << "No path found" << endl;
    }
}

int main()
{
    int startRow, startCol, endRow, endCol;

    cout << "Unesite redak pocetne tocke (1-" << ROWS << "): ";
    cin >> startRow;
    cout << "Unesite stupac pocetne tocke (1-" << COLUMNS << "): ";
    cin >> startCol;
    cout << "Unesite redak krajnje tocke (1-" << ROWS << "): ";
    cin >> endRow;
    cout << "Unesite stupac krajnje tocke (1-" << COLUMNS << "): ";
    cin >> endCol;

    findPath(--startRow, --startCol, --endRow, --endCol);

    return 0;
}