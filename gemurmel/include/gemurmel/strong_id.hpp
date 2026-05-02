//
// Created by Junie on 18.05.26.
//

#ifndef GEMURMEL_STRONG_ID_HPP
#define GEMURMEL_STRONG_ID_HPP

#include <compare>
#include <concepts>
#include <cstdint>
#include <functional>

namespace gemurmel {

/**
 * @brief A type-safe ID wrapper that prevents accidental mixing of different ID types.
 *
 * @tparam Tag A unique type used to distinguish this ID from others.
 * @tparam T The underlying integral type (defaults to uint32_t).
 */
template <typename Tag, std::integral T = std::uint32_t>
struct StrongId {
	using value_type = T;

	T value{};

	constexpr StrongId() = default;
	explicit constexpr StrongId(T v) : value(v) {}

	// Defaulted spaceship operator for all comparisons
	auto operator<=>(const StrongId&) const = default;
	bool operator==(const StrongId&) const = default;

	// Explicit conversion to underlying type to prevent accidental loss of type safety
	explicit constexpr operator T() const { return value; }
};

} // namespace gemurmel

// Enable hashing so StrongId can be used as a key in std::unordered_map
namespace std {
template <typename Tag, typename T>
struct hash<gemurmel::StrongId<Tag, T>> {
	size_t operator()(const gemurmel::StrongId<Tag, T>& id) const noexcept {
		return hash<T>{}(id.value);
	}
};
} // namespace std

#endif // GEMURMEL_STRONG_ID_HPP
