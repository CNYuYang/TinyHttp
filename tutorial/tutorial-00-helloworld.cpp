#include <iostream>
#include "THHttpServer.h"

using namespace std;

int main(){

    THHttpServer server([](THHttpTask *task){
        task->get_resp();
    });

    if (server.start(80) == 0){
        getchar(); // 通过输入 "Enter" 结束
        server.stop();
    }
}