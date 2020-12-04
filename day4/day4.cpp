#include <string>
#include <fstream>
#include <iostream>
#include <regex>
#include <numeric>
#include <map>
using namespace std;

#define DEBUG


//It would be clearer if named capture groups were available in ECMAScript standard
//Screw javascript
regex validator(R"%(((?=(.|\n)*byr:(\S+)\s)(?=(.|\n)*iyr:(\S+)\s)(?=(.|\n)*eyr:(\S+)\s)(?=(.|\n)*hgt:(\S+)\s)(?=(.|\n)*hcl:(\S+)\s)(?=(.|\n)*ecl:(\S+)\s)(?=(.|\n)*pid:(\S+)\s)(((byr|iyr|eyr|hgt|hcl|ecl|pid|cid):(\S+))\s){7,8}))%");

bool is_valid_structure(const string& line,const regex& r);

void initialize_rules(map<int,regex>& field_rules) {
    //Maps capture group IDs to Regexs
    field_rules[3] = R"%(^(19[2-9][0-9])|(200[0-2])$)%"; //byr - CAPTURE GROUP ID 3
    field_rules[5] = R"%(^(201[0-9])|(2020)$)%"; ///iyr - CAPTURE GROUP ID 5
    field_rules[7] = R"%(^(202[0-9])|(2030)$)%"; //eyr - CAPTURE GROUP ID 7
    field_rules[9] = R"%(^(((1[5-8][0-9])|(19[0-3]))cm)|(((59)|(6[0-9])|(7[0-6]))in)$)%"; //hgt - CAPTURE GROUP ID 9
    field_rules[11] = R"%(^#[0-9a-f]{6}$)%"; //hcl - CAPTURE GROUP ID 11
    field_rules[13] = R"%(^((amb)|(blu)|(brn)|(gry)|(grn)|(hzl)|(oth))$)%"; //ecl - CAPTURE GROUP ID 13
    field_rules[15] = R"%(^[0-9]{9}$)%"; //pid - CAPTURE GROUP ID 15
}

bool is_valid_passport(const regex& general_rule, const map<int,regex>& field_rules, const string& passport) {
    //If the structure is invalid, the passport could not be valid
    if(!is_valid_structure(passport, validator)) return false; 
    //Match the valid structure
    smatch first_match = *sregex_iterator(passport.begin(), passport.end(), general_rule);
    bool ok = true;
    //Check all fields for their capture group IDs
    for(int i = 3; i < 16; i+=2) {
        //Match field rule, if fails then passport is invalid
        if(!regex_match(first_match[i].str(), field_rules.at(i))){
            ok = false;
            break;
        }
    }
    return ok;
}

bool is_valid_structure(const string& line,const regex& r) {
    //Simply match the validator
    if (regex_match(line,r)) {
        return true;
    } else {
        return false;
    }
}


void read_input(string const& filename, vector<string>& v) {
    string line, buffer;
    ifstream infile (filename);
    if(infile.is_open()){
        buffer = *new string("");
        while( getline(infile, line)) {
            if(line.empty()) { //New passport incoming
                v.push_back(buffer);
                buffer = *new string("");
            } else { //Continue old passport
                buffer.append(line);
                buffer.append("\n");
            }
        }
    } else {
        cerr << "File Error";
    }
    infile.close();
}


int main() {
    vector<string> passports;
    map<int,regex> rules; //Contains the field rules at the capturing group index
    read_input("day4_giammy_unix.in",passports);
    initialize_rules(rules);
    #ifdef DEBUG
    int x = accumulate(passports.begin(), passports.end(), 0, [](int count, string s){
        cout << s << "---" <<endl;
        return 0;
    });
    #endif //DEBUG
    int matches1 = accumulate(passports.begin(), passports.end(), 0, [](int count, string s){
        return count + (is_valid_structure(s, validator) ? 1 : 0);
    });
    int matches2 = accumulate(passports.begin(), passports.end(), 0, [rules](int count, string s){
        return count + (is_valid_passport(validator, rules, s) ? 1 : 0);
    });
    cout << "4.1 SOLUTION: " << matches1 << endl;
    cout << "4.2 SOLUTION: " << matches2 << endl;
}
