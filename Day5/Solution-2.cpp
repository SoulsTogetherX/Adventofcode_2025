#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>

using namespace std;


struct IDRangeTable {
    private:
        struct range {
            size_t st, ed;

            range(size_t start, size_t end) {
                if (start > end) swap(start, end);

                st = start;
                ed = end;
            }

            void include(size_t num) {
                st = min(st, num);
                ed = max(ed, num);
            }
            bool intersect(range ran) {
                return (st <= ran.st && ran.st <= ed) || (st <= ran.ed && ran.st <= ed);
            }
            void merge(range ran) {
                st = min(st, ran.st);
                ed = max(ed, ran.ed);
            }

            bool in_range(size_t num) {
                return (st <= num && num <= ed);
            }

            size_t length() {
                return ed - st + 1;
            }

            bool operator < (range r) {
                return st < r.st || (st == r.st && ed < r.ed);
            }
            bool operator > (range r) {
                return st > r.st || (st == r.st && ed > r.ed);
            }

            bool operator <= (range r) {
                return st <= r.st || (st == r.st && ed <= r.ed);
            }
            bool operator >= (range r) {
                return st >= r.st || (st == r.st && ed >= r.ed);
            }

            bool operator == (range r) {
                return st == r.st && ed == r.ed;
            }
            bool operator != (range r) {
                return st != r.st || ed != r.ed;
            }
        };

        vector<range> ranges;

        void defragment_ranges() {
            sort(ranges.begin(), ranges.end());
            vector<range> new_ranges;
            new_ranges.reserve(ranges.size());

            for(auto r = ranges.begin(); r < ranges.end(); r++) {
                auto nr = r + 1;

                if (nr != ranges.end() && r->intersect(*nr)) {
                    r->merge(*nr);
                    new_ranges.push_back(*r);

                    r++;
                } else {
                    new_ranges.push_back(*r);
                }
            }

            ranges = new_ranges;
        }

    public:
        void add_range(size_t st, size_t ed) {
            //cout << "add (" << st << ", " << ed << ")" << endl;
            for(auto r = ranges.begin(); r != ranges.end(); r++) {
                if (r->in_range(st)) {
                    r->include(ed);
                    defragment_ranges();
                    return;
                } else if (r->in_range(ed)) {
                    r->include(st);
                    defragment_ranges();
                    return;
                }
            }

            ranges.push_back(range(st, ed));
            defragment_ranges();
        }

        bool in_ranges(size_t num) {
            for(auto r = ranges.begin(); r != ranges.end(); r++) {
                if (r->in_range(num)) {
                    return true;
                }
            }

            return false;
        }

        size_t total_ids() {
            size_t sum = 0;
            
            for(auto r = ranges.begin(); r != ranges.end(); r++) {
                cout << "(" << r->st << ", " << r->ed << ")" << endl;
                sum += r->length();
            }

            return sum;
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

    while(getline(inputStream, textline)) {
        if (textline.empty()) break;

        process_range(table, textline);
    }
    
    return table.total_ids();
}

int main() {
    size_t output = process_file();
    cout << "The number of fresh IDs is " << output << endl;
}