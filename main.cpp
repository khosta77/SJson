#include <iostream>
#include <vector>

#include "SJson.h"

using namespace std;

void read_string(vector<string> &name, const char *str, size_t &i) {
    name.resize(name.size() + 1);
    for (size_t j = i + 1;; j++) {
        if (str[j] != '\"') {
            name[name.size() - 1] += str[j];
        } else {
            i = j;
            break;
        }
    }
}

double read_num(const char *str, size_t &i) {
    double num;  // Число
    bool plus = false;  // Положительное число или отрицательное
    if (str[i] == '-') {
        plus = true;
    }
    bool integer = true;  // Целое или не целое число
    bool first = true;  // Первая цифра
    int trigger = 1;  // Счетчик, если число не целое
    for (size_t j = i;; j++) {
        if (str[j] == ',' || str[j] == '}' || str[j] == ' ' || str[j] == '\n') {
            if (plus) {
                num = num * (-1);
            }
            i = j;
            break;
        }
        if ((str[j] >= 48) && (str[j] <= 57)) {
            if (first) {
                num += (static_cast<int>(int(str[j])) - '0');
                first = false;
            } else {
                if (integer) {
                    num *= 10;
                    num += (static_cast<int>(int(str[j])) - '0');
                } else {
                    double buf = double((static_cast<int>(int(str[j])) - '0'));
                    for (size_t k = 0; k < trigger; k++) {
                        buf /= 10;
                    }
                    num += buf;
                    trigger++;
                }
            }
        } else if (str[j] == '.') {
            integer = false;
        }
    }
    return num;
}

void read_array(vector<double> &arr, const char *str, size_t &i) {
    for (size_t j = i + 1;; j++) {
        if (str[j] == ']') {
            i = j;
            break;
        }
        if (str[j] == ' ' || str[j] == '\n') {
            continue;
        }
        arr.push_back(read_num( str, j));
    }
}

int main(){
    const char *str = "\"test1\" : [\n"
                      "0.98, -1.257, -4, -15\n"
                      "], \n "
                      "\"Array\" : 8 ";
    vector<double> arr;
    vector<double> num;
    vector<string> name;
    for (size_t i = 0; str[i] != '\0'; i++) {
        if (str[i] == '\"') {
            read_string(name, str, i);
        } else if (str[i] == '[') {
            size_t m = i;
            read_array(arr, str, i);
            cout << m << " " << i << endl;
        } else if (((str[i] >= 48) && (str[i] <= 57)) || (str[i] == '-')) {
            num.push_back(read_num( str, i));
        }
    }

    for (size_t i = 0; i < name.size(); i++) {
        cout << name[i] << " ";
    }
    cout << endl;
    for (size_t i = 0; i < arr.size(); i++) {
        cout << arr[i] << " ";
    }
    cout << endl;
    for (size_t i = 0; i < num.size(); i++) {
        cout << num[i] << " ";
    }
    cout << endl;
    return 0;
}