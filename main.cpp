#include <iostream>
#include <any>
#include <vector>
#include "SJson.h"

using namespace std;

int main(){
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
}