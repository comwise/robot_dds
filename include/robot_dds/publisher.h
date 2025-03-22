// publisher.h
#ifndef __COMWISE__ROBOT_RPC__PUBLISHER__H__
#define __COMWISE__ROBOT_RPC__PUBLISHER__H__

#include <memory>
#include "transport_manager.h"


template <typename T>
class Publisher {
public:
    Publisher(const std::string& topic_name, const std::string& ip_address = "")
        : topic_(topic_name), transport_(manager_.create_transport(ip_address)) {}

    void publish(const T& message) {
        if (transport_) {
            transport_->publish(topic_, &message, sizeof(T));
        }
    }

private:
    Topic topic_;
    std::shared_ptr<Transport> transport_;
    TransportManager manager_;
};


#endif // __COMWISE__ROBOT_RPC__PUBLISHER__H__