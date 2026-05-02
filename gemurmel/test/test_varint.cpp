//
// Created by jan on 30.12.23.
//

#include <gemurmel/detail/varint.hpp>

#include <array>

#include <catch2/catch_test_macros.hpp>

TEST_CASE("Test the mumble protocol variable integer decode function", "[common]") {

	SECTION("Decode single byte") {
		constexpr auto data = std::array{std::byte{0b0111'1111}};
		constexpr std::int64_t expected = 0x000007fLL;

		const auto [bytes, result] = gemurmel::detail::DecodeVariableInteger(data).value();

		REQUIRE(bytes == std::size(data));
		REQUIRE(result == expected);
	}

	SECTION("Decode two bytes") {
		constexpr auto data = std::array{std::byte{0b1011'1111}, std::byte{0x00}};
		constexpr std::int64_t expected = 0x00003f00LL;

		const auto [bytes, result] = gemurmel::detail::DecodeVariableInteger(data).value();

		REQUIRE(bytes == std::size(data));
		REQUIRE(result == expected);
	}

	SECTION("Decode three bytes") {
		constexpr auto data = std::array{std::byte{0b1101'1111}, std::byte{0x00}, std::byte{0xff}};
		constexpr std::int64_t expected = 0x001f00ffLL;

		const auto [bytes, result] = gemurmel::detail::DecodeVariableInteger(data).value();

		REQUIRE(bytes == std::size(data));
		REQUIRE(result == expected);
	}

	SECTION("Decode four bytes") {
		constexpr auto data = std::array{std::byte{0b1110'1111}, std::byte{0x00}, std::byte{0xff}, std::byte{0x00}};
		constexpr std::int64_t expected = 0x0f00ff00LL;

		const auto [bytes, result] = gemurmel::detail::DecodeVariableInteger(data).value();

		REQUIRE(bytes == std::size(data));
		REQUIRE(result == expected);
	}

	SECTION("Decode five bytes") {
		constexpr auto data =
			std::array{std::byte{0b1111'0000}, std::byte{0x00}, std::byte{0xff}, std::byte{0x00}, std::byte{0xff}};
		constexpr std::int64_t expected = 0x00ff00ffULL;

		const auto [bytes, result] = gemurmel::detail::DecodeVariableInteger(data).value();

		REQUIRE(bytes == std::size(data));
		REQUIRE(result == expected);
	}

	SECTION("Decode eight bytes") {
		constexpr auto data =
			std::array{std::byte{0b1111'0100}, std::byte{0x00}, std::byte{0xff}, std::byte{0x00}, std::byte{0xff},
					   std::byte{0x00},        std::byte{0xff}, std::byte{0x00}, std::byte{0xff}};
		constexpr std::int64_t expected = 0x00ff00ff00ff00ffULL;

		const auto [bytes, result] = gemurmel::detail::DecodeVariableInteger(data).value();

		REQUIRE(bytes == std::size(data));
		REQUIRE(result == expected);
	}

	SECTION("Decode inverted bytes") {
		constexpr auto data = std::array{std::byte{0b1111'1000}, std::byte{0b0111'1111}};
		constexpr std::int64_t expected = ~0x0000007fLL;

		const auto [bytes, result] = gemurmel::detail::DecodeVariableInteger(data).value();

		REQUIRE(bytes == std::size(data));
		REQUIRE(result == expected);
	}

	SECTION("Decode small int optimization") {
		constexpr auto data = std::array{std::byte{0b1111'1101}};
		constexpr std::int64_t expected = ~0x0000001LL;

		const auto [bytes, result] = gemurmel::detail::DecodeVariableInteger(data).value();

		REQUIRE(bytes == std::size(data));
		REQUIRE(result == expected);
	}
}

TEST_CASE("Test the mumble protocol variable integer encode function", "[common]") {

	SECTION("Encode one byte") {
		constexpr std::int64_t value = 0x0000007fLL;
		constexpr auto expectedData = std::array{std::byte{0b0111'1111}};
		std::array<std::byte, sizeof(expectedData)> buffer{};

		const auto count = gemurmel::detail::EncodeVariableInteger(buffer, value).value();

		REQUIRE(count == std::size(expectedData));
		REQUIRE(buffer == expectedData);
	}

	SECTION("Encode two bytes") {
		constexpr std::int64_t value = 0x00003f00LL;
		constexpr auto expectedData = std::array{std::byte{0b1011'1111}, std::byte{0x00}};
		std::array<std::byte, sizeof(expectedData)> buffer{};

		const auto count = gemurmel::detail::EncodeVariableInteger(buffer, value).value();

		REQUIRE(count == std::size(expectedData));
		REQUIRE(buffer == expectedData);
	}

	SECTION("Encode three bytes") {
		constexpr std::int64_t value = 0x001f00ffLL;
		constexpr auto expectedData = std::array{std::byte{0b1101'1111}, std::byte{0x00}, std::byte{0xff}};
		std::array<std::byte, sizeof(expectedData)> buffer{};

		const auto count = gemurmel::detail::EncodeVariableInteger(buffer, value).value();

		REQUIRE(count == std::size(expectedData));
		REQUIRE(buffer == expectedData);
	}

	SECTION("Encode four bytes") {
		constexpr std::int64_t value = 0x0f00ff00LL;
		constexpr auto expectedData = std::array{std::byte{0b1110'1111}, std::byte{0x00}, std::byte{0xff}, std::byte{0x00}};
		std::array<std::byte, sizeof(expectedData)> buffer{};

		const auto count = gemurmel::detail::EncodeVariableInteger(buffer, value).value();

		REQUIRE(count == std::size(expectedData));
		REQUIRE(buffer == expectedData);
	}

	SECTION("Encode five bytes") {
		constexpr std::int64_t value = 0xff00ff00ULL;
		constexpr auto expectedData =
			std::array{std::byte{0b1111'0000}, std::byte{0xff}, std::byte{0x00}, std::byte{0xff}, std::byte{0x00}};
		std::array<std::byte, sizeof(expectedData)> buffer{};

		const auto count = gemurmel::detail::EncodeVariableInteger(buffer, value).value();

		REQUIRE(count == std::size(expectedData));
		REQUIRE(buffer == expectedData);
	}

	SECTION("Encode eight bytes") {
		constexpr std::int64_t value = 0x7f00ff00ff00ff00ULL;
		constexpr auto expectedData =
			std::array{std::byte{0b1111'0100}, std::byte{0x7f}, std::byte{0x00}, std::byte{0xff}, std::byte{0x00},
					   std::byte{0xff},        std::byte{0x00}, std::byte{0xff}, std::byte{0x00}};
		std::array<std::byte, sizeof(expectedData)> buffer{};

		const auto count = gemurmel::detail::EncodeVariableInteger(buffer, value).value();

		REQUIRE(count == std::size(expectedData));
		REQUIRE(buffer == expectedData);
	}

	SECTION("Encode small int optimization") {
		constexpr std::int64_t value = ~0x0000001LL;
		constexpr auto expectedData = std::array{std::byte{0b1111'1101}};
		std::array<std::byte, sizeof(expectedData)> buffer{};

		const auto count = gemurmel::detail::EncodeVariableInteger(buffer, value).value();

		REQUIRE(count == std::size(expectedData));
		REQUIRE(buffer == expectedData);
	}

	SECTION("Encode inverted bytes") {
		constexpr std::int64_t value = ~0x0000007fLL;
		constexpr auto expectedData = std::array{std::byte{0b1111'1000}, std::byte{0b0111'1111}};
		std::array<std::byte, sizeof(expectedData)> buffer{};

		const auto count = gemurmel::detail::EncodeVariableInteger(buffer, value).value();

		REQUIRE(count == std::size(expectedData));
		REQUIRE(buffer == expectedData);
	}

	SECTION("Encode -2") {
		constexpr std::int64_t value = -2;
		constexpr auto expectedData = std::array{std::byte{0b1111'1101}};
		std::array<std::byte, sizeof(expectedData)> buffer{};

		const auto count = gemurmel::detail::EncodeVariableInteger(buffer, value).value();

		REQUIRE(count == std::size(expectedData));
		REQUIRE(buffer == expectedData);
	}

	SECTION("Encode -5") {
		constexpr std::int64_t value = -5;
		constexpr auto expectedData = std::array{std::byte{0b1111'1000}, std::byte{4}};
		std::array<std::byte, sizeof(expectedData)> buffer{};

		const auto count = gemurmel::detail::EncodeVariableInteger(buffer, value).value();

		REQUIRE(count == std::size(expectedData));
		REQUIRE(buffer == expectedData);
	}
}
