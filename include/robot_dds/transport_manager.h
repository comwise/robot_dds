#ifndef __COMWISE__ROBOT_RPC__TRANSPORT_MANAGER__H__
#define __COMWISE__ROBOT_RPC__TRANSPORT_MANAGER__H__

#include <memory>
#include <string>
#include "udp_transport.h"
#include "sharedmemory_transport.h"

class TransportManager {
public:
    std::shared_ptr<Transport> create_transport(const std::string& ip_address = "") {
        if (ip_address.empty()) {
            return std::make_shared<SharedMemoryTransport>();
        } else {
            return std::make_shared<UdpTransport>();
        }
    }
};

#endif // __COMWISE__ROBOT_RPC__TRANSPORT_MANAGER__H__
