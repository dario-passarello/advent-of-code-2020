#include <string>
#include<sstream>
#include <fstream>
#include <iostream>
#include <numeric>
#include <map>
#include <set>
#include <vector>
#include <algorithm>
using namespace std;

#define DEBUG



vector<string> split (const string &s, char delim) {
    vector<string> result;
    stringstream ss (s);
    string item;

    while (getline (ss, item, delim)) {
        result.push_back (item);
    }

    return result;
}


struct bag {
    string color;

    map<string, int> contained_in;
    map<string, int> contains;
    

    bag(string color) : color(color) {};
    void add_sub_bag(bag& other, int qty) {
        //Create bidirectional link in order to save time
        contains.emplace(other.color, qty);
        other.contained_in.emplace(color, qty);
    }        
};

void read_input(string const& filename, vector<string>& v) {
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

pair<string, bag> parse_rule_bags(string rule) {
    vector<string> rule_tokens = split(rule, ' ');
    string bag_color = rule_tokens[0] + ' ' + rule_tokens[1];
    bag new_bag = *new bag(bag_color);
    return make_pair(bag_color, new_bag);
}


void create_DAG(vector<string>& rules, map<string, bag>& nodes) {
    for(string& rule : rules) { //Iterates all rules
        vector<string> rule_tokens = split(rule, ' ');
        if(rule_tokens.size() == 7) { //Only rules that contain no other bag have length 7
            continue;
        }
        string container_color = rule_tokens[0] + ' ' + rule_tokens[1]; //First two words in rule are the color for which this rule applies
        bag& container = nodes.at(container_color);
        //Match all contained
        //If there are N contained bag for i = 0..N-1
        //position 4 + N*4 -> Number of bags contained
        //position 4 + (N*4) + 1 -> Color adjective
        //position 4 + (N*4) + 2 -> Color Noun
        for(unsigned int pos = 4; pos < rule_tokens.size(); pos += 4) {
            int qty = stoi(rule_tokens[pos]);
            string contained_color = rule_tokens[pos + 1] + ' ' + rule_tokens[pos + 2];
            bag& contained = nodes.at(contained_color);
            container.add_sub_bag(contained, qty); //Create new "bidirectional" edge
        }
    }
}

//Backtracking with bags -> Bagtracking
//Insert bags containing the target in bags set
void bag_tracking(bag& start_node, map<string, bag>& nodes, set<string>& bags) {
    map<string, int>& contained_in = start_node.contained_in;
    bags.insert(start_node.color);
    for(map<string, int>::iterator it = contained_in.begin(); it != contained_in.end(); ++it) {
        bag_tracking(nodes.at(it -> first), nodes, bags);
    }
    return;
}

long long int bag_inside(bag& start_node, map<string, bag>& nodes) {
    map<string, int>& contains = start_node.contains;
    if(contains.size() == 0) {
        return 0LL;
    }
    long long int count = 0;
    for(map<string, int>::iterator it = contains.begin(); it != contains.end(); ++it) {
        count += ((long long int )(it -> second)) * (bag_inside(nodes.at(it -> first), nodes) + 1);
    }
    return count;
}

int main() {
    vector<string> rules;     //Contains of all rules
    map<string, bag> nodes;   //Contains the DAG
    set<string> bags;         //Contains the bags containing "Shiny gold" (included)

    read_input("day7.in", rules); //Create Rule List
    transform(rules.begin(), rules.end(), inserter(nodes, nodes.end()), parse_rule_bags); //Creates DAG Nodes
    create_DAG(rules, nodes); //Create DAG Edges

    bag& target = nodes.at("shiny gold");

    bag_tracking(target, nodes, bags); //Starts from "Shony Gold" and "bagtrack"
    cout << "7.1 SOLUTION: " << bags.size() - 1 << endl;
    cout << "7.2 SOLUTION: " << bag_inside(target, nodes) << endl;

}