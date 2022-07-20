#ifndef SJSON_SJSON_H
#define SJSON_SJSON_H

#include <any>
#include <map>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>

class Json {
private:
    typedef std::map<std::string, std::any> DataFrame;
    DataFrame dataframe;
    inline static DataFrame data;
    inline static std::string last_name;

    static void read_string(std::string &word, const char *str, size_t &i) {
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

    static double read_num(const char *str, size_t &i) {
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

    static void read_array(std::vector<double> &arr, const char *str, size_t &i) {
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

    static bool readfile(std::string& s, const char* filename){
        std::ifstream fp(filename);
        if(! fp.is_open())
            return false;

        char nil = '\0';
        std::getline(fp, s, nil);
        fp.close();
        return (s.length() > 0);
    }

    static DataFrame parser(const std::string &s, size_t start) {
        bool map_status = false;
        DataFrame buf_data;
        std::string buf;
        for (size_t i = start; s[i] != '}'; i++) {
            if (s[i] == ':') {
                map_status = true;
            } else if (s[i] == '\"') {
                buf = last_name;
                read_string(last_name, s.c_str(), i);
                if (map_status) {
                    buf_data[buf] = last_name;
                    buf.clear();
                    last_name.clear();
                    map_status = false;
                }
            } else if ((s[i] == '[') && map_status) {
                std::vector<double> arr;
                read_array(arr, s.c_str(), i);
                buf_data[last_name] = arr;
                arr.clear();
                map_status = false;
                last_name.clear();
            } else if ((((s[i] >= 48) && (s[i] <= 57)) || (s[i] == '-')) && map_status) {
                buf_data[last_name] = read_num(s.c_str(), i);
                map_status = false;
                last_name.clear();
            } else if (((s[i] == 't') || (s[i] == 'f')) && map_status) {
                buf_data[last_name] = (s[i] == 't') ? true : false;
                map_status = false;
                last_name.clear();
            } else if ((s[i] == '{') && map_status) {
                buf = last_name;
                Json buf_obj(parser(s, (1 + i)));
                buf_data[buf] = buf_obj;
                buf.clear();
                last_name.clear();
                map_status = false;
            }
        }
        return buf_data;
    }

    Json(std::map<std::string, std::any> friend_data) {
        dataframe = friend_data;
    }
public:
    // Конструктор из строки, содержащей Json-данные.
    Json() = default;

    Json(const std::string& s) {
        data = parser(s, 0);
        this->dataframe = data;
        data.clear();
    }

    // Метод возращает объект any
    std::any operator[](const std::string& key) {
        return dataframe[key];
    }

    // Метод возвращает объект класса Json из строки, содержащей Json-данные.
    static Json parse(const std::string& s) {
        return Json(parser(s, 0));
    }

    // Метод возвращает объекта класса Json из файла, содержащего Json-данные в текстовом формате.
    static Json parseFile(const std::string& path_to_file) {
        std::string str;
        if(readfile(str, path_to_file.c_str())) {
            return Json(parse(str));
        }
        return Json();
    }

    ~Json() {
        dataframe.clear();
    }
};

#endif //SJSON_SJSON_H
