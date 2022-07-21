# SJson

Парсер json файлов.

#### подключение

Необходимо добавить только в коде

```c++
#include "SJson.h"
```

В дальнейшем работа осуществляется через *sj*.

#### Пример

Код С++:

```c++
auto obj = sj::parseFile("./test.json");
cout << any_cast<string>(obj["lastname"]) << " " 
     << any_cast<string>(obj["firstname"]) << " : "
     << any_cast<double>(obj["age"]) << endl;

auto arr = any_cast<vector<double>>(obj["marks"]);
cout << "marks: ";
for (size_t i = 0; i < arr.size(); i++) {
    cout << arr[i] << " ";
}
cout << endl;
auto address = any_cast<sj>(obj["address"]);
cout << "Address: "
     << any_cast<string>(address["city"]) << ", "
     << any_cast<string>(address["street"]) << endl;
return 0;
```

Вывод в консоль:

```commandline
Philimonov Stepan : 22
marks: 4 5 5 5 2 3 
Address: Moscow, Bauman street
```

Сам Json:

```json
{
  "lastname" : "Philimonov",
  "firstname" : "Stepan",
  "age" : 22,
  "islegal" : true,
  "marks" : [
    4,5,5,5,2,3
  ],
  "address" : {
    "city" : "Moscow",
    "street" : "Bauman street"
  }
}
```