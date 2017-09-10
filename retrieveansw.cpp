#include <iostream>
#include <fstream>
#include <string>

using namespace std;

int main() {
    ifstream fin("res.txt");
    ofstream fout("temp.txt");
    int size;
    double dif;
    string answ;
    fin >> size >> dif >> answ;
    fout << '\"' << answ << '\"';
    while (fin >> size >> dif >> answ) {
        fout << ",\n\"" << answ << '\"';
    }
    fin.close();
    fout.close();
    return 0;
}
