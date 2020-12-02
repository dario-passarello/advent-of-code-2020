#include<iostream>
#include<fstream>
#include<regex>
#include<string>
#include<vector>
#include<numeric>

//#define DEBUG

using namespace std;

struct pwdMatch{
    int num1;
    int num2;
    char sel_char;
    string password;

    pwdMatch(string line);
};

pwdMatch::pwdMatch(string line) {

    regex reg("(\\d+)-(\\d+) ([a-z]): ([a-z]+)");
    smatch first_match = *sregex_iterator(line.begin(), line.end(), reg);
    //Parse capture groups
    num1 = stoi(first_match[1].str());
    num2 = stoi(first_match[2].str());
    sel_char = first_match[3].str()[0];
    password = first_match[4].str();
}




bool isValidPassword1(pwdMatch& match) {
    int value = accumulate(match.password.begin(), match.password.end(), 0, 
        [&match](int count, char c) -> int {
            return count + (c == match.sel_char? 1 : 0);
        });
    return match.num1 <= value && value <= match.num2;
}

inline bool isValidPassword2(pwdMatch& match) {
    return (match.password[match.num1 - 1] == match.sel_char) != (match.password[match.num2 - 1] == match.sel_char);
}

void read_input(string const& filename, vector<pwdMatch>& v) {
    string line;
    ifstream infile (filename);
    if(infile.is_open()){
        while( getline(infile, line)) {
            pwdMatch &a = *new pwdMatch(line);
            v.push_back(a);
        }
    } else {
        cerr << "File Error";
    }
    infile.close();
}

int main() {
    vector<pwdMatch> sv;
    read_input("day2.in",sv);
    //Filters
    int first = accumulate(sv.begin(), sv.end(), 0, [](int count, pwdMatch &m)->int {
        return count + (isValidPassword1(m) ? 1 : 0);
        });
    int second = accumulate(sv.begin(), sv.end(), 0, [](int count, pwdMatch &m) -> int {
        return count + (isValidPassword2(m) ? 1 : 0);
        });
    cout << "SOLUTION 2.1: " << first  << endl;
    cout << "SOLUTION 2.2: " << second << endl;

    return EXIT_SUCCESS;
}

