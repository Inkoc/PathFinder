#include <iostream>
#include <vector>
#include <thread>
#include <chrono>

using namespace std;

const int ROWS = 20;
const int COLUMNS = 40;

const char EMPTY = '-';
const char POSITION = 'x';
const char START = 'A';
const char END = 'B';
const char TRAIL = '*';
const bool LEAVE_TRAIL = true;

const int INTERVAL_MS = 100;

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
    grid[startX][startY] = START;
    grid[endX][endY] = END;
    int x = startX, y = startY;

    while (x != endX || y != endY)
    {
        if (x != startX || y != startY)
        {
            grid[x][y] = POSITION;
        }

        drawGrid(grid);
        this_thread::sleep_for(chrono::milliseconds(INTERVAL_MS));

        if (x != startX || y != startY)
        {
            if (LEAVE_TRAIL)
            {
                grid[x][y] = TRAIL;
            }
            else
            {
                grid[x][y] = EMPTY;
            }
        }

        if (x < endX) ++x;
        else if (x > endX) --x;
        else if (y < endY) ++y; 
        else if (y > endY) --y;
    }

    grid[endX][endY] = POSITION;
    drawGrid(grid);
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
