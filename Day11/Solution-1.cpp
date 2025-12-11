#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <unordered_map>

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


void parse_line(string textline, unordered_map<string, vector<string>> &map) {
    stringstream ss(textline);
    string key, temp;

    ss >> key;
    key.pop_back();

    while(ss >> temp) {
        map[key].push_back(temp);
    }
}

size_t count_paths_recursive(string curr, unordered_map<string, vector<string>> map, unordered_map<string, int> &memo) {
    if (memo.count(curr)) {
        return memo[curr];
    }

    size_t sum = 0;
    for(auto key : map[curr]) {
        sum += count_paths_recursive(key, map, memo);
    }
    memo[curr] = sum;
    return sum;
}

size_t count_paths(unordered_map<string, vector<string>> map) {
    unordered_map<string, int> memo;
    memo["out"] = 1;

    count_paths_recursive("you", map, memo);
    return memo["you"];
}


size_t process_file() {
    string textline;
    ifstream inputStream;
    inputStream = open_file(FILE_NAME);

    unordered_map<string, vector<string>> map;
    while(getline(inputStream, textline)) {
        parse_line(textline, map);
    }

    return count_paths(map);
}

int main() {
    size_t output = process_file();
    cout << "The number of paths is " << output << endl;
}