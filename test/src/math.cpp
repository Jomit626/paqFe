
#include <iostream>
#include <vector>
using namespace std;

int main(){

    vector<int> predictions;
    for(int length = 0; length < 16; length++){
        int p = 2048 + length *(2048 / (length + 1));
        predictions.emplace_back(p);
    }
    for(int p : predictions){
        cout<<p << "\n";
    }
}