#include <iostream>
#include <fstream>
#include <regex>
#include <cmath>

using namespace std;


const string FILE_NAME = "input.txt";
ifstream open_file(string filename) {
    ifstream inputStream;

    cout << endl;

    inputStream.open(filename);
    if (inputStream.fail()) {
        cout << "Failed to open file '" << filename << "'." << endl;
        exit(1);
    }
    if (!inputStream.is_open()) {
        cout << "File '" << filename << "' didn't open." << endl;
        exit(1);
    }

    cout << "File '" << filename << "' opened successfully." << endl;

    return inputStream;
}

void process_range(string range, size_t &sum) {
    stringstream ss(range);
    size_t st, ed;
    char chars;

    ss >> st >> chars >> ed;

    for(size_t i = st; i <= ed; i++) {
        size_t len = to_string(i).length();

        for(size_t j = 1; j <= len / 2; j++) {
            size_t ten_pow = pow(10, j);
            size_t mod_i = i % ten_pow;
            if (mod_i < (ten_pow / 10)) continue;

            size_t val = i;
            bool vaild = true;

            while(val /= ten_pow) {
                if ((val % ten_pow) == mod_i) continue;
                vaild = false;
                break;
            };

            if (vaild) {
                sum += i;
                break;
            }
        }
    }
}

void process_regex(string line, size_t &sum) {
    regex pattern("[1-9][0-9]*-[1-9][0-9]*");
    sregex_iterator begin(line.begin(), line.end(), pattern);
    sregex_iterator end;

    for (sregex_iterator i = begin; i != end; ++i) {
        smatch match = *i;
        process_range(match.str(), sum);
    }
}

size_t process_file() {
    string textline;
    ifstream inputStream;
    inputStream = open_file(FILE_NAME);
    
    size_t sum = 0;
    getline(inputStream, textline);
    process_regex(textline, sum);

    return sum;
}

int main() {
    size_t output = process_file();
    cout << "The sum of all invalid IDs is " << output << endl;
}