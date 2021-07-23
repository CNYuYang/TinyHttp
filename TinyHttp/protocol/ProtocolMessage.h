//
// Created by y00612228 on 2021/7/23.
//

#ifndef TINYHTTP_PROTOCOLMESSAGE_H
#define TINYHTTP_PROTOCOLMESSAGE_H

#include <cerrno>
#include <utility>
#include "Communicator.h"

namespace protocol {

    class ProtocolMessage : public CommMessageOut, public CommMessageIn {

    protected:
        virtual int encode(struct iovec *vectors, int max) {
            errno = ENOSYS;
            return -1;
        }

        /* 你只需要实现下面其中一个append函数
         * 建议实现第一个使用size_t * size参数的
         * */
        virtual int append(const void *buf, size_t *size) {
            return this->append(buf, *size);
        }

        /*实现此函数时，所有字节都会被消耗。不支持流协议。
         * */
        virtual int append(const void *buf, size_t size) {
            errno = ENOSYS;
            return -1;
        }

    public:
        // 设置大小限制
        void set_size_limit(size_t limit) {
            this->size_limit = limit;
        }

        // 获取大小限制
        size_t get_size_limit() const {
            return this->size_limit;
        }

    public:
        //附件
        class Attachment {
        public:
            virtual ~Attachment() {}
        };

        void set_attachment(Attachment *att) { this->attachment = att; }

        Attachment *get_attachment() const { return this->attachment; }

    protected:
        virtual int feedback(const void *buf, size_t size) {
            if (this->wrapper)
                return this->wrapper->feedback(buf, size);
            else
                //@TODO
                return this->CommMessageIn::feedback(buf, size);
        }

    protected:
        size_t size_limit;

    private:
        Attachment *attachment;
        ProtocolMessage *wrapper;
    public:
        ProtocolMessage() {
            this->size_limit = (size_t) -1;
            this->attachment = nullptr;
            this->wrapper = nullptr;
        }

        virtual ~ProtocolMessage() {
            delete this->attachment;
        }

    public:
        // 移动复制
        ProtocolMessage(ProtocolMessage &&msg) {
            this->size_limit = msg.size_limit;
            msg.size_limit = (size_t) -1;
            this->attachment = msg.attachment;
            msg.attachment = nullptr;
        }

        ProtocolMessage &operator=(ProtocolMessage &&msg)
        noexcept {
            if (&msg != this) {
                this->size_limit = msg.size_limit;
                msg.size_limit = (size_t) -1;
                delete this->attachment;
                this->attachment = msg.attachment;
                msg.attachment = nullptr;
            }

            return *this;
        }

        friend class ProtocolWrapper;
    };

    // 包装类，@TODO 用途未知
    class ProtocolWrapper : public ProtocolMessage {

    protected:
        virtual int encode(struct iovec vectors[], int max) {
            return this->msg->encode(vectors, max);
        }

        virtual int append(const void *buf, size_t *size) {
            return this->msg->append(buf, size);
        }

    protected:
        ProtocolMessage *msg;
    public:
        ProtocolWrapper(ProtocolMessage *msg) {
            msg->wrapper = this;
            this->msg = msg;
        }

    public:
        ProtocolWrapper(ProtocolWrapper &&wrapper) :
                ProtocolMessage(std::move(wrapper)) {
            wrapper.msg->wrapper = this;
            this->msg = wrapper.msg;
            wrapper.msg = nullptr;
        }

        ProtocolWrapper &operator=(ProtocolWrapper &&wrapper) noexcept {
            if (&wrapper != this) {
                *(ProtocolMessage *) this = std::move(wrapper);
                wrapper.msg->wrapper = this;
                this->msg = wrapper.msg;
                wrapper.msg = nullptr;
            }

            return *this;
        }
    };
}

#endif //TINYHTTP_PROTOCOLMESSAGE_H
