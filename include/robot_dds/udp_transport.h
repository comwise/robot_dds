#include "transport.h"


#ifdef _WIN32

// Windows UdpTransport.h
#include <winsock2.h>

class UdpTransport : public Transport {
public:
    void publish(const Topic& topic, const void* data, size_t size) override {
        WSADATA wsaData;
        WSAStartup(MAKEWORD(2, 2), &wsaData);

        SOCKET sock = socket(AF_INET, SOCK_DGRAM, 0);
        if (sock == INVALID_SOCKET) {
            throw std::runtime_error("Failed to create socket");
        }

        int broadcast = 1;
        setsockopt(sock, SOL_SOCKET, SO_BROADCAST, (char*)&broadcast, sizeof(broadcast));

        sockaddr_in broadcast_addr;
        broadcast_addr.sin_family = AF_INET;
        broadcast_addr.sin_port = htons(12345);  // Example port
        broadcast_addr.sin_addr.s_addr = INADDR_BROADCAST;

        int sent_bytes = sendto(sock, (const char*)data, size, 0, (sockaddr*)&broadcast_addr, sizeof(broadcast_addr));
        if (sent_bytes == SOCKET_ERROR) {
            throw std::runtime_error("Failed to send UDP broadcast");
        }

        closesocket(sock);
        WSACleanup();
    }

    void subscribe(const Topic& topic, void* data, size_t size) override {
        WSADATA wsaData;
        WSAStartup(MAKEWORD(2, 2), &wsaData);

        SOCKET sock = socket(AF_INET, SOCK_DGRAM, 0);
        if (sock == INVALID_SOCKET) {
            throw std::runtime_error("Failed to create socket");
        }

        sockaddr_in server_addr;
        server_addr.sin_family = AF_INET;
        server_addr.sin_port = htons(12345);  // Example port
        server_addr.sin_addr.s_addr = INADDR_ANY;

        if (bind(sock, (sockaddr*)&server_addr, sizeof(server_addr)) == SOCKET_ERROR) {
            throw std::runtime_error("Failed to bind socket");
        }

        sockaddr_in client_addr;
        int addr_len = sizeof(client_addr);
        int recv_bytes = recvfrom(sock, (char*)data, size, 0, (sockaddr*)&client_addr, &addr_len);
        if (recv_bytes == SOCKET_ERROR) {
            throw std::runtime_error("Failed to receive UDP message");
        }

        closesocket(sock);
        WSACleanup();
    }
};

#else

#include <arpa/inet.h>
#include <sys/socket.h>


class UdpTransport : public Transport {
public:
    void publish(const Topic& topic, const void* data, size_t size) override {
        // 使用 UDP 广播发送数据
        // 这里只是一个简化的实现，实际中要使用合适的网络接口
        int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
        struct sockaddr_in broadcast_addr;
        broadcast_addr.sin_family = AF_INET;
        broadcast_addr.sin_port = htons(12345);  // 示例端口
        broadcast_addr.sin_addr.s_addr = INADDR_BROADCAST;

        sendto(sockfd, data, size, 0, (struct sockaddr*)&broadcast_addr, sizeof(broadcast_addr));
        close(sockfd);
    }

    void subscribe(const Topic& topic, void* data, size_t size) override {
        // 从 UDP 接收数据
        int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
        struct sockaddr_in server_addr;
        server_addr.sin_family = AF_INET;
        server_addr.sin_port = htons(12345);  // 示例端口
        server_addr.sin_addr.s_addr = INADDR_ANY;

        bind(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr));

        recvfrom(sockfd, data, size, 0, nullptr, nullptr);
        close(sockfd);
    }
};

#endif
