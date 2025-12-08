#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <unordered_map>

using namespace std;


const int CLOSEST_PAIR_LIMIT = 1000;
const int CIRCUIT_LIMIT = 3;


struct coordinate {
    int x, y, z;
    
    coordinate(int x_pos, int y_pos, int z_pos) {
        x = x_pos;
        y = y_pos;
        z = z_pos;
    }

    string to_string() {
        return "(" + std::to_string(x) + ", " + std::to_string(y) + ", " + std::to_string(z) + ")";
    }

    bool operator == (const coordinate& coord) const {
        return x == coord.x && y == coord.y && z == coord.z;
    }
};

struct connection {
    int breaker1, breaker2;
    size_t distance;
    
    connection(int breaker1_index, int breaker2_index, size_t distance_val) {
        breaker1 = breaker1_index;
        breaker2 = breaker2_index;
        distance = distance_val;
    }

    string to_string() {
        return "(" + std::to_string(breaker1) + ", " + std::to_string(breaker2) + ") " + std::to_string(distance);
    }

    bool operator == (const connection& coord) const {
        return breaker1 == coord.breaker2 && breaker2 == coord.breaker2;
    }
    bool operator < (const connection& coord) const {
        return distance < coord.distance;
    }
};


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

void process_line(vector<coordinate> &coordinates, string texline) {
    stringstream ss(texline);
    char chars;
    int x, y, z;

    ss >> x >> chars >> y >> chars >> z;
    coordinates.push_back(coordinate(x, y, z));
}

size_t distance_squared(coordinate c1, coordinate c2) {
    size_t dx = c2.x - c1.x;
    size_t dy = c2.y - c1.y;
    size_t dz = c2.z - c1.z;
    return dx*dx + dy*dy + dz*dz;
}

size_t process_circuits(vector<coordinate> coordinates, int pair_limit) {
    vector<connection> connections;

    // Calculates Distances
    for(int idx1 = 1; idx1 < coordinates.size(); idx1++) {
        for(int idx2 = 0; idx2 < idx1; idx2++) {
            connections.push_back(connection(idx1, idx2, distance_squared(coordinates[idx1], coordinates[idx2])));

            //cout << connections.back().to_string() << endl;
        }
    }

    // Get connections distances
    sort(connections.begin(), connections.end());
    pair_limit = (pair_limit < connections.size()) ? pair_limit : connections.size();

    //for(auto connect : connections) {
    //    cout << connect.to_string() << endl;
    //}

    unordered_map<int, int> memo;
    vector<vector<int>> circuits;
    for(int i = 0; i < pair_limit; i++) {
        auto pair = connections[i];
        int breaker1 = pair.breaker1, breaker2 = pair.breaker2;

        // breaker1 already found
        if (memo.find(breaker1) != memo.end()) {
            if (memo.find(breaker2) == memo.end()) {
                memo[breaker2] = memo[breaker1];
                circuits[memo[breaker1]].push_back(breaker2);
                continue;
            }
        }
        // breaker2 already found
        else if (memo.find(breaker2) != memo.end()) {
            if (memo.find(breaker1) == memo.end()) {
                memo[breaker1] = memo[breaker2];
                circuits[memo[breaker2]].push_back(breaker1);
                continue;
            }
        }
        // no breakers found
        else {
            memo[breaker1] = circuits.size();
            memo[breaker2] = circuits.size();

            vector<int> new_circuit;
            new_circuit.push_back(breaker1);
            new_circuit.push_back(breaker2);
            circuits.push_back(new_circuit);

            continue;
        }
        if (memo[breaker1] == memo[breaker2]) {
            continue;
        }

        int discarded_circuit_id = memo[breaker2];

        // Both breakers already found (Merger with bigger)
        for(int idx : circuits[memo[breaker2]]) {
            circuits[memo[breaker1]].push_back(idx);
            memo[idx] = memo[breaker1];
        }
        circuits[discarded_circuit_id] = vector<int>();
    }

    // Get circuit sizes
    vector<int> circuit_sizes;
    for(auto circuit : circuits) {
        if (!circuit.empty()) {
            circuit_sizes.push_back(circuit.size());
        }
    }

    // Sort circuit sizes
    sort(circuit_sizes.begin(), circuit_sizes.end(), greater<int>());

    // Sort multiply the sizes of the three largest circuits
    size_t mul_sum = 1;
    for(int i = 0; i < CIRCUIT_LIMIT; i++) {
        mul_sum *= circuit_sizes[i];
    }

    return mul_sum;
}

size_t process_file() {
    string textline;
    ifstream inputStream;
    inputStream = open_file(FILE_NAME);

    vector<coordinate> coordinates;
    while(getline(inputStream, textline)) {
        process_line(coordinates, textline);
    }

    return process_circuits(coordinates, CLOSEST_PAIR_LIMIT);
}

int main() {
    size_t output = process_file();
    cout << "The mul of the three largest circuits sizes is " << output << endl;
}