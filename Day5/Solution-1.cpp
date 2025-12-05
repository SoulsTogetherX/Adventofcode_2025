#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

using namespace std;


struct IDRangeTable {
    struct range {
        size_t st, ed;

        range(size_t start, size_t end) {
            st = start;
            ed = end;
        }

        void include(size_t num) {
            st = min(st, num);
            ed = max(ed, num);
        }

        bool in_range(size_t num) {
            return (st <= num && num <= ed);
        }
    };

    vector<range> ranges;

    void add_range(size_t st, size_t ed) {
        //cout << "(" << st << ", " << ed << ")" << endl;
        for(auto r = ranges.begin(); r != ranges.end(); r++) {
            if (r->in_range(st)) {
                r->include(ed);
                return;
            } else if (r->in_range(ed)) {
                r->include(st);
                return;
            }
        }
        ranges.push_back(range(st, ed));
    }

    bool in_ranges(size_t num) {
        for(auto r = ranges.begin(); r != ranges.end(); r++) {
            if (r->in_range(num)) {
                return true;
            }
        }

        return false;
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

void process_range(IDRangeTable &table, string line) {
    stringstream ss(line);
    size_t st, ed;
    char chars;

    ss >> st >> chars >> ed;
    table.add_range(st, ed);

    //cout << "(" << st << ", " << ed << ")" << endl;
}

void process_ingredients(IDRangeTable &table, string line, size_t &sum) {
    stringstream ss(line);
    size_t num;

    ss >> num;
    sum += table.in_ranges(num);

    //cout << num << endl;
}

size_t process_file() {
    IDRangeTable table = IDRangeTable();
    string textline;
    ifstream inputStream;
    inputStream = open_file(FILE_NAME);

    size_t sum = 0;
    while(getline(inputStream, textline)) {
        if (textline.empty()) break;

        process_range(table, textline);
    }
    while(getline(inputStream, textline)) {
        process_ingredients(table, textline, sum);
    }

    return sum;
}

int main() {
    size_t output = process_file();
    cout << "The number of fresh IDs is " << output << endl;
}