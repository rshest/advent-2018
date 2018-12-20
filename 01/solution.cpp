#include <iostream>
#include <sstream>
#include <set>
#include <vector>
#include <algorithm>
#include <stdlib.h>

typedef long long int_t;

static int_t getFirstRepeatingFreq(const std::vector<int_t>& values) {
  std::set<int_t> reached;
  int_t sum = 0;
  
  while (true) {
    for (int_t val: values) {
      sum += val;
      if (reached.find(sum) != reached.end()) {
	return sum;
      }
      reached.insert(sum);
    }
  }
}


int main() {
  int_t val;
  std::vector<int_t> values;  
  while ((std::cin >> val)) {
    values.push_back(val);
  }

  int_t res1 = std::accumulate(values.begin(), values.end(), 0);
  int_t res2 = getFirstRepeatingFreq(values);
  
  std::cout << "Result 1: " << res1 << std::endl;
  std::cout << "Result 2: " << res2 << std::endl;
}
