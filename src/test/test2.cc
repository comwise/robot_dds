#include <iostream>
#include "publisher.h"
#include "subscriber.h"

struct SensorDataMsg {
    int sensor_id;
    float temperature;
};

int main() {
    // 本地共享内存通信
    Publisher<SensorDataMsg> publisher("SensorData");
    SensorDataMsg msg{1, 23.5};
    publisher.publish(msg);

    // 跨机器使用 UDP 广播
    Subscriber<SensorDataMsg> subscriber("SensorData", "192.168.1.100");
    SensorDataMsg received_msg;
    subscriber.subscribe(received_msg);

    std::cout << "Received data: " << received_msg.temperature << std::endl;

    return 0;
}
