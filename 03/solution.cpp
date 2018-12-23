#include <iostream>
#include <sstream>
#include <set>
#include <vector>
#include <algorithm>
#include <string>
#include <cstdlib>
#include <set>
#include "boost/icl/discrete_interval.hpp"
#include "boost/icl/interval_map.hpp"

using namespace std;
using namespace boost::icl;

struct Rect {
  int x, y, w, h;
};

vector<Rect> readRects() {
  vector<Rect> res;
  Rect rect;
  for (string line; getline(cin, line);) {
    const char* pos = line.c_str() + line.find('@') + 2;
    char* end;
    rect.x = strtol(pos, &end, 10);
    pos = end + 1;
    rect.y  = strtol(pos, &end, 10);
    pos = end + 2;
    rect.w = strtol(pos, &end, 10);
    pos = end + 1;
    rect.h =  strtol(pos, &end , 10);
    res.push_back(rect);
  }
  return res;
}

typedef pair<int, int> Vertical;
typedef interval_map<int, set<int>> IntervalMap;

pair<int, set<int>> numOverlaps(const vector<Rect>& rects) {
  if (rects.empty()) {
    return {0, {}};
  }
  
  // Create a list of vertical rectangle lines, sorted by X coordinate
  // Rectange indexes go from 1 to N, positive is a rectangle opening vertical,
  // negative is the closing one
  vector<Vertical> verticals;
  set<int> nonOverlapping;
  for (int i = 0; i < rects.size(); i++) {
    const int idx = i + 1;
    verticals.push_back(make_pair(rects[i].x, idx));
    verticals.push_back(make_pair(rects[i].x + rects[i].w, -idx));
    nonOverlapping.insert(idx);
  }
  sort(verticals.begin(), verticals.end());

  // Go through all the intervals created by verticals,
  // maintain a set of currently active intervals
  int res = 0;
  int curX = -1;
  IntervalMap intervals;
  for (const auto& v: verticals) {
    const int rectIndex = v.second;
    const Rect& rect = rects[abs(rectIndex) - 1];
    const int y = rect.y;
    const int h = rect.h;
    auto curInt = discrete_interval<int>::closed(y, y + h - 1);

    for (IntervalMap::const_iterator it = intervals.begin();
         it != intervals.end(); ++ it) {
      const discrete_interval<int>& i = (*it).first;
      if (it->second.size() > 1 && curX >= 0) {
	res += length(i)*(v.first - curX);
	for (int idx: it->second) {
	  nonOverlapping.erase(idx);
	}
      }
    }
      
    if (rectIndex < 0) {
      intervals -= make_pair(curInt, set<int>{-rectIndex});
    } else {
      intervals += make_pair(curInt, set<int>{rectIndex});
    }
    curX = v.first;
  }
  
  return {res, nonOverlapping};
}

int main() {
  auto rects = readRects();

  auto res = numOverlaps(rects);
  cout << "Result 1: " << res.first << endl;
  cout << "Result 2: " << *res.second.begin() << endl;
 }
