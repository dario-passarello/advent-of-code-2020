#include <string>
#include <fstream>
#include <iostream>
#include <regex>
#include <numeric>
#include <map>

using namespace std;


uint32_t calculate_or_mask(const string &s) {
    uint32_t mask = 0;
    for(char c : s){
        mask |= 1 << (c - 'a');
    }
    return mask;
}

uint32_t calculate_vector_or_mask(const vector<string> &vs) {
    uint32_t mask = 0;
    for(string s : vs) {
        mask |= calculate_or_mask(s);
    }
    return mask;
}

uint32_t calculate_vector_and_mask(const vector<string> &vs) {
    uint32_t mask = UINT32_MAX;
    for(string s : vs){
        mask &= calculate_or_mask(s);
    }
    return mask;
}

int count_ones(uint32_t x) {
    int count = 0;
    while(x) {
        count += x & 1; 
        x >>= 1; 
    }
    return count;
}

void read_input(string const& filename, vector<vector<string>>& v) {
    string line;
    vector<string> buffer;
    ifstream infile (filename);
    if(infile.is_open()){
        buffer = *new vector<string>();
        while( getline(infile, line)) {
            if(line.empty()) { //New passport incoming
                v.push_back(buffer);
                buffer = *new vector<string> ();
            } else { //Continue old passport
                buffer.push_back(line);
            }
        }
    } else {
        cerr << "File Error";
    }
    infile.close();
}

int main() {
    vector<vector<string>> vvs;
    read_input("day6.in",vvs);
    int sum1 = accumulate(vvs.begin(), vvs.end(), 0, [](int sum, vector<string> &sv) {
        uint32_t mask = calculate_vector_or_mask(sv);
        return sum + count_ones(mask);
    });
    int sum2 = accumulate(vvs.begin(), vvs.end(), 0, [](int sum, vector<string> &sv) {
        uint32_t mask = calculate_vector_and_mask(sv);
        return sum + count_ones(mask);
    });
    cout << "6.1 SOLUTION: " << sum1 << endl;
    cout << "6.2 SOLUTION: " << sum2 << endl;
}