#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>

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

void parse_line(string textline, vector<vector<int>> &buttons, vector<int> &joltage) {
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

        vector<int> button;
        while(true) {
            ss >> temp >> chars;
            button.push_back(temp);

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

void print_stack(vector<int> stack) {
    cout << "(";
    for(auto num : stack) {
        cout << to_string(num) + ", ";
    }
    cout << ")" << endl;
}

void permutation(vector<int> joltage, vector<vector<int>> buttons, int button_idx, size_t depth, size_t &min_depth, vector<int> stack) {
    print_stack(stack);
    if (depth > min_depth) {
        return;
    }
    if (button_idx != -1) {
        auto button = buttons[button_idx];
        for(auto idx : button) {
            if (joltage[idx] == 0) return;
            joltage[idx]--;
        }

        bool completed = true;
        for(auto j : joltage) {
            if (j != 0) {
                completed = false;
                break;
            }
        }
        
        if (completed) {
            min_depth = (depth < min_depth) ? depth : min_depth;
            // print_stack(stack);
            return;
        }
    }

    for(int i = 0; i < buttons.size(); i++) {
        stack.push_back(i);
        permutation(joltage, buttons, i, depth + 1, min_depth, stack);
        stack.pop_back();
    }
}

void process_line(string textline, size_t &sum) {
    vector<vector<int>> buttons;
    vector<int> joltage;
    
    parse_line(textline, buttons, joltage);

    size_t min_depth = -1;
    permutation(joltage, buttons, -1, 0, min_depth, vector<int>());
    if (min_depth == -1) {
        cout << "No solution found for '" << textline << "'" << endl << endl;
    } else {
        cout << to_string(min_depth) << "-length solution found for '" << textline << "'" << endl << endl;
    }

    sum += min_depth;
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