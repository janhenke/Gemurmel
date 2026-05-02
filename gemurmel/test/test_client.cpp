#include <catch2/catch_test_macros.hpp>
#include <gemurmel/client.hpp>

TEST_CASE("Client construction", "[client]") {
    SECTION("Default construction") {
        gemurmel::Client client;
        CHECK_FALSE(client.is_connected());
    }

    SECTION("Construction with external io_context") {
        asio::io_context io_context;
        gemurmel::Client client(io_context);
        CHECK_FALSE(client.is_connected());
    }
}
