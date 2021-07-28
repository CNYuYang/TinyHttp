#ifndef TINYHTTP_HTTPMESSAGE_H
#define TINYHTTP_HTTPMESSAGE_H

#include <cstring>
#include <string>
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

        bool get_parsed_body(const void **body, size_t *size) const {
            return http_parser_get_body(body, size, this->parser) == 0;
        }

        /**
         * 当消息不完整时调用，但您需要已经解析的正文。如果get_parse_body()在调用此函数后仍返回false，则即使头也不完整。在成功状态任务中，消息始终是完整的。
         *
         */
        void end_parsing() {
            http_parser_close_message(this->parser);
        }

        /**
         * Output body 用于发送。想传输收到的消息，也许：msg->get_parsed_body(&body, &size);
         */
        bool append_output_body(const void *buf, size_t size);

        bool append_output_body_nocopy(const void *buf, size_t size);

        void clear_output_body();

        size_t get_output_body_size() const {
            return this->output_body_size;
        }

        /* 适配std::string接口 */
    public:
        bool get_http_version(std::string &version) const {
            const char *str = this->get_http_version();

            if (str) {
                version.assign(str);
                return true;
            }

            return false;
        }

        bool set_http_version(const std::string &version) {
            return this->set_http_version(version.c_str());
        }

        bool add_header_pair(const std::string &name, const std::string &value) {
            return http_parser_add_header(name.c_str(), name.size(),
                                          value.c_str(), value.size(),
                                          this->parser) == 0;
        }

        bool set_header_pair(const std::string &name, const std::string &value) {
            return http_parser_set_header(name.c_str(), name.size(),
                                          value.c_str(), value.size(),
                                          this->parser) == 0;
        }

        bool append_output_body(const std::string &buf) {
            return this->append_output_body(buf.c_str(), buf.size());
        }

    protected:
        http_parser_t *parser;
        size_t cur_size;

    public:
        const http_parser_t *getParser() const {
            return this->parser;
        }

        /*继承至CommMessageOut*/
    protected:
        virtual int encode(struct iovec vectors[], int max);

        virtual int append(const void *buf, size_t *size);

    private:
        struct list_head *combine_from(struct list_head *pos, size_t size);

    private:
        struct list_head output_body;
        size_t output_body_size;

    public:
        HttpMessage(bool is_resp) : parser(new http_parser_t) {
            http_parser_init(is_resp, this->parser);
            INIT_LIST_HEAD(&this->output_body);//初始化头
            this->output_body_size = 0;
            this->cur_size = 0;
        }

        virtual ~HttpMessage() {
            this->clear_output_body();
            if (this->parser) {
                http_parser_deinit(this->parser);
                delete this->parser;
            }
        }

        //适配c++ 11移动
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
