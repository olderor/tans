#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <math.h>

using namespace std;

struct state {
  long double res;
  long double mulres;
  long double mulsign;

  char sign;
  int prev;
  int previd;

  state() : res(0), mulres(1), mulsign(1) {
  }
  state(long double res, long double mulres, long double mulsign)
    : res(res), mulres(mulres), mulsign(mulsign) {
  }
  state(long double res,
    long double mulres,
    long double mulsign,
    char sign,
    int prev,
    int previd)
    : res(res), mulres(mulres), mulsign(mulsign), sign(sign), prev(prev),
      previd(previd) {
  }

  long double get() {
    return res + mulres * mulsign;
  }
};

const int max_size = 200000;
vector<vector<state> > d[3];
vector<long double> tans;
vector<int> bests;
vector<int> bestsid;

void set_tans(const int size) {
  tans.resize(size);
  for (int i = 1; i < size; ++i) {
    tans[i] = tan(i);
  }
}

long double getdif(const int id, const int i, const int j) {
  long double res = d[id][i][j].get();
  long double need = i;
  if (res > need) {
    return res - need;
  }
  return need - res;
}

bool cmp(state first, state second) {
  return first.get() < second.get();
}

void fill(const int size) {
  for (int id = 0; id < 3; ++id) {
    for (int i = 0; i < d[id][size - 1].size(); ++i) {
      state plus = state(d[id][size - 1][i].get(), tans[size], 1, '+', i, id);
      state minus = state(d[id][size - 1][i].get(), tans[size], -1, '-', i, id);

      state mult = state(d[id][size - 1][i].res,
        d[id][size - 1][i].mulres * tans[size],
        d[id][size - 1][i].mulsign,
        '*',
        i,
        id);

      d[0][size].push_back(plus);
      if (bestsid[size] == -1 || getdif(bestsid[size], size, bests[size]) >
        getdif(0, size, d[0][size].size() - 1)) {
        bests[size] = d[0][size].size() - 1;
        bestsid[size] = 0;
      }
      d[1][size].push_back(minus);
      if (getdif(bestsid[size], size, bests[size]) >
        getdif(1, size, d[1][size].size() - 1)) {
        bests[size] = d[1][size].size() - 1;
        bestsid[size] = 1;
      }
      d[2][size].push_back(mult);
      if (getdif(bestsid[size], size, bests[size]) >
        getdif(2, size, d[2][size].size() - 1)) {
        bests[size] = d[2][size].size() - 1;
        bestsid[size] = 2;
      }
    }
  }
  for (int id = 0; id < 3; ++id) {
    sort(d[id][size].begin(), d[id][size].end(), cmp);
    for (int i = 0; i < d[id][size].size(); ++i) {
      if (getdif(bestsid[size], size, bests[size]) >
        getdif(id, size, i)) {
        bests[size] = i;
        bestsid[size] = id;
      }
    }
    if (d[id][size].size() > max_size) {;
      int step = d[id][size].size() / max_size;
      vector<state> states;
      bool ok = false;
      for (int i = 0; i < d[id][size].size(); i += step) {
        states.push_back(d[id][size][i]);
        if (i == bests[size]) {
          ok = true;
        }
      }
      if (!ok) {
        states.push_back(d[id][size][bests[size]]);
        bests[size] = states.size() - 1;
      }
      d[id][size] = states;
    }
  }
}

void init(const int size) {
  set_tans(size);
  d[0].resize(size);
  d[1].resize(size);
  d[2].resize(size);
  d[0][1].push_back(state(0, tans[1], 1));
  bests.resize(size);
  bestsid.resize(size, -1);
}

string getansw(const int size) {
  if (size < 2) {
    return "";
  }

  stringstream ss;
  int previd = bestsid[size];
  int prev = bests[size];
  int t;
  for (int i = size; i > 1; --i) {
    ss << d[previd][i][prev].sign;
    t = prev;
    prev = d[previd][i][prev].prev;
    previd = d[previd][i][t].previd;
  }
  string res = ss.str();
  reverse(res.begin(), res.end());
  return res;
}

int main() {
  cout.precision(18);
  const int size = 366;
  init(size);

  ofstream fout("output12.txt");
  fout.precision(18);

  for (int i = 2; i < size; ++i) {
    fill(i);
    string res = getansw(i);
    cout << i << " " << fixed << getdif(bestsid[i], i, bests[i]) << " " << res << "\n";
    fout << i << " " << fixed << getdif(bestsid[i], i, bests[i]) << " " << res << "\n";
  }

  fout.close();

  return 0;
}
