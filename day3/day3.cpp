#include<list>
#include<fstream>
#include<iostream>
#include<vector>
#include<string>
#include<algorithm>
#include<numeric>
#define GROUND '.'
#define TREE '#'
#define X_SLOPE 3
#define Y_SLOPE 1

using namespace std;

struct row;
struct forest;
struct point;

struct row {
    vector<int> trees;

    row(string);
};

struct point {
    unsigned int x;
    unsigned int y;
    unsigned int x_mod;
    point(unsigned int x1, unsigned int y1, unsigned int line_length) : x(x1), y(y1), x_mod(line_length) {};
    point& operator+(const point& other);
    point& operator=(const point& other) {
        return *new point(other.x,other.y,other.x_mod);
    }
    void operator+=(const point& other) {
        this -> x = (x + other.x) % x_mod;
        this -> y = y + other.y;
    }
};

struct forest {
    vector<row> rows;
    int line_length;

    forest(vector<row>& s, int l);
    point& f_point(int x, int y) {
        point& p =*new point(x, y, line_length);
        return p;
    }
};

row::row(string s) {
    for(unsigned int i = 0; i < s.length(); i++){
        if(s[i] == TREE){
            trees.push_back(i);
        }
    }
}

forest::forest(vector<row>& s, int l) {
    rows = s;
    line_length = l;
}

point& point::operator+(const point& other){
    cout << x << " " << y << endl;
    cout << other.x << " " << other.y << endl;
    return *new point((this -> x + other.x) % x_mod, this -> y + other.y, this -> x_mod);
}

int read_input(string const& filename, vector<row>& v) {
    string line;
    ifstream infile (filename);
    int ret_value = -1;
    if(infile.is_open()){
        while( getline(infile, line)) {
            ret_value = line.length();
            row& r = *new row(line);
            v.push_back(r);
        }
    } else {
        cerr << "File Error";
    }
    infile.close();
    return ret_value;
}

int count_trees(const point& slope, point& start, forest &f){
    int tree_count = 0;
    int down_count = 0;
    for(point pos = start;pos.y < f.rows.size(); pos += slope) {
        down_count++;
        if (find(f.rows[pos.y].trees.begin(), f.rows[pos.y].trees.end(), pos.x) != f.rows[pos.y].trees.end()) {
            tree_count++;
        }
        //cout << "("<<pos.x <<","<<pos.y << ") " << tree_count << endl;
    }
    //cout << down_count << " ";
    return tree_count;
}
int main() {
    vector<row> rv;
    int row_length = read_input("day3.in",rv);
    forest f = forest(rv, row_length);
    const point slope = f.f_point(X_SLOPE, Y_SLOPE);
    cout << slope.x <<" " << slope.y << " " << slope.x_mod << endl;
    cout << "3.1 Solution: " << count_trees(slope, f.f_point(0,0), f) << endl;
    point lst[] = {f.f_point(1,1), f.f_point(3,1), f.f_point(5,1), f.f_point(7,1), f.f_point(1,2)};
    unsigned long long int acc = 1;
    for(int i = 0; i < 5; i++){
        int v = count_trees(lst[i], f.f_point(0,0), f);
        cout << v << endl;
        acc = acc * v;
    }
    cout << "3.2 Solution: " << acc << endl;
}



