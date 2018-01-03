#include <iostream>
#include <cstdio>

static const int CAT = -1;
static int **map = 0;

void read_normalized(int &x, int &y) {
  std::cin >> x >> y;
  --x, --y;
}

int get_value(int row, int col) {
  if (row == 0 && col == 0)
    return 1;

  if (row < 0 || col < 0 || map[row][col] == CAT) {
    return 0;
  }

  return map[row][col];
}

int main(int argc, char **argv) {
  int R, C;
  std::cin >> R >> C;

  map = new int *[R];
  for (int i = 0; i < R; ++i)
    map[i] = new int[C];

  int K;
  std::cin >> K;
  for (int i = 0; i < K; ++i) {
    int r, c;
    read_normalized(r, c);
    map[r][c] = CAT;
  }

  for (int i = 0; i < R; ++i) {
    for (int j = 0; j < C; ++j) {
      if (map[i][j] == CAT) continue;
      map[i][j] = get_value(i - 1, j) + get_value(i, j - 1);
    }
  }

  std::cout << map[R-1][C-1] << std::endl;
  return 0;
}
