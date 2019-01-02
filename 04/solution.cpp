#include <iostream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <string>
#include <cstdlib>
#include <cstring>
#include <iomanip>
#include <unordered_map>
#include <unordered_set>

using namespace std;

struct SleepSegment {
  int guardId;
  int lengthMinutes;
  int startMinute;
};

typedef unordered_map<int, vector<SleepSegment>> GuardSleepMap;

GuardSleepMap readSleepMap() {
  vector<string> lines;
  for (string line; getline(cin, line);) {
    lines.push_back(line);
  }
  sort(lines.begin(), lines.end());
  
  GuardSleepMap res;
  SleepSegment s{0};
  int year, month, day, hour, minute;  
  for (const string& line: lines) {
    const int r = sscanf(line.c_str(), "[%d-%d-%d %d:%d]",
		   &year, &month, &day, &hour, &minute);
    if (r != 5) {
      cerr << "Invalid input: " << line << endl;
      exit(1);
    }
    const char* p = strstr(line.c_str(), "] ") + 2;
    const bool isStart =
      ((s.guardId != -1) && (strstr(p, "falls") != nullptr)) ||
      (sscanf(p, "Guard #%d", &s.guardId) == 1);
    const bool isEnd = (s.guardId != -1) && (strstr(p, "wakes") != nullptr);

    if (isStart) {
      if (hour > 0) {
	hour = 0;
	minute = 0;
      }
      s.startMinute = minute;
      s.lengthMinutes = 60 - minute;
    } else if (isEnd) {
      s.lengthMinutes = minute - s.startMinute;
      if (res.find(s.guardId) == res.end()) {
	res[s.guardId] = {};
      }
      res[s.guardId].push_back(s);
    }
  }
  return res;
}

typedef pair<int, int> NPair;

pair<int, int> getMaxSleptMinute(const vector<SleepSegment>& segments) {
  typedef pair<int, int> SegmentEdge;
  vector<SegmentEdge> edges;
  for (int i = 0; i < segments.size(); i++) {
    const auto& s = segments[i];
    edges.push_back({s.startMinute, i});
    edges.push_back({s.startMinute + s.lengthMinutes, -i});
  }
  std::sort(begin(edges), end(edges));
  unordered_set<int> overlapped;
  int maxOverlapped = 0;
  int maxOverlappedMinute = -1;
  for (const auto& e: edges) {
    if (e.second >= 0) {
      overlapped.insert(e.second);
      if (overlapped.size() > maxOverlapped) {
	maxOverlapped = overlapped.size();
	maxOverlappedMinute = e.first;
      }
    } else {
      overlapped.erase(-e.second);
    }
  }
  return {maxOverlappedMinute, maxOverlapped};
}

int getStrategy1Result(const GuardSleepMap& sleepMap) {
  unordered_map<int, int> sleepTimes;
  int guardId = -1;
  int maxSleepTime = 0;
  for (const auto& it: sleepMap) {
    const auto& segments = it.second;
    int sleepTime = 0;
    for (const SleepSegment& seg: segments) {
      sleepTime += seg.lengthMinutes;
    }
    if (sleepTime > maxSleepTime) {
      maxSleepTime = sleepTime;
      guardId = it.first;
    }
  }

  const auto& segments = sleepMap.find(guardId)->second;
  const int maxMinute = getMaxSleptMinute(segments).first;
  return guardId * maxMinute;
}

int getStrategy2Result(const GuardSleepMap& sleepMap) {
  int maxMinute = -1, guardId = -1, maxOverlap = 0;
  for (const auto& it: sleepMap) {
    const auto& segments = it.second;
    const auto maxMinuteOverlapped = getMaxSleptMinute(segments);
    if (maxMinuteOverlapped.second > maxOverlap) {
      maxMinute = maxMinuteOverlapped.first;
      guardId = it.first;
      maxOverlap = maxMinuteOverlapped.second;
    }
  }
  return guardId * maxMinute;
}

int main() {
  auto sleepMap = readSleepMap();

  int res1 = getStrategy1Result(sleepMap);
  int res2 = getStrategy2Result(sleepMap);
  
  cout << "Result 1: " << res1 << endl;
  cout << "Result 2: " << res2 << endl;
}
