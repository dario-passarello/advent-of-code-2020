#include<iostream>
#include<fstream>
#include<set>
#include<string>
#include<vector> 
#include<queue>
#include<numeric>

#define BACKTRACK 25

using namespace std;
using ull = unsigned long long int;
using ll = unsigned long long int;

void read_input(const string filename, vector<ll>& v) {
    string line;
    ifstream infile (filename);
    if(infile.is_open()){
        while( getline(infile, line)) {
            v.push_back(stoll(line));
        }
    } else {
        cerr << "File Error";
    }
    infile.close();
}

ll solution1(const vector<ll>& numbers) {
    set<ll> latest;
    queue<ll> fifo;
    //Add to set first BACKTRACK numbers
    for(int i = 0; i < BACKTRACK; i++) {
        latest.insert(numbers[i]);
        fifo.push(numbers[i]);
    }
    ll failing = -1;
    for(vector<ll>::const_iterator i = numbers.begin() + BACKTRACK; i != numbers.end(); i++) {
        bool found = false;
        for(set<ll>::iterator j = latest.begin(); j != latest.end(); ++j) {
            if(latest.count(*i - *j)) {
                found = true;
                break;
            }
        }
        if(!found) {
            failing = *i;
            break;
        } else {
            latest.erase(fifo.front());
            fifo.pop();
            latest.insert(*i);
            fifo.push(*i);
        }
    }
    return failing;
}

ll verify_sol(const vector<ll>& numbers, const pair<uint,uint>& sol) {
    ll count = 0;
    for(uint i = sol.first; i < sol.second; i++) {
        count += numbers[i];
    }
    return count;
}


pair<uint,uint> solution2(const vector<ll>& numbers, ll value) {
    queue<pair<uint,uint>> BFSqueue;
    set<pair<int,int>> explored;
    BFSqueue.push(make_pair(0,0));
    do {
        auto candidate = BFSqueue.front();
        //cout << candidate.first << "," << candidate.second << endl;
        if(verify_sol(numbers, candidate) == value) {
            return candidate;
        }
        if(candidate.first + 1 < numbers.size()) {
            auto p = make_pair(candidate.first + 1, candidate.second);
            if(!explored.count(p)) {
                explored.insert(p);
                BFSqueue.push(p);
            }

        }
        if(candidate.second + 1 < numbers.size()) {
            auto p = make_pair(candidate.first , candidate.second + 1);
            if(!explored.count(p)) {
                explored.insert(p);
                BFSqueue.push(p);
            }
        }
        BFSqueue.pop();
    } while(!BFSqueue.empty());
    cout << "UNLUCKY" << endl;
    fflush(stdout);
    throw 42;
}

pair<ll,ll> solution2_val(const vector<ll>& numbers, pair<uint,uint> value)  {
    ll min = INT64_MAX;
    ll max = 0;
    for(uint i = value.first; i < value.second; i++){
        ll curr = numbers[i];
        min = curr < min ? curr : min;
        max = curr > max ? curr : max;
    }
    return make_pair(min,max);
}

int main() {
    vector<ll> numbers;
    read_input("day9.in", numbers);
    ll sol1 = solution1(numbers);
    cout << "9.1 SOLUTION: " << sol1 << endl;
    pair<uint,uint> sol2 = solution2(numbers, sol1);
    pair<ll,ll> nsol2 = solution2_val(numbers, sol2);
    cout << sol2.first << " , " << sol2.second << endl;
    cout << verify_sol(numbers,sol2) << endl;
    cout << nsol2.first << " + " << nsol2.second << endl;
    cout << "9.2 SOLUTION: " << nsol2.first + nsol2.second << endl;
}