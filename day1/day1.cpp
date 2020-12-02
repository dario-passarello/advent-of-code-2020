#include <iostream>
#include<fstream>
#include<vector>
#include<algorithm>
#include<string>
#define TARGET 2020

using namespace std;

void read_input(const string filename, vector<int>& v);
bool search_complementary(vector<int>::iterator begin, vector<int>::iterator end, int value, int &result);

int main() {
    vector<int> v;
    read_input("day1.in", v);
    sort(v.begin(), v.end());
    int value;
    if(search_complementary(v.begin(), v.end(), TARGET, value)){
        cout << "DAY 1.1 SOLUTION: " << value*(TARGET - value) << endl;
    } else {
        cout << "DAY 1.1 SOLUTION Not Found, check Input!" << endl;
    }   
    for(vector<int>::iterator it = v.begin(); it != v.end(); it++){
        if(search_complementary(it, v.end(), TARGET - *it, value)) {
            cout << "DAY 1.2 SOLUTION: " << *it * (TARGET - *it - value) * value << endl;
            break;
        }
    }
    return EXIT_SUCCESS;
}

void read_input(const string filename, vector<int>& v) {
    string line;
    ifstream infile ("day1.in");
    if(infile.is_open()){
        while( getline(infile, line)) {
            string t;
            infile >> t;
            int value = stoi(t, nullptr);
            v.push_back(value);
        }
    } else {
        cerr << "File Error";
    }
    infile.close();
}

bool search_complementary(vector<int>::iterator begin, vector<int>::iterator end, int value, int &result){
    for(vector<int>::iterator it = begin; it != end; ++it){
        if (binary_search(it + 1, end, value - *it)) {
            result = *it;
            return true;
        }
    }
    return false;
}

