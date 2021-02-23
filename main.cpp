#include <iostream>

using namespace std;

#define POINT_ITEM(a, r, c) (*((*(a + r)) + c))

enum PLAYER {HUMAN='X', AI='0', EMPTY='_'};

typedef struct {
  int szY;
  int szX;
  PLAYER** map;
  int towin;
} Field;

char getval(PLAYER** array, const int col, const int row) {
  return POINT_ITEM(array, row, col);
}

void setval(PLAYER** array, const int col, const int row, PLAYER value) {
  POINT_ITEM(array, row, col) = value;
}

void init(Field &field) {
  field.towin = 3;
  field.szY = 3;
  field.szX = 3;
  field.map = new PLAYER* [field.szY];
  for (int y = 0; y < field.szY; ++y) {
      *(field.map + y) = new PLAYER [field.szX];
      for (int x = 0; x < field.szX; ++x) {
          setval(field.map, x, y, EMPTY);
        }
    }

}

void print(Field &field) {
  system("clear");
  cout << endl;
  for (int i = 0; i < field.szY; ++i) {
      cout << "|";
      for (int j = 0; j < field.szX; ++j) {
          cout << getval(field.map, i, j) << "|";
        }
      cout << endl;
    }
  cout << endl;
}


int main()
{
  Field field;

  init(field);
  print(field);

  return 0;
}
