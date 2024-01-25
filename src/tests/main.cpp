//
// Created by Oliver Epper on 19.01.24.
//

#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers.hpp>
#include <sstream>

#include <stunning.h>

TEST_CASE("print_bytes", "[print_bytes]") {
    SECTION("stun request") {
        stun_request request{};

        std::ostringstream oss;
        print_bytes(&request, sizeof(request) - 12, oss);

        std::string required = "00 01 00 00\t21 12 a4 42\n\n";

        REQUIRE( required.compare(0, required.length(), oss.str()) == 0);
    }
}

TEST_CASE("convert hostname to ip addresses", "[hostname_to_ips]") {
    SECTION("get the first ipv4 address") {
        REQUIRE( first_ipv4_address("localhost") == "127.0.0.1");
        REQUIRE_THROWS_AS( first_ipv4_address("non-existant"), stunning::exception );
        REQUIRE_THROWS_WITH( first_ipv4_address("non-existant"), "nodename nor servname provided, or not known");
        REQUIRE( first_ipv4_address("one6only.oliver-epper.de") == std::nullopt);
    }
}

TEST_CASE("ostream operator<< for socket_address_t", "[socket_address_t]") {
    SECTION("IPv4 address with port") {
        socket_address_t socketAddress{
                .family = IPv4,
                .value = "127.0.0.1",
                .port = 65535
        };

        std::ostringstream oss;
        oss << socketAddress;

        REQUIRE(oss.str() == "ipv4: 127.0.0.1:65535");
    }

    SECTION("IPv6 address without port") {
        socket_address_t socketAddress{
                .family = IPv6,
                .value = "::1",
        };

        std::ostringstream oss;
        oss << socketAddress;

        REQUIRE(oss.str() == "ipv6: ::1");
    }
}

TEST_CASE("stun packets", "[stun_packets]") {
    SECTION("random transaction id") {
        stun_request r1{}, r2{};

        REQUIRE(r1.transaction_id != r2.transaction_id);
    }
}

TEST_CASE("MAPPED-ADDRESS as a fallback", "[MappedAddress_fallback]") {
    SECTION("stun.t-online.de doesn't send XOR-MAPPED-ADDRESS, atm.") {
        auto result = perform_binding_request("stun.t-online.de");

        REQUIRE(result.has_value());
    }

    SECTION("t-online vs google") {
        auto t_online = perform_binding_request("stun.t-online.de");
        auto google = perform_binding_request("stun1.l.google.com", 19302);

        REQUIRE(t_online.value().value.compare(google.value().value) == 0);
    }
}
