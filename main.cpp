#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <any>
#include <map>

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

bool readfile(std::string& s, const char* filename){
    std::ifstream fp(filename);
    if(! fp.is_open())
        return false;

    char nil = '\0';
    std::getline(fp, s, nil);
    fp.close();
    return (s.length() > 0);
}

void read_string(std::string &word, const char *str, size_t &i) {
    word.clear();
    for (size_t j = i + 1;; j++) {
        if (str[j] != '\"') {
            word += str[j];
        } else {
            i = j;
            break;
        }
    }
}

int main(){
//// вы можете использовать any в контейнерах:
//    std::map<std::string, std::any> m;
//    m["integer"] = 10;
//    m["string"] = std::string("Hello World");
//    m["float"] = 1.0f;
//
//    for (auto &[key, val] : m)
//    {
//        if (val.type() == typeid(int))
//            std::cout << "int: " << std::any_cast<int>(val) << "\n";
//        else if (val.type() == typeid(std::string))
//            std::cout << "string: " << std::any_cast<std::string>(val) << "\n";
//        else if (val.type() == typeid(float))
//            std::cout << "float: " << std::any_cast<float>(val) << "\n";
//    }
    string str;
    readfile(str, "./test2.json");
//    const char *str = "\"test1\" : [\n"
//                      "0.98, -1.257, -4, -15\n"
//                      "], \n "
//                      "\"Array\" : 8 ";
    Json::parse(str);
//    vector<double> arr;
//    vector<double> num;
//    vector<string> name;
//    for (size_t i = 0; str[i] != '\0'; i++) {
//        if (str[i] == '\"') {
//            read_string(name, str.c_str(), i);
//        } else if (str[i] == '[') {
//            size_t m = i;
//            read_array(arr, str.c_str(), i);
////            cout << m << " " << i << endl;
//        } else if (((str[i] >= 48) && (str[i] <= 57)) || (str[i] == '-')) {
//            num.push_back(read_num(str.c_str(), i));
//        }
//    }

//    for (size_t i = 0; i < name.size(); i++) {
//        cout << name[i] << " ";
//    }
//    cout << endl;
//    for (size_t i = 0; i < arr.size(); i++) {
//        cout << arr[i] << " ";
//    }
//    cout << endl;
//    for (size_t i = 0; i < num.size(); i++) {
//        cout << num[i] << " ";
//    }
//    cout << endl;
//    string last_name;
//
//    string s = str;
//    bool map_status = false;
//    for (size_t i = 0; s[i] != '\0'; i++) {
//        if (s[i] == ':') {
//            map_status = true;
//        } else if (s[i] == '\"') {
//            read_string(last_name, s.c_str(), i);
//            cout << last_name << endl;
//        }
////        else if (s[i] == '[') {
////            read_array(arr, s.c_str(), i);
////        } else if (((s[i] >= 48) && (s[i] <= 57)) || (s[i] == '-')) {
////            num.push_back(read_num(s.c_str(), i));
////        }
//    }
    return 0;
}