#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <set>
#include <algorithm>
#include <cmath>

using namespace std;


class Pair {
    public:
        int val, idx;

        Pair() {val = 0; idx = 0;}
        Pair(int value, int index) {
            val = value;
            idx = index;
        }

        string to_string() const {
            return "Pair(" + std::to_string(val) + ", " + std::to_string(idx) + ")";
        }

        bool operator < (const Pair& p) const {
            return val < p.val;
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

void print_array(vector<vector<double>> &array) {
    int cols = array[0].size(), rows = array.size();

    for(int r = 0; r < rows; r++) {
        for(int c = 0; c < cols; c++) {
            cout << array[r][c] << "\t";
        }
        cout << endl;
    }
    cout << endl;
}

void simplify_array(vector<vector<double>> &array) {
    if (array.empty()) return;

    double factor;
    int cols = array[0].size(), rows = array.size();
    int c = 0, r = 0;
    bool skip_col;

    //print_array(array);
    while(c < cols && r < rows) {
        // swap row with the nearest row of nonzero value in col.
        skip_col = true;
        for(int i = r; i < rows; i++) {
            if (array[i][c] != 0) {
                swap(array[r], array[i]);
                skip_col = false;
                break;
            }
        }
        // If no nonzero value found, then we can't do anything more this col.
        if (skip_col) {
            c++;
            continue;
        }

        // Reduce col value to 1
        factor = array[r][c];
        for(int i = c; i < cols; i++) {
            array[r][i] /= factor;
        }

        // Ensure single one on col
        for(int i = 0; i < rows; i++) {
            if (i == r) continue;

            factor = array[i][c];

            for(int j = c; j < cols; j++) {
                array[i][j] -= array[r][j] * factor;
            }
        }

        r++;
        c++;
        //print_array(array);
    }
}

int minimize_free_variables(vector<vector<double>> &array) {
    int cols_m = array[0].size() - 1, rows = array.size();
    int addon_value, neg_pos_addon, temp;

    bool finished;

    vector<Pair> free_variables;
    set<int> free_indexes;
    
    int skip = 0;
    for(int c = 1; c < cols_m; c++) {
        for(int r = 0; r < c - skip && r < rows; r++) {
            if (array[r][c] != 0 && !free_indexes.count(c)) {
                skip++;

                int value = -1;
                for(int r2 = 0; r2 < rows; r2++) {
                    value += array[r2][c];
                }

                free_indexes.insert(c);
                free_variables.push_back(Pair(value, c));
            }
        }
    }

    sort(free_variables.begin(), free_variables.end());

    // Minimizes Free Variables
    for(auto var : free_variables) {
        if (var.val > 0) {
            finished = false;
            neg_pos_addon = 0;
            for(int r = 0; r < rows; r++) {
                if (array[r][cols_m] < array[r][var.idx]) {
                    finished = true;
                    break;
                }

                double val = array[r][var.idx];
                temp = 1;
                while(val > array[r][cols_m]) {
                    val -= array[r][cols_m];
                    temp++;
                }
                neg_pos_addon = (temp < neg_pos_addon) ? neg_pos_addon : temp;
            }
            if (finished) break;

            addon_value -= neg_pos_addon * var.val;
            break;
        }
    }

    // Removes Negatives
    for(int r = 0; r < rows; r++) {
        if (array[r][cols_m] < 0) {
            Pair free_var;
            for(auto var : free_variables) {
                if (array[r][var.idx] < 0) {
                    free_var = var;
                    break;
                }
            }

            double val = array[r][cols_m];
            temp = 0;
            while(val < 0) {
                val -= array[r][free_var.idx];
                temp++;
            }
            addon_value -= free_var.val * temp;
        }
    }

    return addon_value;
}

int get_buttons_required(vector<vector<double>> array) {
    if (array.empty()) return 0;

    int rows = array.size();
    int sum = minimize_free_variables(array);

    for(int r = 0; r < rows; r++) {
        sum += array[r].back();
    }

    return sum;
}

vector<vector<double>> convert_to_matrix(vector<set<int>> buttons, vector<int> joltage) {
    vector<vector<double>> matrix(joltage.size(), vector<double>());

    for(auto button : buttons) {
        for(int i = 0; i < matrix.size(); i++) {
            if (button.count(i) != 0) {
                matrix[i].push_back(1);
                continue;
            }
            matrix[i].push_back(0);
        }
    }


    for(int i = 0; i < matrix.size(); i++) {
        matrix[i].push_back(joltage[i]);
    }

    return matrix;
}

void parse_line(string textline, vector<set<int>> &buttons, vector<int> &joltage) {
    stringstream ss(textline);
    int temp;
    char chars;

    ss >> chars;
    while(true) {
        ss >> chars;
        if (chars == ']') {
            break;
        }
    }

    while(true) {
        ss >> chars;
        if (chars != '(') break;

        set<int> button;
        while(true) {
            ss >> temp >> chars;
            button.insert(temp);

            if (chars == ')') break;
        }
        buttons.push_back(button);
    }

    while(true) {
        ss >> temp >> chars;
        joltage.push_back(temp);
        if (chars == '}') break;
    }
}

size_t find_min_steps(vector<set<int>> buttons, vector<int> joltage) {
    auto matrix = convert_to_matrix(buttons, joltage);
    simplify_array(matrix);
    return get_buttons_required(matrix);
}

void process_line(string textline, size_t &sum) {
    vector<set<int>> buttons;
    vector<int> joltage;
    
    parse_line(textline, buttons, joltage);
    sum += find_min_steps(buttons, joltage);
}

size_t process_file() {
    string textline;
    ifstream inputStream;
    inputStream = open_file(FILE_NAME);

    size_t sum = 0;
    while(getline(inputStream, textline)) {
        process_line(textline, sum);
    }

    return sum;
}

int main() {
    size_t output = process_file();
    cout << "The fewest number of button presses is " << output << endl;
}