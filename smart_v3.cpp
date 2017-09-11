#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <math.h>

using namespace std;

struct symb_state {
  char c;
  symb_state *prev = 0;
};

struct state {
  long double res;
  long double mulres;
  long double mulsign;
  
  symb_state *hist;
  
  state() : res(0), mulres(1), mulsign(1), hist(0) {
  }
  
  state(long double res, long double mulres, long double mulsign)
  : res(res), mulres(mulres), mulsign(mulsign), hist(0) {
  }
  
  state(long double res,
    long double mulres,
    long double mulsign,
    char symb,
    symb_state *h)
    : res(res), mulres(mulres), mulsign(mulsign) {
      hist = new symb_state();
      hist->prev = h;
      hist->c = symb;
  }

  long double get() {
    return res + mulres * mulsign;
  }
};

const int max_size = 151617;
vector< vector<state> > d[2];
vector<long double> tans;
vector<int> bests;
vector<int> bestsid;
vector<symb_state*> answs;

void set_tans(const int size) {
  tans.resize(size);
  for (int i = 1; i < size; ++i) {
    tans[i] = tan(i);
  }
}

long double getdif(const int id, const int size, const int j) {
  long double res = d[1][id][j].get();
  long double need = size;
  if (res > need) {
    return res - need;
  }
  return need - res;
}

long double getdif(const int i) {
  long double res = d[0][bestsid[i]][bests[i]].get();
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
    for (int i = 0; i < d[0][id].size(); ++i) {
      state plus = state(d[0][id][i].get(), tans[size], 1, '+', d[0][id][i].hist);
      state minus = state(d[0][id][i].get(), tans[size], -1, '-', d[0][id][i].hist);

      state mult = state(d[0][id][i].res,
        d[0][id][i].mulres * tans[size],
        d[0][id][i].mulsign,
        '*',
        d[0][id][i].hist);

      d[1][0].push_back(plus);
      d[1][1].push_back(minus);
      d[1][2].push_back(mult);
    }
  }
  for (int id = 0; id < 3; ++id) {
    sort(d[1][id].begin(), d[1][id].end(), cmp);
    for (int i = 0; i < d[1][id].size(); ++i) {
      if (bestsid[size] == -1 || getdif(bestsid[size], size, bests[size]) >
        getdif(id, size, i)) {
        bests[size] = i;
        bestsid[size] = id;
      }
    }
    if (d[1][id].size() > max_size) {;
      int step = d[1][id].size() / max_size;
      int offset = rand() % step;
      vector<state> states;
      bool ok = bestsid[size] != id;
      for (int i = 0; i < offset; ++i) {
        if (!(i == bests[size] && id == bestsid[size])) {
           delete d[1][id][i].hist;
        }
      }
      for (int i = offset; i < d[1][id].size(); ++i) {
        if ((i - offset) % step == 0) {
          states.push_back(d[1][id][i]);
          if (i == bests[size] && bestsid[size] == id) {
            ok = true;
            bests[size] = states.size() - 1;
          }
        } else if (!(i == bests[size] && id == bestsid[size])) {
           delete d[1][id][i].hist;
        }
      }
      if (!ok) {
        states.push_back(d[1][id][bests[size]]);
        bests[size] = states.size() - 1;
      }
      d[1][id] = states;
    }
  }
  answs[size] = d[1][bestsid[size]][bests[size]].hist;
  swap(d[0], d[1]);
  d[1].clear();
  d[1].resize(3);
}

void init(const int size) {
  set_tans(size);
  d[0].resize(3);
  d[1].resize(3);
  d[0][0].push_back(state(0, tans[1], 1));
  bests.resize(size);
  bestsid.resize(size, -1);
  answs.resize(size);
}

string getansw(const int size) {
  if (size < 2) {
    return "";
  }

  stringstream ss;
  symb_state *cur = answs[size];
  while (cur != 0) {
    ss << cur->c;
    cur = cur->prev;
  }
  string res = ss.str();
  reverse(res.begin(), res.end());
  return res;
}

int main() {
  srand(time(0));
  cout.precision(18);
  const int size = 366;
  init(size);

  ofstream fout("output45.txt");
  fout.precision(18);

  for (int i = 2; i < size; ++i) {
    fill(i);
    string res = getansw(i);
    cout << i << " " << fixed << getdif(i) << " " << res << "\n";
    fout << i << " " << fixed << getdif(i) << " " << res << "\n";
  }

  fout.close();

  return 0;
}
