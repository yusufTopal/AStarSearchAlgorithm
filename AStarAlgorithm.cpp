#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

enum class State { kStart, kFinish, kEmpty, kObstacle, kClosed, kPath };

/**
 * Where can an object move
*/
const int delta[4][2]{{-1, 0}, {0, -1}, {1, 0}, {0, 1}};

/**
 * Parse piece of map to state
*/
std::vector<State> ParseLine(std::string line) {
  std::vector<State> states;
  std::istringstream myStream(line);
  int step;
  char seperator;
  while (myStream >> step >> seperator && seperator == ',') {
    if (step == 0)
      states.push_back(State::kEmpty);
    else {
      states.push_back(State::kObstacle);
    }
  }
  return states;
}
/**
 * Reading map.
*/
std::vector<std::vector<State>> ReadBoardFile(std::string path) {
  std::ifstream streamer(path);
  std::string currentLine;
  std::vector<std::vector<State>> board{};
  if (streamer) {
    while (getline(streamer, currentLine)) {
      std::vector<State> temp = ParseLine(currentLine);
      board.push_back(temp);
    }
  }
  return board;
}

std::string CellString(State cell) {
  switch (cell) {
  case State::kObstacle:
    return "⛰️   ";
  case State::kPath:
    return "🚗   ";
  case State::kStart:
    return "🚦   ";
  case State::kFinish:
    return "🏁   ";
  default:
    return "0   ";
  }
}

void PrintBoard(const std::vector<std::vector<State>> board) {
  for (int i = 0; i < board.size(); i++) {
    for (int j = 0; j < board[i].size(); j++) {
      std::cout << CellString(board[i][j]);
    }
    std::cout << "\n";
  }
}
/**
 * Calculates "h(x,y)", Manhattan distance, between two points
*/
int Heuristic(int x1, int y1, int x2, int y2) {
  return std::abs(x2 - x1) + std::abs(y2 - y1);
}

/**
 * Compare f-values(g+h) of two possible nodes
*/
bool Compare(const std::vector<int> x, const std::vector<int> y) {
  int xfValue = x[2] + x[3];
  int yfValue = y[2] + y[3];
  return xfValue > yfValue;
}

/**
 * Sort open nodes by f-values
*/
void CellSort(std::vector<std::vector<int>> *list) {
  std::sort(list->begin(), list->end(), Compare);
}
/**
 * Check if the possible cell exists in grid and empty
*/
bool CheckValidCell(int x, int y, std::vector<std::vector<State>> &grid) {
  bool isXinGrid = (x >= 0 && x < grid.size());
  bool isYinGrid = (y >= 0 && y < grid[0].size());
  if (isYinGrid && isXinGrid)
    return grid[x][y] == State::kEmpty;

  return false;
}

/**
 * Add new node to open node list
*/
void AddToOpen(int x, int y, int g, int h,
               std::vector<std::vector<State>> &grid,
               std::vector<std::vector<int>> &openList) {
  openList.push_back(std::vector<int>{x, y, g, h});
  grid[x][y] = State::kClosed;
}

/**
 * Search for possible routes
*/
void ExpandNeighbors(const std::vector<int> &currentNode,
                     std::vector<std::vector<int>> &openList,
                     std::vector<std::vector<State>> &grid, int goal[2]) {
  // Get current node info
  int x = currentNode[0];
  int y = currentNode[1];
  int g = currentNode[2];

  // Start searching nodes around
  for (int i = 0; i < 4; i++) {
    int nx = x + delta[i][0];
    int ny = y + delta[i][1];
    if (CheckValidCell(nx, ny, grid)) {
      int ng = g + 1;
      int h = Heuristic(nx, ny, goal[0], goal[1]);
      AddToOpen(nx, ny, ng, h, grid, openList);
    }
  }
}

std::vector<std::vector<State>> Search(std::vector<std::vector<State>> grid,
                                       int init[2], int goal[2]) {
  std::vector<std::vector<int>> openList{};

  //Set start point and add to open node list
  int x = init[0];
  int y = init[1];
  int g = 0;
  int h = Heuristic(x, y, goal[0], goal[1]);
  AddToOpen(x, y, g, h, grid, openList);

  while (openList.size() > 0) {
    CellSort(&openList);
    auto currentNode = openList.back();
    openList.pop_back();
    int x = currentNode[0];
    int y = currentNode[1];
    grid[x][y] = State::kPath;

    // Check if you reached the goal
    if (x == goal[0] && y == goal[1]) {
      grid[init[0]][init[1]] = State::kStart;
      grid[goal[0]][goal[1]] = State::kFinish;
      return grid;
    }
    ExpandNeighbors(currentNode, openList, grid, goal);
  }
  std::cout << "No path found!"
            << "\n";
  return std::vector<std::vector<State>>{};
}

int main() {
  int init[2]{0, 0};
  int goal[2]{6, 5};
  auto board = ReadBoardFile("test.txt");
  auto solution = Search(board, init, goal);
  PrintBoard(solution);
  return 0;
}