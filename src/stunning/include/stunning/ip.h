//
// Created by Oliver Epper on 19.01.24.
//

#ifndef STUNNING_IP_H
#define STUNNING_IP_H

#include "exception.h"

#include <optional>
#include <sstream>
#include <vector>
#include <cerrno>
#include <arpa/inet.h>
#include <netdb.h>

enum address_family {
    IPv4,
    IPv6
};

struct socket_address_t {
    address_family                  family;
    std::string                     value;
    std::optional<unsigned short>   port;
};

std::ostream & operator<<(std::ostream& os, const socket_address_t& address) {
    os << ((address.family == address_family::IPv4) ? "ipv4" : "ipv6")
       << ": " << address.value;
    if (address.port.has_value())
        os << ":" << address.port.value();
    return os;
}

std::vector<socket_address_t> hostname_to_ip(const std::string& hostname) {
    std::vector<socket_address_t> ips{};

    addrinfo hints {
        .ai_family  = PF_UNSPEC,
        .ai_flags   = AI_PASSIVE,
    };

    addrinfo *result;

    int status{};
    if ((status = getaddrinfo(hostname.c_str(), NULL, &hints, &result)) != 0) {
        throw stunning::exception{gai_strerror(status)};
    }

    struct addrinfo *p;
    for (p = result; p != nullptr; p = p->ai_next) {
        switch (p->ai_family) {
            case AF_INET: {
                struct sockaddr_in *address = (struct sockaddr_in *) p->ai_addr;
                char ip[INET_ADDRSTRLEN];
                inet_ntop(AF_INET, &(address->sin_addr), ip, INET_ADDRSTRLEN);
                ips.push_back({.family = IPv4, .value = ip});
                break;
            }
            case AF_INET6: {
                struct sockaddr_in6 *sa = (struct sockaddr_in6 *) p->ai_addr;
                char ip[INET6_ADDRSTRLEN];
                inet_ntop(AF_INET6, &(sa->sin6_addr), ip, INET6_ADDRSTRLEN);
                ips.push_back({.family = IPv6, .value = ip});
                break;
            }
        }
    }

    freeaddrinfo(result);

    return ips;
}

std::optional<std::string> first_ipv4_address(const std::string& server) {
    auto ips = hostname_to_ip(server);

    auto it = std::find_if(ips.begin(), ips.end(), [](const auto& ip){
        return ip.family == address_family::IPv4;
    });

    if (it == ips.end())
        return std::nullopt;

    return it->value;
}

#endif //STUNNING_IP_H
