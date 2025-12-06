#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>

using namespace std;


const char MULTIPLY_OP = '*';
const char ADDITION_OP = '+';


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

void process_numbers(vector<vector<int>> &nums, string textline) {
    stringstream ss(textline);
    int idx = 0;
    int num;

    while(ss >> num) {
        if (nums.size() <= idx) {
            nums.push_back(vector<int>(1, num));
        } else {
            nums[idx].push_back(num);
        }

        idx++;
    }
}

size_t process_operations(vector<vector<int>> &nums, string textline) {
    stringstream ss(textline);
    size_t sum = 0, temp;
    int idx = 0;
    char op;

    while(ss >> op) {
        vector<int> op_nums = nums[idx];
        temp = op_nums[0];

        for(auto num = op_nums.begin() + 1; num != op_nums.end(); num++) {
            //cout << temp << " " << op << " " << *num << endl;
            if (op == MULTIPLY_OP) {
                temp *= (*num);
            } else if (op == ADDITION_OP) {
                temp += (*num);
            }
        }

        //cout << temp << endl;
        //cout << endl;
        //cout << endl;

        sum += temp;
        idx++;
    }

    return sum;
}

size_t process_file() {
    vector<vector<int>> nums;
    string textline;
    ifstream inputStream;
    inputStream = open_file(FILE_NAME);

    while(getline(inputStream, textline)) {
        if (inputStream.peek() == EOF) { 
            return process_operations(nums, textline);
        }
        process_numbers(nums, textline);
    }

    return 0;
}

int main() {
    size_t output = process_file();
    cout << "The homework solution sum total is " << output << endl;
}