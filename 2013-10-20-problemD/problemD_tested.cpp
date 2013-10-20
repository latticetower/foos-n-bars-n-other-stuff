#include <iostream>
#include <time.h>
#include <string>
#include <stdlib.h>

struct Cell{
  long weight;//количество предметов в ячейке
  long total_cost;//стоимость перемещений всего в ячейку
  long w2; //для тестирования
  Cell(): weight(0), total_cost(0), w2(0) {  }
  Cell(long w): weight(w), total_cost(0) { }
};

struct min_result{
  long x; long y; long min;
  min_result():x(0), y(0), min(0) { }
};

struct WeightData
{
  long cell, prev, prev_el, next, next_el;
  WeightData(): cell(0), prev(0),next(0), prev_el(0), next_el(0) { }
  WeightData(long c, long p = 0, long n = 0): cell(c), prev(p), next(n), prev_el(0), next_el(0) { }
  long cost() { return next + prev; }
  long prev_cost() { return cell + prev + prev_el; }
  long next_cost() { return cell + next + next_el; }
};


min_result get_problem_input_and_return_something(long m, long n, Cell** field, bool with_debug = false) 
{
  long all_weights = 0;
  WeightData* data_m = new WeightData[m];
  WeightData* data_n = new WeightData[n];
  for (int i = 0; i < m; i++)
  {
    for (int j = 0; j < n; j++) 
    {
      all_weights += field[i][j].weight;
      data_m[i].cell += field[i][j].weight;
      data_n[j].cell += field[i][j].weight;
    }
  } 
  for (int i = 1; i < m; i++)
  {
    data_m[i].prev_el = data_m[i - 1].cell + data_m[i - 1].prev_el;
    data_m[i].prev = data_m[i - 1].prev_cost();

    data_m[m - 1 - i].next_el = data_m[m - i].cell + data_m[m - i].next_el;
    data_m[m - 1 - i].next = data_m[m - i].next_cost();
  }
  for (int j = 1; j < n; j++)
  {
    data_n[j].prev_el = data_n[j - 1].cell + data_n[j - 1].prev_el;
    data_n[j].prev = data_n[j - 1].prev_cost();

    data_n[n - 1 - j].next_el = data_n[n - j].cell + data_n[n - j].next_el;
    data_n[n - 1 - j].next = data_n[n - j].next_cost();
  }
  min_result minimum;
  minimum.min = data_m[0].cost() + data_n[0].cost();
  for (long i = 0; i < m; i++)
  {
    for (long j = 0; j < n; j++)
    {
      field[i][j].total_cost = data_m[i].cost() + data_n[j].cost();
      if (field[i][j].total_cost < minimum.min)
      {
        minimum.min = field[i][j].total_cost;
        minimum.x = i; 
        minimum.y = j;
      }
    }
  }
  
  minimum.min += all_weights;
  return minimum;
}

void debug_some_data(long m, long* array, char * array_name)
{
  std::cout << array_name << std::endl;
  for (long i = 0; i < m; i++) {
    std::cout << array[i] << " ";
  }
  std::cout << std::endl;
}

void get_bruteforced_value(long m, long n, Cell** space_matrix, long k, long l)
{
  space_matrix[k][l].w2 = 0;
  for (long i = 0; i < m; i++)
  {
    for (long j = 0; j < n; j++)
    {
      if (space_matrix[i][j].weight > 0)
        space_matrix[k][l].w2 += space_matrix[i][j].weight*(1+ abs(k-i) + abs(l - j));
    }
  }
}

min_result get_problem_input_and_return_bruteforced(long m, long n, Cell** space_matrix)
{
  for (long i = 0; i < m; i++)
  {
    for (long j = 0; j < n; j++)
    {
      get_bruteforced_value(m, n, space_matrix, i, j);
    }
  }
  min_result minimum;
  minimum.min = space_matrix[0][0].w2;
  for (long i = 0; i < m; i++)
  {
    for (long j = 0; j < n; j++)
    {
      if (space_matrix[i][j].w2 < minimum.min)
      {
        minimum.x = i;
        minimum.y = j;
        minimum.min = space_matrix[i][j].w2;
      }
    }
  }
  return minimum;
}
/*real solution function*/
void solution()
{
  long m, n;
  std::cin >> m >> n;
  Cell ** space_matrix = new Cell*[m];
  for (long i = 0; i < m; i++)
  {
    space_matrix[i] = new Cell[n];
    for (long j = 0; j < n; j++) 
    {
      std::cin >> space_matrix[i][j].weight;
    }
  }

  min_result minimum = get_problem_input_and_return_something(m,n,space_matrix); 
  std::cout << (minimum.x + 1) << " " << (minimum.y + 1) << std::endl;
  std::cout << minimum.min*2 << std::endl;
}

bool test_stub(long max_rand = 10, long max_number=3)
{
  srand(time(NULL));
  long m, n;
  m = rand() % max_rand + 1;
  n = rand() % max_rand + 1;
  Cell ** space_matrix = new Cell*[m];
  for (long i = 0; i < m; i++)
  {
    space_matrix[i] = new Cell[n];
    for (long j = 0; j < n; j++) 
    {
      space_matrix[i][j].weight = rand() % max_number;
    }
  }

  min_result minimum1 = get_problem_input_and_return_something(m, n, space_matrix, true); 
  min_result minimum2 = get_problem_input_and_return_bruteforced(m, n, space_matrix); 
  if (minimum1.min != minimum2.min)
  {
    std::cout << "----ERROR! doesnt' work!!! on test: m = " << m << ", n = "<< n << std::endl;
    for (long i = 0; i < m; i++)
    {
      for (long j = 0; j < n; j++)
      {
        std::cout << space_matrix[i][j].weight << " ";
      }
      std::cout << std::endl;
    }
    std::cout << "min1: at(" << (minimum1.x + 1) << ", " << (minimum1.y + 1) <<"), value: " << minimum1.min << std::endl;
    std::cout << "min2: at(" << (minimum2.x + 1) << ", " << (minimum2.y + 1) <<"), value: " << minimum2.min << std::endl;
    std::cout << "---------end of error output----" << std::endl;
    return false;
  }
  else
  {
    std::cout << "tested: ok" << std::endl;
  }
  return true;
}

int main() {
  /*char result = ' ';
  while (result!='Q'){ 
    if (!test_stub(10, 2))
      std::cin >> result;
  }*/

  solution();
  return 0;
}