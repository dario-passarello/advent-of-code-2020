#include <string>
#include <fstream>
#include <iostream>
#include <regex>
#include <numeric>
#include <map>

using namespace std;

#define N_BITS 10

inline char t_char(char a) {
    switch (a)
    {
    case 'F':
    case 'L':
        return 0;
    case 'B':
    case 'R':
        return 1;
    default:
        cout << "Malformed input" << endl;
        return -1;
    }
} 

int translate(const string& b) {
    int value = 0;
    for(int i = 0; i < N_BITS; i++) {
        value |= t_char(b[i]) << (N_BITS - i - 1);
    }
    return value;
}

void read_input(const string filename, vector<string>& v) {
    string line;
    ifstream infile (filename);
    if(infile.is_open()){
        while( getline(infile, line)) {
            v.push_back(line);
        }
    } else {
        cerr << "File Error";
    }
    infile.close();
}


int main() {
    vector<string> s_input;
    read_input("day5.in", s_input);
    vector<int> xs;
    xs.reserve(s_input.size()); //Avoid resizing
    transform(s_input.begin(), s_input.end(), back_inserter(xs), translate); //C++ functional is not so funny
    sort(xs.begin(), xs.end());
    int solution2;
    for(uint32_t i = 1; i < xs.size(); i++){
        if(xs[i] - xs[i - 1] > 1) {
            solution2 = (xs[i] + xs[i - 1])/2;
            break; 
        }
    }
    cout << "5.1 RESULT: " << xs.back() << endl;
    cout << "5.2 RESULT: " << solution2 << endl;
}