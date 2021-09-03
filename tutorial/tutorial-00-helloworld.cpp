#include <iostream>
#include "THHttpServer.h"

using namespace std;

int main(){

    THHttpServer server([](THHttpTask *task){
        task->get_resp()->append_output_body("Hello World!");
    });

    if (server.start(80) == 0){
        getchar(); // 通过输入 "Enter" 结束
        server.stop();
    }
}
