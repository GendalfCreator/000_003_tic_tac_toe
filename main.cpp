#include <iostream>
#include <random>

using namespace std;

#define POINT_ITEM(a, r, c) (*((*(a + r)) + c))
#define CHECK_DOT(x, sz) ((x) >= 0 && (x) < (sz))

enum PLAYER {HUMAN='X', AI='0', EMPTY='_'};

typedef struct {
  int szY;
  int szX;
  PLAYER** map;
  int towin;
} Field;

char getVal(PLAYER** array, const int col, const int row) {
  return POINT_ITEM(array, row, col);
}

void setVal(PLAYER** array, const int col, const int row, PLAYER value) {
  POINT_ITEM(array, row, col) = value;
}

bool isValid(Field &field, int x, int y) {
  return CHECK_DOT(x, field.szX) && CHECK_DOT(y, field.szY);
}

bool isEmpty(Field &field, int x, int y) {
  return getVal(field.map, x, y) == EMPTY;
}

void initGame(Field &field) {
  field.towin = 3;
  field.szY = 3;
  field.szX = 3;
  field.map = new PLAYER* [field.szY];
  for (int y = 0; y < field.szY; ++y) {
      *(field.map + y) = new PLAYER [field.szX];
      for (int x = 0; x < field.szX; ++x) {
          setVal(field.map, x, y, EMPTY);
        }
    }
}

void printField(Field &field) {
  system("clear");
  cout << "   1 2 3"<< endl;
  for (int i = 0; i < field.szY; ++i) {
      cout << " " << i+1 << "|";
      for (int j = 0; j < field.szX; ++j) {
          cout << getVal(field.map, i, j) << "|";
        }
      cout << endl;
    }
  cout << endl;
}

void turnHuman(Field &field) {
  int x, y;
  do {
      cout << "Введите номер строки и столбца через пробел: ";
      cin >> x >> y;
      x--;
      y--;
  }
  while (!isValid(field, x, y) || !isEmpty(field, x, y));
  setVal(field.map, x, y, HUMAN);
}

void turnAi(Field &field) {
  int x, y;

  random_device rd;
  mt19937 mt(rd());
  uniform_real_distribution <double> dist(0, 3);

  do {
      x = dist(mt);
      y = dist(mt);
    }
  while (!isEmpty(field, x, y));
  setVal(field.map, x, y, AI);
}

bool checkLine(Field &field, int x, int y, int vx, int vy, int len, PLAYER player) {
  const int endx = x + (len - 1) * vx;
  const int endy = y + (len - 1) * vy;

  if (!isValid(field, endx, endy))
    return false;
  for (int i = 0; i < len; i++)
    if (getVal(field.map, (y + i * vy), (x + i * vx)) != player)
      return false;
  return true;
}

bool checkWin(Field &field, PLAYER player) {
  for (int y = 0; y < field.szX; y++) {
      for (int x = 0; x < field.szY; x++) {
          if (checkLine(field, x, y, 1, 0, field.towin, player)) return true;
          if (checkLine(field, x, y, 1, 1, field.towin, player)) return true;
          if (checkLine(field, x, y, 0, 1, field.towin, player)) return true;
          if (checkLine(field, x, y, 1, -1, field.towin, player)) return true;
        }
    }
  return false;
}

bool checkDraw(Field &field) {
  for (int y = 0; y < field.szY; y++)
    for (int x = 0; x < field.szX; x++)
      if (isEmpty(field, x, y))
        return false;
  return true;
}

int main()
{
  Field field;
  initGame(field);

  while (true) {
      printField(field);
      turnHuman(field);
      if (checkWin(field, HUMAN) || checkDraw(field)) {
          printField(field);
          cout << "Human win" << endl;
          break;
        }
      turnAi(field);
      if (checkWin(field, AI) || checkDraw(field)) {
          printField(field);
          cout << "AI win" << endl;
          break;
        }
    }

  return 0;
}
