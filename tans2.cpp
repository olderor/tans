#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include <math.h>
#include <algorithm>
#include <iostream>

using namespace std;

struct state {
  long double res;
  long double mulres;
  long double mulsign;
  
  char sign;
  int prev;
  
  vector<int> next;
  
  state() : res(0), mulres(1), mulsign(1) {
    next.resize(3);
  }
  
  state(long double res, long double mulres, long double mulsign) :
    res(res), mulres(mulres), mulsign(mulsign) {
    
    next.resize(3);
  }
  
  state(long double res, long double mulres, long double mulsign, char sign, int prev) :
    res(res), mulres(mulres), mulsign(mulsign), sign(sign), prev(prev) {
    
    next.resize(3);
  }
  
  long double get() {
    return res + mulres * mulsign;
  }
};

vector<long double> tans;

vector< vector<state> > d;
vector<int> bests;

void set_tans(const int size) {
  tans.resize(size);
  for (int i = 1; i < size; ++i) {
    tans[i] = tan(i);
  }
}

void fill(const int size) {
  for (int i = 0; i < d[size - 1].size(); ++i) {
    state plus = state(
                       d[size - 1][i].get(),
                       tans[size],
                       1,
                       '+',
                       i);
    
    state minus = state(
                        d[size - 1][i].get(),
                        tans[size],
                        -1,
                        '-',
                        i);
    
    state mult = state(
                       d[size - 1][i].res,
                       d[size - 1][i].mulres * tans[size],
                       d[size - 1][i].mulsign,
                       '*',
                       i);
    
    d[size].push_back(plus);
    d[size].push_back(minus);
    d[size].push_back(mult);
  }
}

void init(const int size) {
  set_tans(size);
  d.resize(size);
  d[0].push_back(state());
  d[1].push_back(state(0, tans[1], 1));
  bests.resize(size);
}

long double getdif(const int i, const int j) {
  long double res = d[i][j].get();
  long double need = i;
  if (res > need) {
    return res - need;
  }
  return need - res;
}

void setrandom(const int size) {
  int prev = 0, sign;
  for (int i = 2; i < size; ++i) {
    sign = rand() % 3;
    if (d[i - 1][prev].next[sign] != 0) {
      prev = d[i - 1][prev].next[sign];
      continue;
    }
    d[i - 1][prev].next[sign] = d[i].size();
    if (sign == 0) {
      // plus
      state plus = state(
        d[i - 1][prev].get(),
        tans[i],
        1,
        '+',
        prev);
      d[i].push_back(plus);
    } else if (sign == 1) {
      state minus = state(
                          d[i - 1][prev].get(),
                          tans[i],
                          -1,
                          '-',
                          prev);
      d[i].push_back(minus);
    } else {
      state mult = state(
                         d[i - 1][prev].res,
                         d[i - 1][prev].mulres * tans[i],
                         d[i - 1][prev].mulsign,
                         '*',
                         prev);
      d[i].push_back(mult);
    }
    
    prev = d[i - 1][prev].next[sign];
    if (getdif(i, bests[i]) > getdif(i, prev)) {
      bests[i] = prev;
    }
  }
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

void setrandom(const int size, const int times) {
  for (int i = 2; i <= size; ++i) {
    cout << "processing " << i << "\n";
    for (int j = 0; j < times; ++j) {
      setrandom(i);
    }
  }
}

int main() {
  srand(time(0));
  const int size = 366;
  init(size);
  setrandom(size, 1000);
  
  ofstream fout("output2.txt");
  
  for (int i = 2; i < size; ++i) {
    string res = getansw(i, bests[i]);
    
    cout << i << " " << getdif(i, bests[i]) << " " << d[i][bests[i]].get() << " " << res << "\n";
    fout << i << " " << getdif(i, bests[i]) << " " << d[i][bests[i]].get() << " " << res << "\n";
  }
  
  fout.close();
  
  return 0;
}
