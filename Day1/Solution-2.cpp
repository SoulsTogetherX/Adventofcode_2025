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

void process_line(string line, int &curr, int &code) {
    stringstream ss(line);
    char dir;
    int distance;

    ss >> dir >> distance;

    if (distance == 0) {
        return;
    }
    while(distance >= DIAL_MAX) {
        distance -= DIAL_MAX;
        code++;
    }

    if (dir == LEFT_DIR) {
        curr -= distance;
    }
    if (dir == RIGHT_DIR) {
        curr += distance;
    }

    if (curr == 0) {
        code++;
    }
    if (curr < 0) {
        code += (distance != abs(curr));
        curr += DIAL_MAX;
    }
    if (curr >= DIAL_MAX) {
        code += (distance != abs(curr));
        curr -= DIAL_MAX;
    }
    cout << (char)dir << " : " << distance << " : " << curr << " : " << code << endl;
}

int process_file() {
    string textline;
    ifstream inputStream;
    inputStream = open_file(FILE_NAME);

    int code = 0;
    int curr = 50;
    while(getline(inputStream, textline)) {
        process_line(textline, curr, code);
    }

    return code;
}

int main() {
    auto output = process_file();
    cout << "The secert code is " << output << endl;
}