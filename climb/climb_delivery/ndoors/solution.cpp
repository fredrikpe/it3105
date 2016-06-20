#include <iostream>

int main()
{
  int N;
  std::cin >> N;

  bool *is_open = new bool[N]{ false };
 
  for (int pass = 0; pass < N; ++pass)
    for (int door = pass; door < N; door += pass+1)
      is_open[door] = !is_open[door];
 
  for (int door = 0; door < N; ++door)
    if (is_open[door]) 
        std::cout << door+1 << " ";
  return 0;
}
