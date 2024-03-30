#include "UGraphModel.h"
#include <fstream>
#include <time.h>
#include <iostream>
using namespace std;
bool intEQ(int &lhs, int &rhs) {
    return lhs==rhs;
}
string vertex2str(int & item) {
    return to_string(item);
}
int main(){
    int N = 50;
    int M = 1000000;
    UGraphModel<int> *model = new UGraphModel<int>(&intEQ, &vertex2str);
    ofstream scalefile("number_of_data.txt");
    ofstream timefile("time.txt");
    
    for (int i = 0; i < N; i++){
        for (int j = 0; j < M; j++)
            model->add(j);
        clock_t start = clock();
        cout<<model->size();
        clock_t end = clock();
        timefile << setprecision(15) << (float)(end - start)/CLOCKS_PER_SEC << " ";
        scalefile << i*M << " ";      
    }
}