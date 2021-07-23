//
// Created by y00612228 on 2021/7/21.
//

#ifndef TINYHTTP_THTASK_H
#define TINYHTTP_THTASK_H

//@TODO
template<class REQ, class RESP>
class THNetworkTask {

public:
    REQ *get_req() { return &this->req; }

    RESP *get_resp() { return &this->resp; }

protected:
    REQ req;
    RESP resp;
};

#endif //TINYHTTP_THTASK_H
