#include "bootcode.hpp"
#include "bootcode_impl.hpp"

#include<map>
#include<iostream>
#include<fstream>
#include<sstream>
#include<set>
#include<atomic>
#include<thread>

#ifndef N_THREADS
#define N_THREADS 4
#endif

using namespace std;
using namespace bootcode;

const map<opcode, function<void(processor<int>&, int)>> arch {
    {"nop", [](processor<int>& p, int arg){/*nothing**/}},
    {"acc", [](processor<int>& p, int arg){p.inc_acc(arg);}},
    {"jmp", [](processor<int>& p, int arg){p.inc_pc(arg - 1);}}
};

vector<string> split (const string &s, char delim) {
    vector<string> result;
    stringstream ss (s);
    string item;

    while (getline (ss, item, delim)) {
        result.push_back (item);
    }

    return result;
}



void read_input(string const& filename, vector<instruction<int>>& v) {
    string line;
    instruction<int> buffer;
    vector<string> parts;
    ifstream infile (filename);
    if(infile.is_open()){
        while( getline(infile, line)) {
            parts = split(line, ' ');
            buffer.opc = parts[0];
            buffer.arg = stoi(parts[1]);
            v.push_back(buffer);
        }
    } else {
        cerr << "File Error";
    }
    infile.close();
}

value solution1(const vector<instruction<int>>& vi) {
    set<address> visited;
    processor<int> p(arch,vi);
    while(!visited.count(p.get_pc())) {
        visited.insert(p.get_pc());
        p.step();
    }
    return p.get_acc();
}

bool switcheroo(instruction<int>& instr) {
    if(instr.opc.compare("jmp") == 0) {
        instr.opc = "nop";
    } else if(instr.opc.compare("nop") == 0) {
        instr.opc = "jmp";
    } else {
        return false;
    }
    return true;
}

void worker(const vector<instruction<int>>& vi, address start, address end, value& acc, atomic<bool>& complete) {
    processor<int> p(arch, vi);
    for(address addr = start; addr < end && !complete.load(); addr++) {
        instruction<int> old_instr = p.read_flash_mem(addr);
        auto new_instr = old_instr;
        if(switcheroo(new_instr)){
            p.write_instr_mem(addr, new_instr);
        } else {
            continue;
        }
        set<address> visited;
        while(!visited.count(p.get_pc())) {
            visited.insert(p.get_pc());
            bool finish = p.step();
            if(finish) break;
        }
        if(p.get_state() == processor<int>::status::FINISH) {
            cout << "SOLUTION FOUND AT $PC=" << addr << endl;
            acc = p.get_acc();
            complete.store(true);
        } else {
            p.reset();
            p.write_instr_mem(addr,old_instr);
        }
    }
}

value solution2(const vector<instruction<int>>& vi, uint max_threads = thread::hardware_concurrency()) {
    atomic<bool> complete(false);
    value result = -1;
    uint fragment = (int) (vi.size()/(max_threads + 1));
    vector<thread> threads;
    for(uint i = 0; i < max_threads; i++) {
        address range_start = fragment * i;
        address range_end = i + 1 < max_threads ? fragment * (i + 1) : vi.size();
        #ifdef DEBUG
        cout << "THREAD " << i << " WILL WORK IN RANGE( " << range_start << "," << range_end << ")\n";
        #endif
        threads.push_back(thread(worker, vi,range_start,range_end, ref(result), ref(complete)));
    }
    for(uint i = 0; i < max_threads; i++) {
        threads[i].join();
    }
    return result;
}




int main() {
    vector<instruction<int>> vi;
    read_input("day8.in",vi);
    value sol1 = solution1(vi);
    cout << "8.1 SOLUTION: " << sol1 << endl;
    value sol2 = solution2(vi, N_THREADS);
    cout << "8.2 SOLUTION: " << sol2 << endl;
}