#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

const char LEFT_DIR = 'L';
const char RIGHT_DIR = 'R';
const int DIAL_MAX = 100;


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

int wrap(int num, int mod) {
    return (num + mod) % mod;
}

int process_line(string line, int curr) {
    stringstream ss(line);
    char dir;
    int distance;

    ss >> dir >> distance;

    if (dir == LEFT_DIR) {
        return wrap(curr - distance, DIAL_MAX);
    }
    if (dir == RIGHT_DIR) {
        return wrap(curr + distance, DIAL_MAX);
    }

    return curr;
}

int process_file() {
    string textline;
    ifstream inputStream;
    inputStream = open_file(FILE_NAME);

    int code = 0;
    int curr = 50;
    while(getline(inputStream, textline)) {
        curr = process_line(textline, curr);

        if (curr == 0) {
            code++;
        }
    }

    return code;
}

int main() {
    auto output = process_file();
    cout << "The secert code is " << output << endl;
}