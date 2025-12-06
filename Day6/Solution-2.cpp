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

void process_numbers(vector<string> num_strs, vector<vector<int>> &numbers) {
    if (num_strs.empty()) return;

    bool number_found = true;
    char digit;
    int temp, feed_idx = 0, max_feed = num_strs[0].length();

    while(feed_idx < max_feed) {
        vector<int> number_col;

        while(number_found && feed_idx < max_feed) {
            number_found = false;
            temp = 0;

            for(auto str : num_strs) {
                digit = str[feed_idx];

                if (digit == ' ') continue;
                number_found = true;

                temp *= 10;
                temp += (digit - '0');
            }

            if (temp != 0) {
                //cout << temp << endl;
                number_col.push_back(temp);
            }
            feed_idx++;
        }
        
        number_found = true;
        numbers.push_back(number_col);
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
    vector<string> num_strs;
    string textline;
    ifstream inputStream;
    inputStream = open_file(FILE_NAME);

    while(getline(inputStream, textline)) {
        if (inputStream.peek() == EOF) {
            vector<vector<int>> numbers;
            process_numbers(num_strs, numbers);

            return process_operations(numbers, textline);
        }
        num_strs.push_back(textline);
    }

    return 0;
}

int main() {
    size_t output = process_file();
    cout << "The homework solution sum total is " << output << endl;
}