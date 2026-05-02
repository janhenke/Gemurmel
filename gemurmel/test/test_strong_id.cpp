//
// Created by Junie on 18.05.26.
//

#include <catch2/catch_test_macros.hpp>
#include <gemurmel/strong_id.hpp>
#include <unordered_map>
#include <type_traits>

using namespace gemurmel;

struct TestTagA {};
struct TestTagB {};

using IdA = StrongId<TestTagA>;
using IdB = StrongId<TestTagB>;

TEST_CASE("StrongId type safety", "[strong_id]") {
	SECTION("Distinct types are not assignable") {
		// This section essentially documents what the compiler enforces.
		// IdA a{1};
		// IdB b{2};
		// a = b; // Should NOT compile
		
		STATIC_CHECK_FALSE(std::is_assignable_v<IdA&, IdB>);
		STATIC_CHECK_FALSE(std::is_convertible_v<IdB, IdA>);
	}

	SECTION("Explicit conversion is required") {
		constexpr IdA a{42};
		// std::uint32_t val = a; // Should NOT compile
		constexpr auto val = static_cast<std::uint32_t>(a);
		CHECK(val == 42);
	}
}

TEST_CASE("StrongId comparison and hashing", "[strong_id]") {
	SECTION("Comparison operators") {
		IdA a1{10};
		IdA a2{20};
		IdA a1_again{10};

		CHECK(a1 == a1_again);
		CHECK(a1 != a2);
		CHECK(a1 < a2);
		CHECK(a2 > a1);
	}

	SECTION("Hashing in unordered_map") {
		std::unordered_map<IdA, std::string> map;
		map[IdA{1}] = "one";
		map[IdA{2}] = "two";

		CHECK(map.size() == 2);
		CHECK(map[IdA{1}] == "one");
		CHECK(map.at(IdA{2}) == "two");
	}
}
