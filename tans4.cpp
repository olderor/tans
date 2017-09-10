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

  state() : res(0), mulres(1), mulsign(1) {
  }
  state(long double res, long double mulres, long double mulsign)
    : res(res), mulres(mulres), mulsign(mulsign) {
  }
  state(long double res,
    long double mulres,
    long double mulsign,
    char sign,
    int prev)
    : res(res), mulres(mulres), mulsign(mulsign), sign(sign), prev(prev) {
  }

  long double get() {
    return res + mulres * mulsign;
  }
};

const int max_size = 300001;
vector< vector<state> > d;
vector<long double> tans;
vector<int> bests;

void set_tans(const int size) {
    tans.resize(size);
    for (int i = 1; i < size; ++i) {
        tans[i] = tan(i);
    }
}

long double getdif(const int i, const int j) {
  long double res = d[i][j].get();
  long double need = i;
  if (res > need) {
    return res - need;
  }
  return need - res;
}

void fill(const int size) {
  for (int i = 0; i < d[size - 1].size(); ++i) {
    state plus = state(d[size - 1][i].get(), tans[size], 1, '+', i);
    state minus = state(d[size - 1][i].get(), tans[size], -1, '-', i);

    state mult = state(d[size - 1][i].res,
      d[size - 1][i].mulres * tans[size],
      d[size - 1][i].mulsign,
      '*',
      i);

    d[size].push_back(plus);
    if (getdif(size, bests[size]) > getdif(size, d[size].size() - 1)) {
      bests[size] = d[size].size() - 1;
    }
    d[size].push_back(minus);
    if (getdif(size, bests[size]) > getdif(size, d[size].size() - 1)) {
      bests[size] = d[size].size() - 1;
    }
    d[size].push_back(mult);
    if (getdif(size, bests[size]) > getdif(size, d[size].size() - 1)) {
      bests[size] = d[size].size() - 1;
    }
  }

  if (d[size].size() > max_size) {
    int step = d[size].size() / max_size;
    vector<state> states;
    bool ok = false;
    for (int i = 0; i < d[size].size(); i += step) {
      states.push_back(d[size][i]);
      if (i == bests[size]) {
        ok = true;
      }
    }
    if (!ok) {
      states.push_back(d[size][bests[size]]);
      bests[size] = states.size() - 1;
    }
    d[size] = states;
  }
}

void init(const int size) {
  set_tans(size);
  d.resize(size);
  d[0].push_back(state());
  d[1].push_back(state(0, tans[1], 1));
  bests.resize(size);
}

string getansw(const int id, int best) {
  if (id < 2) {
    return "";
  }

  stringstream ss;
  for (int i = id; i > 1; --i) {
    ss << d[i][best].sign;
    best = d[i][best].prev;
  }
  string res = ss.str();
  reverse(res.begin(), res.end());
  return res;
}

int main() {
  const int size = 366;
  init(size);

  ofstream fout("output8.txt");

  for (int i = 2; i < size; ++i) {
    fill(i);
    string res = getansw(i, bests[i]);

    cout << i << " " << getdif(i, bests[i]) << " " << d[i][bests[i]].get() << " " << res << "\n";
    fout << i << " " << getdif(i, bests[i]) << " " << d[i][bests[i]].get() << " " << res << "\n";
  }

  fout.close();

  return 0;
}

