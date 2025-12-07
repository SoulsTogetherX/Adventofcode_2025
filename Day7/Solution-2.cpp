#include <iostream>
#include <fstream>
#include <unordered_map>
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

size_t process_grid(vector<string> grid) {
    unordered_map<int, size_t> beams;
    size_t split_count = 0;

    beams[grid[0].find('S')] = 1;
    
    for(int r = 1; r < grid.size(); r++) {
        unordered_map<int, size_t> old_beams(beams);
        string row = grid[r];

        for (const auto& pair : old_beams) {
            if (pair.second == 0) continue;
            cout << "(" << pair.first << ", " << pair.second << ")" << endl;

            if (row[pair.first] == '^') {
                beams[pair.first - 1] += beams[pair.first];
                beams[pair.first + 1] += beams[pair.first];
                beams[pair.first] = 0;
            }
        }

        cout << endl;
    }

    for (const auto& pair : beams) {
        split_count += pair.second;
    }

    return split_count;
}

size_t process_file() {
    string textline;
    ifstream inputStream;
    inputStream = open_file(FILE_NAME);

    vector<string> grid;
    while(getline(inputStream, textline)) {
        grid.push_back(textline);
    }

    return process_grid(grid);
}

int main() {
    size_t output = process_file();
    cout << "The number of timeline splits is " << output << endl;
}