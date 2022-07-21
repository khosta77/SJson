#ifndef SJSON_SJSON_H
#define SJSON_SJSON_H

#include <any>
#include <map>
#include <string>
#include <vector>
#include <fstream>

namespace stepan_json {
/** \class - Парсер json файлов
 * */
    class SJson {
    private:
        typedef std::map <std::string, std::any> DataFrame;
        DataFrame df;  /* Прослойка map, для каждого отдельного объекта */

        /** \brief - возращает строку в ""
         * \param word - словоко которое должно вернуть
         * \param str - все строка
         * \param i - номер элемента, на котором остановился парсер
         * */
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

        /** \brief - считывает и возращает число
         * \param str - все строка
         * \param i - номер элемента, на котором остановился парсер
         * */
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

        /** \brief - возращает массив в []
         * \param arr - массив которое должно вернуть
         * \param str - все строка
         * \param i - номер элемента, на котором остановился парсер
         * */
        static void read_array(std::vector<double> &arr, const char *str, size_t &i) {
            for (size_t j = i + 1;; j++) {
                if (str[j] == ']') {
                    i = j;
                    break;
                }
                if (str[j] == ' ' || str[j] == '\n') {
                    continue;
                }
                arr.push_back(read_num(str, j));
            }
        }

        /** \brief - Открывает файл и записывает его содержимое в строку
         * \param str - все строка
         * \param filename - имя файла
         * */
        static bool readfile(std::string &s, const char *filename) {
            std::ifstream fp(filename);
            if (!fp.is_open()) {
                return false;
            }

            char nil = '\0';
            std::getline(fp, s, nil);
            fp.close();
            return (s.length() > 0);
        }

        /** \brief - сам парсер
         * \param str - все строка, которую необходимо запарсить
         * \param start - номер элемента, с которого начинает
         * */
        static DataFrame parser(const std::string &s, size_t start) {
            std::string key_value;      /*  */
            std::string buf_key_value;  /* В случае если у нас ключ дает доступ к строке */
            DataFrame buf_data;         /* Буферный map, чтобы можно было иметь больше пространства для маневра */
            bool map_status = false;    /* Проверка если у нас уже ключ - значение */
            for (size_t i = start; s[i] != '}'; i++) {
                if (s[i] == ':') {
                    map_status = true;
                } else if (s[i] == '\"') {
                    buf_key_value = key_value;
                    read_string(key_value, s.c_str(), i);
                    if (map_status) {
                        buf_data[buf_key_value] = key_value;
                        buf_key_value.clear();
                        key_value.clear();
                        map_status = false;
                    }
                } else if ((s[i] == '[') && map_status) {
                    std::vector<double> arr;
                    read_array(arr, s.c_str(), i);
                    buf_data[key_value] = arr;
                    arr.clear();
                    map_status = false;
                    key_value.clear();
                } else if ((((s[i] >= 48) && (s[i] <= 57)) || (s[i] == '-')) && map_status) {
                    buf_data[key_value] = read_num(s.c_str(), i);
                    map_status = false;
                    key_value.clear();
                } else if (((s[i] == 't') || (s[i] == 'f')) && map_status) {
                    buf_data[key_value] = (s[i] == 't') ? true : false;
                    map_status = false;
                    key_value.clear();
                } else if ((s[i] == '{') && map_status) {
                    buf_key_value = key_value;
                    SJson buf_obj(parser(s, (1 + i)));
                    buf_data[buf_key_value] = buf_obj;
                    buf_key_value.clear();
                    key_value.clear();
                    map_status = false;
                }
            }
            return buf_data;
        }

        /** \brief - Конструктор для копирования внутри класса
         * \param friend_data - копируемая переменная
         * */
        SJson(DataFrame friend_data) {
            this->df = friend_data;
        }

    public:

        /** \brief - конструктор по умолчанию
         * */
        SJson() = default;

        /** \brief - конструктор принимающий всю строку
         * \param s - строка
         * */
        SJson(const std::string &s) {
            if(s[0] != '{') {
                throw;
            }
            this->df = parser(s, 0);
        }

        ~SJson() {
            this->df.clear();
        }

        /** \brief - перегрженный оператор [] для доступа к значениям
         * \param key - ключ
         * \return - возращает объект std::any
         * */
        std::any& operator[](const std::string &key) {
            return this->df[key];
        }

        /** \brief - Метод возвращает объект класса Json из строки, содержащей Json-данные.
         * \param s - строка
         * */
        static SJson parse(const std::string &s) {
            return SJson(s);
        }

        /** \brief - Метод возвращает объекта класса Json из файла, содержащего Json-данные в текстовом формате.
         * \param path_to_file - путь до файла
         * */
        static SJson parseFile(const std::string &path_to_file) {
            std::string str;
            if (readfile(str, path_to_file.c_str())) {
                return SJson(str);
            } else {
                throw;
            }
            return SJson();
        }

    };
}

using sj = stepan_json::SJson;

#endif //SJSON_SJSON_H
