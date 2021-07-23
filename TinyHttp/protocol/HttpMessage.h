#ifndef TINYHTTP_HTTPMESSAGE_H
#define TINYHTTP_HTTPMESSAGE_H

#include <cstring>
#include "ProtocolMessage.h"
#include "http_parser.h"

//@TODO 未开始
namespace protocol {

    struct HttpMessageHeader {
        const void *name;
        size_t name_len;
        const void *value;
        size_t value_len;
    };

    class HttpMessage : public ProtocolMessage {
    public:
        // 获取http版本
        const char *get_http_version() const {
            return http_parser_get_version(this->parser);
        }

        // 配置http版本
        bool set_http_version(const char *version) {
            return http_parser_set_version(version, this->parser) == 0;
        }

        /* is_chunked(), is_keep_alive() 仅仅来自被解析的结果 only reflect the parsed result.  */

        bool is_chunked() const {
            return http_parser_chunked(this->parser);
        }

        bool is_keep_alive() const {
            return http_parser_keep_alive(this->parser);
        }

        /*添加、设置头信息*/
        bool add_header(const struct HttpMessageHeader *header) {
            return http_parser_add_header(header->name, header->name_len,
                                          header->value, header->value_len,
                                          this->parser) == 0;
        }

        bool add_header_pair(const char *name, const char *value) {
            return http_parser_add_header(name, strlen(name),
                                          value, strlen(value),
                                          this->parser) == 0;
        }

        bool set_header(const struct HttpMessageHeader *header) {
            return http_parser_set_header(header->name, header->name_len,
                                          header->value, header->value_len,
                                          this->parser) == 0;
        }

        bool set_header_pair(const char *name, const char *value) {
            return http_parser_set_header(name, strlen(name),
                                          value, strlen(value),
                                          this->parser) == 0;
        }

        //适配c++ 11移动
    protected:
        http_parser_t *parser;

    public:
        HttpMessage(HttpMessage &&msg);

        HttpMessage &operator=(HttpMessage &&msg);
    };

    class HttpRequest : public HttpMessage {
    };

    class HttpResponse : public HttpMessage {
    };
}


#endif //TINYHTTP_HTTPMESSAGE_H
