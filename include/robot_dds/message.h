// message.h
#ifndef __COMWISE__ROBOT_RPC__MESSAGE__H__
#define __COMWISE__ROBOT_RPC__MESSAGE__H__

#include <string>
#include <sstream>
#include <iostream>

template <typename T>
class IMessage {
public:
    virtual ~IMessage() = default;
    
    virtual std::string serialize() const = 0;
    virtual void deserialize(const std::string& data) = 0;
    
    virtual T& getContent() = 0;
    virtual const T& getContent() const = 0;
};

// 实现消息类模板
template <typename T>
class Message : public IMessage<T> {
public:
    Message() : content_() {}
    explicit Message(const T& content) : content_(content) {}

    std::string serialize() const override {
        std::ostringstream oss;
        oss << content_;
        return oss.str();
    }

    void deserialize(const std::string& data) override {
        std::istringstream iss(data);
        iss >> content_;
    }

    T& getContent() override {
        return content_;
    }

    const T& getContent() const override {
        return content_;
    }

private:
    T content_;
};

#endif // __COMWISE__ROBOT_RPC__MESSAGE__H__
