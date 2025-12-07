#include <iostream>
#include <fstream>
#include <vector>
#include <set>

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
    set<int> beams;
    size_t split_count = 0;

    beams.insert(grid[0].find('S'));
    
    for(int r = 1; r < grid.size(); r++) {
        string row = grid[r];
        set<int> new_beams;

        for (auto split = beams.begin(); split != beams.end(); split++) {
            if (row[*split] == '^') {
                new_beams.insert(*split - 1);
                new_beams.insert(*split + 1);

                split_count += 1;
            } else {
                new_beams.insert(*split);
            }
        }

        beams = new_beams;
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
    cout << "The number of splits is " << output << endl;
}