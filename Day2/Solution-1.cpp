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
        int len = to_string(i).length();
        if (len % 2 == 1) continue;

        size_t ten_half_len = pow(10, len / 2);
        size_t mod_i = i % ten_half_len;

        if (mod_i == ((i - mod_i) / ten_half_len)) {
            sum += i;
            //cout << i << " : " << st << " : " << ed << " : " << sum << endl;
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