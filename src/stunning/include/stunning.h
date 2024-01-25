//
// Created by Oliver Epper on 19.01.24.
//

#ifndef STUNNING_STUNNING_H
#define STUNNING_STUNNING_H

#include "stunning/debug.h"
#include "stunning/exception.h"
#include "stunning/ip.h"

#include <array>
#include <cstring>
#include <random>

constexpr uint16_t kBindingRequest      = 0x0001;
constexpr uint16_t kBindingResponse     = 0x0101;
constexpr uint16_t kMappedAddress       = 0x0001;
constexpr uint16_t kXorMappedAddress    = 0x0020;
constexpr uint32_t kMessageCookie       = 0x2112a442;

struct __attribute__((packed)) stun_request {
    explicit stun_request() {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<uint8_t> distribution(0, 255);
        for (int i = 0; i < transaction_id.size(); i++) {
            transaction_id[i] = distribution(gen);
        }
    }
    const int16_t message_type      = htons(kBindingRequest);
    const int16_t message_length    = 0;
    const int32_t message_cookie    = htonl(kMessageCookie);
    std::array<uint8_t, 12> transaction_id;
};

struct __attribute__((packed)) stun_response {
    int16_t message_type;
    int16_t message_length;
    int32_t message_cookie;
    std::array<uint8_t, 12> transaction_id;
    std::array<uint8_t, 1000> attributes;
};

std::optional<socket_address_t> perform_binding_request(const std::string& server, const unsigned short port = 3478) {
    // get socket
    int fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (fd == -1)
        throw stunning::exception{strerror(errno)};

    // set timeout
    struct timeval timeout;
    timeout.tv_sec = 1;
    timeout.tv_usec = 0;
    if (setsockopt(fd, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout)) == -1)
        throw stunning::exception{strerror(errno)};

    // get the server address
    sockaddr_in server_address{0};
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(port);

    auto maybe_server_ipv4 = first_ipv4_address(server);
    if (maybe_server_ipv4.has_value())
        inet_pton(AF_INET, maybe_server_ipv4.value().c_str(), &server_address.sin_addr);
    else
        throw stunning::exception{"could not get ipv4 address for server" + server};


    // send the binding request
    stun_request request{};
    if (sendto(fd, &request, sizeof(stun_request), 0, (sockaddr *)&server_address, sizeof(server_address)) == -1)
        throw stunning::exception{strerror(errno)};

    // read the response
    stun_response response{};
    auto received = recv(fd, &response, sizeof(stun_response), 0);

    if (response.transaction_id == request.transaction_id &&
        response.message_type == kBindingResponse) {
        auto const& attributes = response.attributes;
        auto const& attributes_length = std::min<unsigned short>(ntohs(response.message_length), response.attributes.size());

        std::optional<socket_address_t> fallback;
        int i = 0;
        while (i < attributes_length) {
            auto type = ntohs(*(uint16_t *)(&attributes[i]));
            auto length = ntohs(*(uint16_t *)(&attributes[i + 2]));

            if (type == kMappedAddress) {
                auto port = ntohs(*(uint16_t *)(&attributes[i + 6]));
                std::string ip =
                        std::to_string(attributes[i +  8]) + "." +
                        std::to_string(attributes[i +  9]) + "." +
                        std::to_string(attributes[i + 10]) + "." +
                        std::to_string(attributes[i + 11]);

                fallback = socket_address_t{.family = IPv4, .value = ip, .port = port};
            }

            if (type == kXorMappedAddress) {
                auto port = ntohs(*(uint16_t *)(&attributes[i + 6])) ^(kMessageCookie >> 16);
                std::string ip =
                        std::to_string(attributes[i +  8] ^((kMessageCookie & 0xff000000) >> 24)) + "." +
                        std::to_string(attributes[i +  9] ^((kMessageCookie & 0x00ff0000) >> 16)) + "." +
                        std::to_string(attributes[i + 10] ^((kMessageCookie & 0x0000ff00) >>  8)) + "." +
                        std::to_string(attributes[i + 11] ^((kMessageCookie & 0x000000ff) >>  0));

                return socket_address_t{.family = IPv4, .value = ip, .port = port};
            }
            i += (length + 4);
        }

        if (fallback.has_value())
            return fallback;
    }

    if (received == -1)
        throw stunning::exception{"no data received"};

    return std::nullopt;
}



#endif //STUNNING_STUNNING_H
