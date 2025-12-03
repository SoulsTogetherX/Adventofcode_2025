#include <iostream>
#include <fstream>
#include <regex>
#include <cmath>

using namespace std;

const int BATTERY_NUM = 12;


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

size_t process_bank(string line) {
    size_t joltage = 0;
    byte batteries[line.length()];

    int pos = 0;
    for(int i = BATTERY_NUM - 1; i >= 0; i--) {
        int digit = 0;
        for(int j = pos; j < line.size() - i; j++) {
            if (digit < line[j]) {
                digit = line[j];
                pos = j + 1;
            }
        }

        joltage *= 10;
        joltage += (digit - '0');
    }

    //cout << joltage << " : " << line << endl;
    return joltage;
}

size_t process_file() {
    string textline;
    ifstream inputStream;
    inputStream = open_file(FILE_NAME);

    size_t joltage_sum = 0;
    while(getline(inputStream, textline)) {
        joltage_sum += process_bank(textline);
    }

    return joltage_sum;
}

int main() {
    size_t output = process_file();
    cout << "The total output joltage is " << output << endl;
}