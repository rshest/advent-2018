#include <iostream>
#include <string>
#include <cstdlib>
#include <cstring>
#include <list>

using namespace std;

constexpr int CASE_DIFF = 'a' - 'A';
constexpr int NUM_CHARS = 'z' - 'a' + 1;

bool isCancelEachOther(char c1, char c2) {
  if (c1 > c2) return isCancelEachOther(c2, c1);
  return c2 == c1 + CASE_DIFF;
}

int countRemoved(const string& text, char maskOut = 0) {
  if (text.empty()) {
    return 0;
  }
  
  list<char> chars;
  for (char c: text) {
    if (maskOut != 0 && (c == maskOut || c == maskOut + CASE_DIFF)) {
      continue;
    }
    chars.push_back(c);
  }

  auto p1 = chars.begin();
  auto p2 = p1;
  p2++;
  
  while (p2 != chars.end()) {
    if (isCancelEachOther(*p1, *p2)) {
      chars.erase(p1);
      p2 = chars.erase(p2);
      --p1;
    } else {
      p2++;
      p1++;
    }
  }
  return chars.size();
}

int countMaxRemovedWithoutOneChar(const string& text) {
  int res = text.size();
  for (int i = 0; i < NUM_CHARS; i++) {
    res = min(res, countRemoved(text, 'A' + i));
  }
  return res;
}

int main() {
  string text;
  cin >> text;
  
  int res1 = countRemoved(text);
  int res2 = countMaxRemovedWithoutOneChar(text);
  cout << "Result 1: " << res1 << endl;
  cout << "Result 2: " << res2 << endl;
}
