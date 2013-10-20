#include <iostream>
#include <time.h>
#include <string>
#include <stdlib.h>

struct Cell{
  long weight;//количество предметов в ячейке
  long total_cost;//стоимость перемещений всего в ячейку
  Cell(): weight(0), total_cost(0) {  }
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


int main()
{
  long m, n;
  std::cin >> m >> n;
  Cell **field = new Cell*[m];
  long all_weights = 0;
  WeightData* data_m = new WeightData[m];
  WeightData* data_n = new WeightData[n];
  for (int i = 0; i < m; i++)
  {
    field[i] = new Cell[n];
    for (int j = 0; j < n; j++) 
    {
      std::cin >> field[i][j].weight;
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

  for (long i = 0; i < m; i++)
  {
    for (long j = 0; j < n; j++)
    {
      field[i][j].total_cost = data_m[i].cost() + data_n[j].cost();
      std::cout << field[i][j].total_cost <<" ";
    }
    std::cout << std::endl;
  }


  for (long i = 0; i < m; i++)
    delete[] field[i];
  delete[] field;
  
  delete[] data_m;
  delete[] data_n;
  
  return 0;
}