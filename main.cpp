#include <iostream>
#include <vector>

#include "SJson.h"

using namespace std;

void get_num(std::vector<double> &array, const char *str, const size_t i) {
    static bool call = false;
    if
    if ((str[i] >= 48) && (str[i] <= 57)) {
        if (str[i + 1] != '\0') {
            cout << str[i] << " " << int(str[i]) << endl;
        }
        arr.push_back(static_cast<int>(int(str[i])) - '0');
    }
}

int main(){
    const char *str = "test1 0.98 Array 8";
    vector<double> arr;
    bool integer = false;  // Целое или не целое число
    for (size_t i = 0; str[i] != '\0'; i++) {
        int condition = int(str[i]);
        switch (condition) {
            case 46: {

            }
            case 95: {

            }
            default: {

            }
        }
        if ((str[i] >= 48) && (str[i] <= 57)) {
            if (str[i + 1] == 46) {}
            if (str[i + 1] != '\0') {
                cout << str[i] << " " << int(str[i]) << endl;
            }
            arr.push_back(static_cast<int>(int(str[i])) - '0');
        }
    }

    for (size_t i = 0; i < arr.size(); i++) {
        cout << arr[i] << " ";
    }
    cout << endl;

    return 0;
}