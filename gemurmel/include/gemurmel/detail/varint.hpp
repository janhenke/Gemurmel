//
// Created by Jan Henke on 10.05.26.
//

#ifndef GEMURMEL_VARINT_HPP
#define GEMURMEL_VARINT_HPP

#include <expected>
#include <span>
#include <string_view>

#include <gemurmel/gemurmel_export.h>

namespace gemurmel::detail {

GEMURMEL_EXPORT auto DecodeVariableInteger(
	std::span<const std::byte> tuple) -> std::expected<std::tuple<std::size_t, std::int64_t>, std::u8string>;

GEMURMEL_EXPORT auto EncodeVariableInteger(std::span<std::byte> buffer,
												  std::int64_t value) -> std::expected<std::size_t, std::u8string>;
}

#endif  //GEMURMEL_VARINT_HPP
