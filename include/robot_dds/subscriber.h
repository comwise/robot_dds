#ifndef __COMWISE__ROBOT_RPC__SUBSCRIBER__H__
#define __COMWISE__ROBOT_RPC__SUBSCRIBER__H__

#include "transport_manager.h"

template <typename T>
class Subscriber {
public:
    Subscriber(const std::string& topic_name, const std::string& ip_address = "")
        : topic_(topic_name), transport_(manager_.create_transport(ip_address)) {}

    void subscribe(T& message) {
        if (transport_) {
            transport_->subscribe(topic_, &message, sizeof(T));
        }
    }

private:
    Topic topic_;
    std::shared_ptr<Transport> transport_;
    TransportManager manager_;
};

#endif // __COMWISE__ROBOT_RPC__SUBSCRIBER__H__
