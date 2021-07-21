#include <iostream>
#include "THHttpServer.h"

using namespace std;

int main(){

    THHttpServer httpServer([](THHttpTask *task){

    });
    cout << "Hello World" << endl;
}