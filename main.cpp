#include <iostream>
#include <vector>

#include "SJson.h"

using namespace std;

//void get_num(std::vector<double> &array, const char *str, const size_t i) {
//    static bool call = false;
//    if
//    if ((str[i] >= 48) && (str[i] <= 57)) {
//        if (str[i + 1] != '\0') {
//            cout << str[i] << " " << int(str[i]) << endl;
//        }
//        arr.push_back(static_cast<int>(int(str[i])) - '0');
//    }
//}

int main(){
    const char *str = " \"test1\" : 0.98, \n \"Array\" : 8";
    vector<double> arr;
    vector<string> name;
    bool integer = true, plus = true;  // Целое или не целое число
    for (size_t i = 0; str[i] != '\0'; i++) {
        if (str[i] == '\"') {
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
//        if (((str[i] >= 48) && (str[i] <= 57)) || (str[i] == '+') || (str[i] == '-')) {
//            if (str[i] == '-') {
//                plus = false;
//            }
//            for (size_t j = i; str[j] != ',' && str[j] != '\0' && str[j] != ']'; j++) {
//                if (integer) {
//                    if ((str[i] >= 48) && (str[i] <= 57)) {
//
//                    }
//                } else {
//                    if ((str[i] >= 48) && (str[i] <= 57)) {
//
//                    }
//                }
//            }
//        }
//        int condition = int(str[i]);
//        if ((str[i] >= 48) && (str[i] <= 57)) {
//            if (str[i + 1] == 46) {}
//            if (str[i + 1] != '\0') {
//                cout << str[i] << " " << int(str[i]) << endl;
//            }
//            arr.push_back(static_cast<int>(int(str[i])) - '0');
//        }
    }

    for (size_t i = 0; i < name.size(); i++) {
        cout << name[i] << " ";
    }
    cout << endl;

    return 0;
}