#ifndef SERVER_H
#define SERVER_H

#include <cstdlib>
#include <iostream>

const int MAX_SIZE = 35;

bool mine_map[MAX_SIZE][MAX_SIZE];
int mine_count[MAX_SIZE][MAX_SIZE];
bool visited[MAX_SIZE][MAX_SIZE];
bool marked[MAX_SIZE][MAX_SIZE];

int rows;
int columns;
int total_mines;
int game_state;
int visit_count;
int marked_mine_count;

int dx[] = {-1, -1, -1, 0, 0, 1, 1, 1};
int dy[] = {-1, 0, 1, -1, 1, -1, 0, 1};

void InitMap() {
  std::cin >> rows >> columns;
  total_mines = 0;
  visit_count = 0;
  marked_mine_count = 0;
  game_state = 0;

  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < columns; j++) {
      visited[i][j] = false;
      marked[i][j] = false;
      mine_count[i][j] = 0;
    }
  }

  for (int i = 0; i < rows; i++) {
    std::string line;
    std::cin >> line;
    for (int j = 0; j < columns; j++) {
      if (line[j] == 'X') {
        mine_map[i][j] = true;
        total_mines++;
      } else {
        mine_map[i][j] = false;
      }
    }
  }

  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < columns; j++) {
      if (!mine_map[i][j]) {
        int cnt = 0;
        for (int d = 0; d < 8; d++) {
          int ni = i + dx[d];
          int nj = j + dy[d];
          if (ni >= 0 && ni < rows && nj >= 0 && nj < columns && mine_map[ni][nj]) {
            cnt++;
          }
        }
        mine_count[i][j] = cnt;
      }
    }
  }
}

void VisitBlock(int r, int c) {
  if (r < 0 || r >= rows || c < 0 || c >= columns) return;
  if (visited[r][c] || marked[r][c]) return;
  if (game_state != 0) return;

  if (mine_map[r][c]) {
    visited[r][c] = true;
    game_state = -1;
    return;
  }

  visited[r][c] = true;
  visit_count++;

  if (mine_count[r][c] == 0) {
    for (int d = 0; d < 8; d++) {
      int ni = r + dx[d];
      int nj = c + dy[d];
      if (ni >= 0 && ni < rows && nj >= 0 && nj < columns) {
        VisitBlock(ni, nj);
      }
    }
  }

  int total_non_mine = rows * columns - total_mines;
  if (visit_count == total_non_mine) {
    game_state = 1;
    marked_mine_count = total_mines;
  }
}

void MarkMine(int r, int c) {
  if (r < 0 || r >= rows || c < 0 || c >= columns) return;
  if (visited[r][c] || marked[r][c]) return;
  if (game_state != 0) return;

  if (mine_map[r][c]) {
    marked[r][c] = true;
    marked_mine_count++;
  } else {
    game_state = -1;
  }
}

void AutoExplore(int r, int c) {
  if (r < 0 || r >= rows || c < 0 || c >= columns) return;
  if (!visited[r][c] || mine_map[r][c]) return;
  if (game_state != 0) return;

  int marked_around = 0;
  for (int d = 0; d < 8; d++) {
    int ni = r + dx[d];
    int nj = c + dy[d];
    if (ni >= 0 && ni < rows && nj >= 0 && nj < columns && marked[ni][nj]) {
      marked_around++;
    }
  }

  if (marked_around == mine_count[r][c]) {
    for (int d = 0; d < 8; d++) {
      int ni = r + dx[d];
      int nj = c + dy[d];
      if (ni >= 0 && ni < rows && nj >= 0 && nj < columns && !visited[ni][nj] && !marked[ni][nj]) {
        VisitBlock(ni, nj);
      }
    }
  }
}

void ExitGame() {
  if (game_state == 1) {
    std::cout << "YOU WIN!" << std::endl;
  } else {
    std::cout << "GAME OVER!" << std::endl;
  }
  std::cout << visit_count << " " << marked_mine_count << std::endl;
  exit(0);
}

void PrintMap() {
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < columns; j++) {
      if (visited[i][j]) {
        if (mine_map[i][j]) {
          std::cout << "X";
        } else {
          std::cout << mine_count[i][j];
        }
      } else if (marked[i][j]) {
        if (game_state == 1) {
          std::cout << "@";
        } else {
          if (mine_map[i][j]) {
            std::cout << "@";
          } else {
            std::cout << "X";
          }
        }
      } else {
        if (game_state == 1 && mine_map[i][j]) {
          std::cout << "@";
        } else {
          std::cout << "?";
        }
      }
    }
    std::cout << std::endl;
  }
}

#endif
