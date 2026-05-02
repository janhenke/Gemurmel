//
// Created by Jan Henke on 08.05.26.
//

#ifndef GEMURMEL_DETAIL_PROTOCOL_HPP
#define GEMURMEL_DETAIL_PROTOCOL_HPP

#include <cstddef>
// ReSharper disable once CppUnusedIncludeDirective
#include <cstdint>
#include <limits>
#include <memory>
#include <span>
#include <variant>

#include "Mumble.pb.h"

namespace gemurmel::detail {

/**
 * Maximum length of the payload part of the packet according to the specification.
 */
constexpr std::size_t kMaxPayloadLength = 8 * 1024 * 1024 - 1;

/**
 * Length of the packet header.
 */
constexpr std::size_t kHeaderLength = 2 + 4;

/**
 * Maximum length of the entire packet (header + payload).
 */
constexpr std::size_t kMaxPacketLength = kHeaderLength + kMaxPayloadLength;

/**
 * All defined packet types.
 */
enum struct PacketType : std::uint16_t {
	Version = 0,
	UDPTunnel = 1,
	Authenticate = 2,
	Ping = 3,
	Reject = 4,
	ServerSync = 5,
	ChannelRemove = 6,
	ChannelState = 7,
	UserRemove = 8,
	UserState = 9,
	BanList = 10,
	TextMessage = 11,
	PermissionDenied = 12,
	ACL = 13,
	QueryUsers = 14,
	CryptSetup = 15,
	ContextActionModify = 16,
	ContextAction = 17,
	UserList = 18,
	VoiceTarget = 19,
	PermissionQuery = 20,
	CodecVersion = 21,
	UserStats = 22,
	RequestBlob = 23,
	ServerConfig = 24,
	SuggestConfig = 25,
	PluginDataTransmission = 26
};

using ControlPacket =
	std::variant<MumbleProto::Version, MumbleProto::UDPTunnel, MumbleProto::Authenticate, MumbleProto::Ping,
				 MumbleProto::Reject, MumbleProto::ServerSync, MumbleProto::ChannelRemove, MumbleProto::ChannelState,
				 MumbleProto::UserRemove, MumbleProto::UserState, MumbleProto::BanList, MumbleProto::TextMessage,
				 MumbleProto::PermissionDenied, MumbleProto::ACL, MumbleProto::QueryUsers, MumbleProto::CryptSetup,
				 MumbleProto::ContextActionModify, MumbleProto::ContextAction, MumbleProto::UserList,
				 MumbleProto::VoiceTarget, MumbleProto::PermissionQuery, MumbleProto::CodecVersion,
				 MumbleProto::UserStats, MumbleProto::RequestBlob, MumbleProto::ServerConfig,
				 MumbleProto::SuggestConfig, MumbleProto::PluginDataTransmission>;

[[nodiscard]] auto ToPacketType(const ControlPacket&) -> PacketType;

[[nodiscard]] auto Serialize(std::span<std::byte, kMaxPacketLength>, const ControlPacket&) -> std::size_t;

[[nodiscard]] auto Deserialize(std::span<const std::byte, kMaxPacketLength>) -> std::shared_ptr<ControlPacket>;

/**
 * Swaps the endianness of a number if the system is little-endian.
 * @param number The number to swap endianness for
 * @return The number with swapped endianness
 */
// ReSharper disable once CppDFAUnreachableCode
auto SwapNetworkBytes(std::integral auto const number) {
	if constexpr (std::endian::native == std::endian::little) {
		return std::byteswap(number);
	} else {
		return number;
	}
}

//
// The mumble version format (v2) is an uint64:
// major   minor   patch   reserved/unused
// 0xFFFF  0xFFFF  0xFFFF  0xFFFF
// (big-endian)
//
// Legacy versions: These versions are kept around for backward compatibility but
// have since been replaced by other version formats.
//
// Mumble legacy version format (v1) is an uint32:
// major   minor  patch
// 0xFFFF  0xFF   0xFF
// (big-endian)
//
class MumbleVersion {
	std::uint16_t major_;
	std::uint16_t minor_;
	std::uint16_t patch_;

   public:
	MumbleVersion() : major_(0), minor_(0), patch_(0) {}

	MumbleVersion(const std::uint16_t major, const std::uint16_t minor, const std::uint16_t patch)
		: major_(major), minor_(minor), patch_(patch) {}

	explicit operator std::uint64_t() const {
		std::uint64_t result = static_cast<std::uint64_t>(major_) << 48;
		result |= static_cast<std::uint64_t>(minor_) << 32;
		result |= static_cast<std::uint64_t>(patch_) << 16;
		return result;
	}

	explicit operator std::uint32_t() const {
		std::uint32_t result = (static_cast<std::uint32_t>(major_) << 16);
		result |= (std::min(static_cast<std::uint32_t>(minor_),
							static_cast<std::uint32_t>(std::numeric_limits<std::uint8_t>::max()))
				   << 8);
		result |= (std::min(static_cast<std::uint32_t>(patch_),
							static_cast<std::uint32_t>(std::numeric_limits<std::uint8_t>::max())));
		return result;
	}

	[[nodiscard]] auto major() const { return major_; }

	[[nodiscard]] auto minor() const { return minor_; }

	[[nodiscard]] auto patch() const { return patch_; }

	void parse(const std::uint64_t version) {
		major_ = static_cast<std::uint16_t>((version & 0xffff'0000'0000'0000) >> 48);
		minor_ = static_cast<std::uint16_t>((version & 0xffff'0000'0000) >> 32);
		patch_ = static_cast<std::uint16_t>((version & 0xffff'0000) >> 16);
	}
};

}  // namespace gemurmel::detail

#endif  //GEMURMEL_DETAIL_PROTOCOL_HPP
