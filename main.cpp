#include <iostream>
#include <any>
#include "SJson.h"

using namespace std;


int main(){
    Json obj = Json::parseFile("./test1.json");
    Json obj2 = std::any_cast<Json>(obj["address"]);
    cout << std::any_cast<string>(obj2["city"]) << endl;
    return 0;
}