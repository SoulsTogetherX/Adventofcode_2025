#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

using namespace std;


const int PRESET_NUMBER = 6;


class Order {
    int length, height, orders[PRESET_NUMBER];

    public:
        Order(int length, int height, int orders[PRESET_NUMBER]) {
            this->length = length;
            this->height = height;

            for(int i = 0; i < PRESET_NUMBER; i++) {
                this->orders[i] = orders[i];
            }
        }

        vector<int> get_orders() {
            vector<int> v_orders;
            v_orders.reserve(PRESET_NUMBER);
            for(int i = 0; i < PRESET_NUMBER; i++) {
                v_orders.push_back(orders[i]);
            }

            return v_orders;
        }
        int get_length() {
            return length;
        }
        int get_height() {
            return length;
        }
};

class Present {
    vector<vector<bool>> shape;
    int tile_count;

    public:
        Present(vector<vector<bool>> shape) {
            this->shape = shape;

            tile_count = 0;
            for(auto row : shape) {
                for(auto tile : row) {
                    tile_count += tile;
                }
            }
        }

        vector<vector<bool>> get_shape() {
            return shape;
        }
        int get_tile_count() {
            return tile_count;
        }
        int get_length() {
            return shape[0].size();
        }
        int get_height() {
            return shape.size();
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

void print_present(vector<vector<bool>> presents) {
    for(auto row : presents) {
        for(auto tile : row) {
            cout << tile ? "#" : ".";
        }
        cout << endl;
    }
    cout << endl;
}

void parse_present(ifstream &inputStream, vector<Present> &presents) {
    vector<vector<bool>> present;
    string textline;
    int index;

    getline(inputStream, textline);
    stringstream ss(textline);

    ss >> index;

    while(getline(inputStream, textline)) {
        if (textline.empty()) break;

        vector<bool> row;
        for(char c : textline) {
            if (c == '#') {
                row.push_back(true);
            } else {
                row.push_back(false);
            }
        }
        present.push_back(row);
    }

    presents.push_back(Present(present));

    //cout << index << ":" << endl;
    //print_present(present);
}

void parse_order(string textline, vector<Order> &orders) {
    int length, height, temp, p_orders[PRESET_NUMBER];
    char chars;

    stringstream ss(textline);

    ss >> length >> chars >> height >> chars;

    for(int i = 0; i < PRESET_NUMBER; i++) {
        ss >> p_orders[i];
    }

    orders.push_back(Order(length, height, p_orders));

    //cout << length << "x" << height;
    //for(int i = 0; i < PRESET_NUMBER; i++) {
    //    cout << "\t" << p_orders[i];
    //}
    //cout << endl;
}

size_t process_orders(vector<Present> presents, vector<Order> orders) {
    size_t area, min_space, min_area_space;
    size_t invaild = 0, vaild = 0, unchecked_orders = 0;

    for(auto order : orders) {
        area = order.get_height() * order.get_length();

        min_space = 0;
        for(int i = 0; i < PRESET_NUMBER; i++) {
            min_space += order.get_orders()[i] * presents[i].get_tile_count();
            min_area_space += order.get_orders()[i] * presents[i].get_length() * presents[i].get_height();
        }

        if (min_space > area) {
            invaild++;
        } else if (area <= min_area_space) {
            vaild++;
        } else {
            unchecked_orders++;
        }
    }

    cout << "Auto Vaild: " << vaild << endl;
    cout << "Auto Invaild: " << invaild << endl;
    cout << "Unchecked: " << unchecked_orders << endl;

    return vaild;
}

size_t process_file() {
    string textline;
    ifstream inputStream;
    inputStream = open_file(FILE_NAME);

    vector<Present> presents;
    vector<Order> orders;

    for(int i = 0; i < PRESET_NUMBER; i++) {
        parse_present(inputStream, presents);
    }
    while(getline(inputStream, textline)) {
        parse_order(textline, orders);
    }

    return process_orders(presents, orders);
}

int main() {
    size_t output = process_file();
    cout << "The number of vaild regions is " << output << endl;
}