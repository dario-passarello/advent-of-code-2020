#include<list>
#include<fstream>
#include<iostream>
#include<vector>
#include<string>
#include<algorithm>
#include<numeric>

#define GROUND 0
#define TREE 1
#define GROUND_CH '.'
#define TREE_CH '#'



using namespace std;
typedef u_int32_t row_t;
struct point;

struct forest {
    vector<row_t> grid;
    unsigned int grid_xlength; 
    void add_row(const string&);
    bool is_tree(const point&) const;
    point& f_point(int x, int y) const;
    inline unsigned int grid_ylength() const {return grid.size();}
};

struct point {
    unsigned int x;
    unsigned int y;
    unsigned int x_mod;

    point(unsigned int x1, unsigned int y1, unsigned int x_mod1) : x(x1), y(y1), x_mod(x_mod1) {};
    void operator+=(const point& other);
};

void forest::add_row(const string& row_string) {
    row_t bin_row = 0;
    for(unsigned int i = 0; i < row_string.length(); i++){
        bin_row |= (row_string[i] == TREE_CH ? 1 : 0) << i;
    }
    grid.push_back(bin_row);
}

inline bool forest::is_tree(const point& p) const {
    return (grid[p.y] & 1 << p.x) != 0;
}

point& forest::f_point(int x, int y) const {
    point& p =*new point(x, y, grid_xlength);
    return p;
 }

void point::operator+=(const point& other) {
    this -> x = (x + other.x) % x_mod;
    this -> y = y + other.y;
}

void read_input(string const& filename, forest& f) {
    string line;
    ifstream infile (filename);
    if(infile.is_open()){
        while(getline(infile, line)) {
            f.add_row(line);
        }
        f.grid_xlength = line.length();
    } else {
        cerr << "File Error";
    }
    infile.close();
}

int count_trees(const point& slope, const point& start, const forest& f) {
    int tree_count = 0;
    for(point pos = start; pos.y < f.grid_ylength(); pos += slope) {
        if(f.is_tree(pos)) {
            tree_count++;
        }
    }
    return tree_count;
}

int main() {
    forest f;
    read_input("day3.in", f);
    cout << "DAY 3.1 SOLUTION: "<<count_trees(f.f_point(3,1), f.f_point(0,0), f) << endl << endl;
    
    vector<point> p = {f.f_point(1,1), f.f_point(3,1), f.f_point(5,1), f.f_point(7,1), f.f_point(1,2)};
    long long d32 = accumulate(p.begin(), p.end(), 1LL , [&f](long long acc, point slope)->long long {
        cout << "Slope (" << slope.x << "," << slope.y << "): ";
        int trees = count_trees(slope, f.f_point(0,0), f);
        cout << trees << " trees" << endl;
        return acc * trees;
    });
    cout <<"DAY 3.2 SOLUTION: " << d32 << endl;
}