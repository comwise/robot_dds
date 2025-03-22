#ifndef __COMWISE__ROBOT_RPC__TRANSPORT__H__
#define __COMWISE__ROBOT_RPC__TRANSPORT__H__

#include <cstdint>
#include "topic.h"

class Transport {
public:
    virtual ~Transport() = default;

    virtual void publish(const Topic& topic, const void* data, size_t size) = 0;
    virtual void subscribe(const Topic& topic, void* data, size_t size) = 0;
};

#endif // __COMWISE__ROBOT_RPC__TRANSPORT__H__