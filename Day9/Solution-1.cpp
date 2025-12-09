#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>

using namespace std;


struct coordinate {
    int x, y;
    
    coordinate() {
        x = 0;
        y = 0;
    }
    coordinate(int x_pos, int y_pos) {
        x = x_pos;
        y = y_pos;
    }

    string to_string() const {
        return "(" + std::to_string(x) + ", " + std::to_string(y)+ ")";
    }

    size_t get_manhattan(const coordinate& coord) const {
        return abs(coord.x - x) + abs(coord.y - y);
    }
    size_t get_area(const coordinate& coord) const {
        size_t dx = abs(coord.x - x) + 1;
        size_t dy = abs(coord.y - y) + 1;
        return dx * dy;
    }

    bool operator == (const coordinate& coord) const {
        return x == coord.x && y == coord.y;
    }
    bool operator < (const coordinate& coord) const {
        return x + y < coord.x + coord.y;
    }
}; 


struct rect {
    coordinate c1, c2;
    size_t area;
    
    rect() {area = 1;}
    rect(coordinate coord1, coordinate coord2) {
        c1 = coord1;
        c2 = coord2;
        area = c1.get_area(c2);
    }

    string to_string() const {
        return "[" + c1.to_string() + ", " + c2.to_string() + "] " + std::to_string(get_area());
    }

    size_t get_manhattan() const {
        return c1.get_manhattan(c2);
    }
    size_t get_area() const {
        return area;
    }

    bool operator == (const rect& rect) const {
        return c1 == rect.c1 && c2 == rect.c2;
    }
    bool operator < (const rect& rect) const {
        return get_area() < rect.get_area();
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

coordinate process_line(string textline) {
    stringstream ss(textline);
    int x, y;
    char chars;

    ss >> x >> chars >> y;
    return coordinate(x, y);
}

rect process_coordinates(vector<coordinate> coordinates) {
    vector<rect> rectangles;

    for(int idx1 = 0; idx1 < coordinates.size(); idx1++) {
        for(int idx2 = idx1 + 1; idx2 < coordinates.size(); idx2++) {
            rectangles.push_back(rect(coordinates[idx1], coordinates[idx2]));
        }
    }

    sort(rectangles.begin(), rectangles.end());
    //for(auto rect : rectangles) {
    //    cout << rect.to_string() << endl;
    //}
    
    return rectangles.back();
}

size_t process_file() {
    string textline;
    ifstream inputStream;
    inputStream = open_file(FILE_NAME);

    vector<coordinate> coordinates;

    while(getline(inputStream, textline)) {
        coordinates.push_back(process_line(textline));
    }
    return process_coordinates(coordinates).get_area();
}

int main() {
    size_t output = process_file();
    cout << "The largest rectangle area is " << output << endl;
}