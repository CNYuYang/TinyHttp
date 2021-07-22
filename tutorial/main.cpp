#include <iostream>
#include "THHttpServer.h"

using namespace std;

int main(){

    THHttpServer server([](THHttpTask *task){

    });

    server.start(80);

    cout << "Hello World" << endl;
}