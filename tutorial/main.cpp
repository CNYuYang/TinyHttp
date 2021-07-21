#include <iostream>
#include "THHttpServer.h"

using namespace std;

int main(){

    THHttpServer httpServer([](THHttpTask *task){

    });

    httpServer.start();

    cout << "Hello World" << endl;
}