
#include "UGraphModel.h"
#include <fstream>
#include <time.h>
#include <iostream>
#include <math.h>
using namespace std;
bool intEQ(int &lhs, int &rhs) {
    return lhs==rhs;
}
string vertex2str(int & item) {
    return to_string(item);
}
int main(){
    int N = 50;
    int M = 100000;
    UGraphModel<int> *model = new UGraphModel<int>(&intEQ, &vertex2str);
    ofstream scalefile("number_of_data.txt");
    ofstream timefile("time.txt");
    for (int i = 0; i < N; i++){
        for (int j = 0; j < M; j++)
            model->add(j + i*M);
            int c = rand()%((i+1)*M);
            int d = rand()%((i+1)*M);
        clock_t start = clock();
            model->connected(c, d);
        clock_t end = clock();
        timefile << setprecision(15) << (float)(end - start)/CLOCKS_PER_SEC << " ";
        scalefile << i*M << " ";
    }
}