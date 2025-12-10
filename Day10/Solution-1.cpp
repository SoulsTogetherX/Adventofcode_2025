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

void parse_line(string textline, size_t &display, vector<size_t> &buttons, vector<int> &other) {
    stringstream ss(textline);
    int temp, display_size = 0;
    char chars;

    display = 0;

    ss >> chars;
    while(true) {
        ss >> chars;
        if (chars == '.') {
            display <<= 1;
        } else if (chars == '#') {
            display <<= 1;
            display |= 1;
        } else {
            break;
        }
        display_size++;
    }

    while(true) {
        ss >> chars;
        if (chars != '(') break;

        size_t button = 0;
        while(true) {
            ss >> temp >> chars;
            button |= (1 << (display_size - temp - 1));

            if (chars == ')') break;
        }
        buttons.push_back(button);
    }

    while(true) {
        ss >> temp >> chars;
        other.push_back(temp);
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

void permutation(size_t display, vector<size_t> buttons, int button_idx, size_t depth, size_t &min_depth, vector<int> stack) {
    if (depth > min_depth) {
        return;
    }
    if (button_idx != -1) {
        display ^= buttons[button_idx];
        
        if (display == 0) {
            min_depth = (depth < min_depth) ? depth : min_depth;
            // print_stack(stack);
            return;
        }
    }

    for(int i = button_idx + 1; i < buttons.size(); i++) {
        stack.push_back(i);
        permutation(display, buttons, i, depth + 1, min_depth, stack);
        stack.pop_back();
    }
}

void process_line(string textline, size_t &sum) {
    size_t display;
    vector<size_t> buttons;
    vector<int> other;
    
    parse_line(textline, display, buttons, other);

    size_t min_depth = -1;
    permutation(display, buttons, -1, 0, min_depth, vector<int>());
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