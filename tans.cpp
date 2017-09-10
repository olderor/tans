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
  
  state() : res(0), mulres(1), mulsign(1) {}
  state(long double res, long double mulres, long double mulsign) :
  res(res), mulres(mulres), mulsign(mulsign) {}
  state(long double res, long double mulres, long double mulsign, char sign, int prev) :
  res(res), mulres(mulres), mulsign(mulsign), sign(sign), prev(prev) {}
  
  long double get() {
    return res + mulres * mulsign;
  }
};

vector<long double> tans;

vector< vector<state> > d;

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
}

long double getdif(const int i, const int j) {
  long double res = d[i][j].get();
  long double need = i;
  if (res > need) {
    return res - need;
  }
  return need - res;
}

int getbest(const int id) {
  int best = 0;
  for (int j = 1; j < d[id].size(); ++j) {
    if (getdif(id, best) > getdif(id, j)) {
      best = j;
    }
  }
  return best;
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
  const int size = 18;
  init(size);
  
  ofstream fout("output.txt");
  
  for (int i = 2; i < size; ++i) {
    fill(i);
    int best = getbest(i);
    string res = getansw(i, best);
    
    cout << i << " " << d[i][best].get() << " " << res << "\n";
    fout << i << " " << d[i][best].get() << " " << res << "\n";
  }
  
  fout.close();
  
  return 0;
}
