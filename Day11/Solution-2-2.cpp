#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <algorithm>

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

size_t count_paths_recursive(
    string curr,
    unordered_map<string, vector<string>> map,
    unordered_map<string, size_t> &memo
) {
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

size_t count_paths_helper(
    unordered_map<string, vector<string>> map,
    string start,
    string end,
    size_t end_value
) {
    unordered_map<string, size_t> memo;
    memo["out"] = 0;
    memo[end] = end_value;

    return count_paths_recursive(start, map, memo);
}

size_t count_paths(unordered_map<string, vector<string>> map) {
    size_t svr_to_dac = count_paths_helper(
        map,
        "svr",
        "dac",
        1
    );
    cout << "svr -> dac: " << svr_to_dac << endl;
    size_t dac_to_fft = (svr_to_dac == 0) ? 0 : count_paths_helper(
        map,
        "dac",
        "fft",
        svr_to_dac
    );
    cout << "dac -> fft: " << dac_to_fft << endl;
    size_t fft_to_out = (dac_to_fft == 0) ? 0 : count_paths_helper(
        map,
        "fft",
        "out",
        dac_to_fft
    );
    cout << "fft -> out: " << fft_to_out << endl << endl;

    size_t svr_to_fft = count_paths_helper(
        map,
        "svr",
        "fft",
        1
    );
    cout << "svr -> fft: " << svr_to_fft << endl;
    size_t fft_to_dac = (svr_to_fft == 0) ? 0 : count_paths_helper(
        map,
        "fft",
        "dac",
        svr_to_fft
    );
    cout << "fft -> dac: " << fft_to_dac << endl;
    size_t dac_to_out = (fft_to_dac == 0) ? 0 : count_paths_helper(
        map,
        "dac",
        "out",
        fft_to_dac
    );
    cout << "dac -> out: " << dac_to_out << endl;

    return fft_to_out + dac_to_out;
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