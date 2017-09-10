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
  
  state() : res(0), mulres(1), mulsign(1) {
  }
  
  state(long double res, long double mulres, long double mulsign) :
    res(res), mulres(mulres), mulsign(mulsign) {
  }
  
  long double get() {
    return res + mulres * mulsign;
  }
};

vector<long double> tans;

vector< vector<char> > answs;
vector<long double> bests;

void set_tans(const int size) {
  tans.resize(size);
  for (int i = 1; i < size; ++i) {
    tans[i] = tan(i);
  }
}

void init(const int size) {
  set_tans(size);
  bests.resize(size, -1);
  answs.resize(size);
  for (int i = 0; i < size; ++i) {
    answs[i].resize(i);
  }
}

long double getdif(const long double res, const int size) {
  long double need = size;
  if (res > need) {
    return res - need;
  }
  return need - res;
}

void setrandom(const int size) {
  int sign;
  long double prev;
  state cur = state(0, tans[1], 1);
  vector<char> answ;
  for (int i = 2; i < size; ++i) {
    sign = rand() % 3;
    if (sign == 0) {
      // plus
      prev = cur.get();
      cur.res = prev;
      cur.mulres = tans[i];
      cur.mulsign = 1;
      answ.push_back('+');
    } else if (sign == 1) {
      prev = cur.get();
      cur.res = prev;
      cur.mulres = tans[i];
      cur.mulsign = -1;
      answ.push_back('-');
    } else {
      cur.mulres *= tans[i];
      answ.push_back('*');
    }
    prev = getdif(cur.get(), i);
    if (prev < bests[i] || bests[i] == -1) {
      bests[i] = prev;
      for (int j = 0; j < i; ++j) {
        answs[i][j] = answ[j];
      }
    }
  }
}

string getansw(const int size) {
  if (size < 2) {
    return "";
  }
  
  stringstream ss;
  for (int i = 0; i < size; ++i) {
    ss << answs[size][i];
  }
  string res = ss.str();
  return res;
}

void setrandom(const int size, const int times) {
  for (int j = 0; j < times; ++j) {
    setrandom(size);
  }
}

int main() {
  srand(time(0));
  const int size = 366;
  init(size);
  for (int i = 0; i < 1000; ++i) {
    cout << "processing " << (i + 1) * 100000 << "\n";
    setrandom(size, 100000);
  }
  
  ofstream fout("output5.txt");
  
  for (int i = 2; i < size; ++i) {
    string res = getansw(i);
    
    cout << i << " " << bests[i] << " " << res << "\n";
    fout << i << " " << bests[i] << " " << res << "\n";
  }
  
  fout.close();
  
  return 0;
}