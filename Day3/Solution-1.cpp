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

int process_bank(string line) {
    char most_sig = 0;
    char least_sig = 0;
    byte batteries[line.length()];

    int j = 0;
    for(int i = 0; i < line.size() - 1; i++) {
        if (most_sig < line[i]) {
            most_sig = line[i];
            j = i + 1;
        }
    }
    for(; j < line.size(); j++) {
        least_sig = max(least_sig, line[j]);
    }

    most_sig -= '0';
    least_sig -= '0';
    //cout << (most_sig * 10) + least_sig << " : " << line << endl;

    return (most_sig * 10) + least_sig;
}

size_t process_file() {
    string textline;
    ifstream inputStream;
    inputStream = open_file(FILE_NAME);

    int joltage_sum = 0;
    while(getline(inputStream, textline)) {
        joltage_sum += process_bank(textline);
    }

    return joltage_sum;
}

int main() {
    size_t output = process_file();
    cout << "The total output joltage is " << output << endl;
}