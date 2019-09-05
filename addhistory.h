#include "utils.h"
using namespace std;
vector<string> vec;
void adhistory(string history){
    vec.push_back(history);
}
string print_history(int i){
    return vec.at(i);
}