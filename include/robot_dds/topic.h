
#ifndef __COMWISE__ROBOT_RPC__TOPIC__H__
#define __COMWISE__ROBOT_RPC__TOPIC__H__

#include <string>
#include <type_traits>

class Topic {
public:
    Topic(const std::string& topic_name) : topic_name_(topic_name) {}

    const std::string& get_name() const { return topic_name_; }

private:
    std::string topic_name_;
};

#endif // __COMWISE__ROBOT_RPC__TOPIC__H__