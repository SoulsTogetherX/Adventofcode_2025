#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <algorithm>

using namespace std;

class Pair {
    public:
        int first, second;

        Pair() {
            first = 0;
            second = 0;
        }
        Pair(int first_val, int second_val) {
            first = first_val;
            second = second_val;
        }

        string to_string() const {
            return "Pair(" + std::to_string(first) + ", " + std::to_string(second) + ")";
        }

        bool operator == (const Pair& c) const {
            return first == c.first && second == c.second;
        }
};

class Rect {
    private:
        Pair c1, c2;
        size_t area;
    
    public:
        Rect() {area = 1;}
        Rect(Pair coord1, Pair coord2) {
            c1 = coord1;
            c2 = coord2;

            if (coord1.first > coord2.first) {
                swap(coord1.first, coord2.first);
            }
            if (coord1.second > coord2.second) {
                swap(coord1.second, coord2.second);
            }
            area = (coord2.first - coord1.first + 1) * (coord2.second - coord1.second + 1);
        }

        string to_string() const {
            return "Rect[" + c1.to_string() + ", " + c2.to_string() + "]";
        }

        size_t get_area() const {
            return area;
        }
 
        Pair get_c1() const {
            return c1;
        }
        Pair get_c2() const {
            return c2;
        }


        bool operator == (const Rect& r) const {
            return c1 == r.c1 && c2 == r.c2;
        }
        bool operator < (const Rect& rect) const {
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

Pair packed_sorted(int first, int second){
    if (first < second) {
        return Pair(first, second);
    }
    return Pair(second, first);
}

size_t process_coordinates(vector<Pair> coordinates) {
    size_t coord_num = coordinates.size();

    unordered_map<int, vector<Pair>> h_segments;
    unordered_map<int, vector<Pair>> v_segments;

    vector<Rect> rectanges;

    bool is_valid;

    for(int i = 0; i < coord_num; i++) {
        Pair c1 = coordinates[i];
        Pair c2 = coordinates[(i+1) % coord_num];

        if (c1.first == c2.first) {
            v_segments[c1.first].push_back(packed_sorted(c1.second, c2.second));
        } else {
            h_segments[c1.second].push_back(packed_sorted(c1.first, c2.first));
        }
    }

    for(int i = 0; i < coord_num - 1; i++) {
        for(int j = i + 1; j < coord_num; j++) {
            rectanges.push_back(Rect(coordinates[i], coordinates[j]));
        }
    }

    sort(rectanges.begin(), rectanges.end());

    for(int i = rectanges.size() - 1; i >= 0; i--) {
        auto rect = rectanges[i];

        Pair c1 = rect.get_c1();
        Pair c2 = rect.get_c2();

        Pair c_x = packed_sorted(c1.first, c2.first);
        Pair c_y = packed_sorted(c1.second, c2.second);

        is_valid = true;
        
        for(auto inc = v_segments.begin(); inc != v_segments.end(); inc++) {
            int x = inc->first;
            vector<Pair> segments = inc->second;

            if (!(c_x.first < x && x < c_x.second)) continue;

            for(auto seg : segments) {
                if (seg.second > c_y.first && seg.first < c_y.second) {
                    is_valid = false;
                    break;
                }
            }

            if (!is_valid) break;
        }

        for(auto inc = h_segments.begin(); inc != h_segments.end(); inc++) {
            int y = inc->first;
            vector<Pair> segments = inc->second;

            if (!(c_y.first < y && y < c_y.second)) continue;

            for(auto seg : segments) {
                if (seg.second > c_x.first && seg.first < c_x.second) {
                    is_valid = false;
                    break;
                }
            }

            if (!is_valid) break;
        }

        if (!is_valid) continue;

        cout << rect.to_string() << endl;
        return rect.get_area();
    }

    return 0;
}


Pair process_line(string textline) {
    stringstream ss(textline);
    int x, y;
    char chars;

    ss >> x >> chars >> y;
    return Pair(x, y);
}
size_t process_file() {
    string textline;
    ifstream inputStream;
    inputStream = open_file(FILE_NAME);

    vector<Pair> coordinates;

    while(getline(inputStream, textline)) {
        coordinates.push_back(process_line(textline));
    }
    return process_coordinates(coordinates);
}

int main() {
    size_t output = process_file();
    cout << "The largest rectangle area is " << output << endl;
}