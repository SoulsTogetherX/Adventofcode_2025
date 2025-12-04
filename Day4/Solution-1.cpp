#include <iostream>
#include <fstream>
#include <vector>

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

int process_shelf(vector<string> shelf) {
    int height = shelf.size();
    int width = shelf.back().length();
    int memo[height][width];

    for(int y = 0; y < height; y++) {
        for(int x = 0; x < width; x++) {
            memo[y][x] = 0;
        }
    }

    for(int y = 0; y < height; y++) {
        for(int x = 0; x < width; x++) {
            if (shelf[y][x] != '@') continue;

            for(int y2 = y - 1; y2 <= y + 1; y2++) {
                for(int x2 = x - 1; x2 <= x + 1; x2++) {
                    if (x2 == x && y2 == y) continue;
                    if (x2 < 0 || y2 < 0) continue;
                    if (x2 >= width || y2 >= height) continue;

                    memo[y2][x2] += 1;
                }
            }
        }
    }

    int sum = 0;
    for(int y = 0; y < height; y++) {
        for(int x = 0; x < width; x++) {
            if (shelf[y][x] != '@') continue;
            sum += (memo[y][x] < 4);
        }
    }
    return sum;
}

size_t process_file() {
    string textline;
    ifstream inputStream;
    inputStream = open_file(FILE_NAME);

    vector<string> shelf;
    while(getline(inputStream, textline)) {
        shelf.push_back(textline);
    }

    return process_shelf(shelf);
}

int main() {
    size_t output = process_file();
    cout << "The number of accessible rolls of paper is " << output << endl;
}