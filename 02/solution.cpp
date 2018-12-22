#include <iostream>
#include <sstream>
#include <set>
#include <vector>
#include <algorithm>
#include <string>
#include <stdlib.h>

using namespace std;

static pair<bool, bool> hasCharCounts2Or3(const string& str) {
  vector<int> counts(256, 0);
  for (char c: str) {
    counts[c]++;
  }
  
  bool has2 = false, has3 = false;
  for (int cnt: counts) {
    has2 |= (cnt == 2);
    has3 |= (cnt == 3);
    if (has2 && has3) {
      break;
    }
  }
  return {has2, has3};
}


static int countChecksum(const vector<string>& ids) {
  int num2 = 0;
  int num3 = 0;
  for (const string& id: ids) {
    auto has23 = hasCharCounts2Or3(id);
    num2 += has23.first;
    num3 += has23.second;
  }
  return num2*num3;
}

// NB. Modifies the input array
string findCommonCharsOfCorrectIDsForward(vector<string>& ids) {
  sort(ids.begin(), ids.end());
  for (int i = ids.size() - 1; i > 0; i--) {
    const string& id1 = ids[i];
    const string& id2 = ids[i - 1];
    int pos1 = 0;
    int pos2 = 0;
        while (pos1 < id1.size() && pos1 >= 0 && pos2 < id2.size() && pos2 >= 0) {
      if (id1[pos1] != id2[pos2]) {
	if (id1.compare(pos1 + 1, id1.size() - pos1 - 1,
			id2, pos2 + 1, id2.size() - pos2 - 1) == 0) {
	  return id1.substr(0, pos1) + id1.substr(pos1 + 1, id1.size() - pos1 - 1);
	} else {
	  break;
	}
      }
      pos1++;
      pos2++;
    }
  }
  return "";
}

string findCommonCharsOfCorrectIDs(vector<string>& ids) {
  string res = findCommonCharsOfCorrectIDsForward(ids);
  if (res == "") {
    // If the forward pass fails, do a pass in reverse direction
    transform(ids.begin(), ids.end(), ids.begin(), [](string& s) {
	string res = s;
	reverse(s.begin(), s.end());
	return s;
     });
    res = findCommonCharsOfCorrectIDsForward(ids);
    reverse(res.begin(), res.end());
  }
  return res;
}


int main() {
  string id;
  vector<string> ids;
  while ((cin >> id)) {
    ids.push_back(id);
  }

  int res1 = countChecksum(ids);
  string res2 = findCommonCharsOfCorrectIDs(ids);
  cout << "Result 1: " << res1 << endl;
  cout << "Result 2: " << res2 << endl;					    
}
