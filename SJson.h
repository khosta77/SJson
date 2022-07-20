#ifndef SJSON_SJSON_H
#define SJSON_SJSON_H

#include <any>
#include <map>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>

/*
 * Ну а если серьезно:
1) Сделайте парсер, способный прочитать число.
2) Сделайте парсер, способный прочитать строку в кавычках.
3) Сделайте парсер, который будет делать пункты один и два, пока не напорется на закрывающую квадратную скобку.
 (Чтение json-массива).
4) Добавьте в пункт три, выполнение пункта три - натолкнувшись на открывающую скобку, парсер вызывает себя рекурсивно.
Вот уже будет какое-то примерное понимание как эта хреновина работает.
 * */

class Json {
private:
    typedef std::map<std::string, std::any> DataFrame;
    inline static DataFrame data;
    inline static std::string last_name;

    bool Array_status = false;
    bool Object_status = false;

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

    bool readfile(std::string& s, const char* filename){
        std::ifstream fp(filename);
        if(! fp.is_open())
            return false;

        char nil = '\0';
        std::getline(fp, s, nil);
        fp.close();
        return (s.length() > 0);
    }


    static DataFrame init_data() {
        DataFrame d;
        return d;
    }
public:
    // Конструктор из строки, содержащей Json-данные.
    Json() {}
    Json(const std::string& s);

    // Метод возвращает true, если данный экземпляр содержит в себе JSON-массив. Иначе false.
    bool is_array() const;
    // Метод возвращает true, если данный экземпляр содержит в себе JSON-объект. Иначе false.
    bool is_object() const;

    // Метод возвращает значение по ключу key, если экземпляр является JSON-объектом.
    // Значение может иметь один из следующих типов: Json, std::string, double, bool или быть пустым.
    // Если экземпляр является JSON-массивом, генерируется исключение.
    std::any& operator[](const std::string& key);

    // Метод возвращает значение по индексу index, если экземпляр является JSON-массивом.
    // Значение может иметь один из следующих типов: Json, std::string, double, bool или быть пустым.
    // Если экземпляр является JSON-объектом, генерируется исключение.
    std::any& operator[](int index);

    // Метод возвращает объект класса Json из строки, содержащей Json-данные.
    static void parse(const std::string& s) {
        bool map_status = false;
        for (size_t i = 0; s[i] != '\0'; i++) {
            if (s[i] == ':') {
                map_status = true;
            } else if (s[i] == '\"') {
                std::string buf = last_name;
                read_string(last_name, s.c_str(), i);
                if (map_status) {
                    data[buf] = last_name;
                    last_name.clear();
                    buf.clear();
                    map_status = false;
                }
            } else if (s[i] == '[') {
                std::vector<double> arr;
                read_array(arr, s.c_str(), i);
                if (map_status) {
                    data[last_name] = arr;
                    arr.clear();
                    last_name.clear();
                    map_status = false;
                }
            } else if (((s[i] >= 48) && (s[i] <= 57)) || (s[i] == '-')) {
                if (map_status) {
                    data[last_name] = read_num(s.c_str(), i);
                    last_name.clear();
                    map_status = false;
                }
            }
        }

        for (auto &[key, val] : data) {
            if (val.type() == typeid(std::vector<double>)) {
                std::cout << std::any_cast<std::string>(key) << " ";
                std::vector<double> vec = std::any_cast<std::vector<double>>(val);
                for (size_t i = 0; i < vec.size(); i++) {
                    std::cout << vec[i] << " ";
                }
                std::cout << std::endl;
            } else if (val.type() == typeid(std::string))
                std::cout << std::any_cast<std::string>(key) << " " << std::any_cast<std::string>(val) << "\n";
            else if (val.type() == typeid(double))
                std::cout << std::any_cast<std::string>(key) << " " << std::any_cast<double>(val) << "\n";
        }
    }

    // Метод возвращает объекта класса Json из файла, содержащего Json-данные в текстовом формате.
    static Json parseFile(const std::string& path_to_file);
} object;

#endif //SJSON_SJSON_H
