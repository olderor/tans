#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

vector<string> answers;
vector<long double> difs;
vector<int> sources;
int source = 1;

void read1(string filename, bool clear = false) {
    ifstream fin(filename);
    int size;
    long double dif, res;
    string answ;
    while (fin >> size >> dif >> res >> answ) {
        cout << dif << "\n";
        if (clear || dif < difs[size]) {
            answers[size] = answ;
            difs[size] = dif;
            sources[size] = source;
        }
    }
    fin.close();
}

void read2(string filename, bool clear = false) {
    ifstream fin(filename);
    int size;
    long double dif;
    string answ;
    while (fin >> size >> dif >> answ) {
        if (clear || dif < difs[size]) {
            while (answ.length() > size - 1) {
                answ.pop_back();
            }
            answers[size] = answ;
            difs[size] = dif;
            sources[size] = source;
        }
    }
    fin.close();
}

int main() {
    
    answers.resize(366);
    difs.resize(366);
    sources.resize(366);


  read2("output12.txt", true); ++source;    //1
  read2("output11.txt"); ++source;          //2
  read2("output10.txt"); ++source;          //3
  
  ofstream fout("res.txt");
  fout.precision(18);
  cout.precision(18);
  
  for (int i = 2; i < 366; ++i) {
    cout << sources[i] << " " << i << " " << fixed << difs[i] << " " << answers[i] << "\n";
    fout << i << " " << fixed << difs[i] << " " << answers[i] << "\n";
  }
  
  fout.close();
  
  return 0;
}
