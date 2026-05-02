//
// Created by Jan Henke on 08.05.26.
//

#include <gemurmel/detail/protocol.hpp>

namespace gemurmel::detail {

// helper type for the visitor
template <class... Ts>
struct overloaded : Ts... {
	using Ts::operator()...;
};

auto ToPacketType(const ControlPacket& packet) -> PacketType {
	return std::visit(
		overloaded{
			[](const MumbleProto::Version&) -> auto { return PacketType::Version; },
			[](const MumbleProto::UDPTunnel&) -> auto { return PacketType::UDPTunnel; },
			[](const MumbleProto::Authenticate&) -> auto { return PacketType::Authenticate; },
			[](const MumbleProto::Ping&) -> auto { return PacketType::Ping; },
			[](const MumbleProto::Reject&) -> auto { return PacketType::Reject; },
			[](const MumbleProto::ServerSync&) -> auto { return PacketType::ServerSync; },
			[](const MumbleProto::ChannelRemove&) -> auto { return PacketType::ChannelRemove; },
			[](const MumbleProto::ChannelState&) -> auto { return PacketType::ChannelState; },
			[](const MumbleProto::UserRemove&) -> auto { return PacketType::UserRemove; },
			[](const MumbleProto::UserState&) -> auto { return PacketType::UserState; },
			[](const MumbleProto::BanList&) -> auto { return PacketType::BanList; },
			[](const MumbleProto::TextMessage&) -> auto { return PacketType::TextMessage; },
			[](const MumbleProto::PermissionDenied&) -> auto { return PacketType::PermissionDenied; },
			[](const MumbleProto::ACL&) -> auto { return PacketType::ACL; },
			[](const MumbleProto::QueryUsers&) -> auto { return PacketType::QueryUsers; },
			[](const MumbleProto::CryptSetup&) -> auto { return PacketType::CryptSetup; },
			[](const MumbleProto::ContextActionModify&) -> auto { return PacketType::ContextActionModify; },
			[](const MumbleProto::ContextAction&) -> auto { return PacketType::ContextAction; },
			[](const MumbleProto::UserList&) -> auto { return PacketType::UserList; },
			[](const MumbleProto::VoiceTarget&) -> auto { return PacketType::VoiceTarget; },
			[](const MumbleProto::PermissionQuery&) -> auto { return PacketType::PermissionQuery; },
			[](const MumbleProto::CodecVersion&) -> auto { return PacketType::CodecVersion; },
			[](const MumbleProto::UserStats&) -> auto { return PacketType::UserStats; },
			[](const MumbleProto::RequestBlob&) -> auto { return PacketType::RequestBlob; },
			[](const MumbleProto::ServerConfig&) -> auto { return PacketType::ServerConfig; },
			[](const MumbleProto::SuggestConfig&) -> auto { return PacketType::SuggestConfig; },
			[](const MumbleProto::PluginDataTransmission&) -> auto { return PacketType::PluginDataTransmission; }},
		packet);
}

auto Serialize(std::span<std::byte, kMaxPacketLength> buffer, const ControlPacket& packet) -> std::size_t {

	const auto packet_type = ToPacketType(packet);
	const auto& message = std::visit([](const auto& msg) -> const google::protobuf::Message& { return msg; }, packet);

	const std::size_t payload_bytes = message.ByteSizeLong();
	const size_t total_length = kHeaderLength + payload_bytes;
	const auto numeric_packet_type = SwapNetworkBytes(std::to_underlying(packet_type));
	const auto payload_length = SwapNetworkBytes(static_cast<uint32_t>(payload_bytes));

	std::byte* data = buffer.data();
	std::memcpy(data, &numeric_packet_type, sizeof(numeric_packet_type));
	std::memcpy(data + sizeof(numeric_packet_type), &payload_length, sizeof(payload_length));
	message.SerializeToArray(data + kHeaderLength, static_cast<int>(payload_bytes));

	return total_length;
}

auto Deserialize(std::span<const std::byte, kMaxPacketLength> buffer) -> std::shared_ptr<ControlPacket> {

	std::uint16_t raw_packet_type;
	std::uint32_t raw_payload_length;

	std::memcpy(&raw_packet_type, buffer.data(), sizeof(raw_packet_type));
	std::memcpy(&raw_payload_length, buffer.data() + sizeof(raw_packet_type), sizeof(raw_payload_length));

	const auto packet_type = static_cast<PacketType>(SwapNetworkBytes(raw_packet_type));
	const auto payload_length = SwapNetworkBytes(raw_payload_length);
	const auto payload = buffer.subspan(kHeaderLength, payload_length);

	std::shared_ptr<ControlPacket> packet;
	switch (packet_type) {
		case PacketType::Version:
			packet = std::make_shared<ControlPacket>(std::in_place_type<MumbleProto::Version>);
			break;
		case PacketType::UDPTunnel:
			packet = std::make_shared<ControlPacket>(std::in_place_type<MumbleProto::UDPTunnel>);
			break;
		case PacketType::Authenticate:
			packet = std::make_shared<ControlPacket>(std::in_place_type<MumbleProto::Authenticate>);
			break;
		case PacketType::Ping:
			packet = std::make_shared<ControlPacket>(std::in_place_type<MumbleProto::Ping>);
			break;
		case PacketType::Reject:
			packet = std::make_shared<ControlPacket>(std::in_place_type<MumbleProto::Reject>);
			break;
		case PacketType::ServerSync:
			packet = std::make_shared<ControlPacket>(std::in_place_type<MumbleProto::ServerSync>);
			break;
		case PacketType::ChannelRemove:
			packet = std::make_shared<ControlPacket>(std::in_place_type<MumbleProto::ChannelRemove>);
			break;
		case PacketType::ChannelState:
			packet = std::make_shared<ControlPacket>(std::in_place_type<MumbleProto::ChannelState>);
			break;
		case PacketType::UserRemove:
			packet = std::make_shared<ControlPacket>(std::in_place_type<MumbleProto::UserRemove>);
			break;
		case PacketType::UserState:
			packet = std::make_shared<ControlPacket>(std::in_place_type<MumbleProto::UserState>);
			break;
		case PacketType::BanList:
			packet = std::make_shared<ControlPacket>(std::in_place_type<MumbleProto::BanList>);
			break;
		case PacketType::TextMessage:
			packet = std::make_shared<ControlPacket>(std::in_place_type<MumbleProto::TextMessage>);
			break;
		case PacketType::PermissionDenied:
			packet = std::make_shared<ControlPacket>(std::in_place_type<MumbleProto::PermissionDenied>);
			break;
		case PacketType::ACL:
			packet = std::make_shared<ControlPacket>(std::in_place_type<MumbleProto::ACL>);
			break;
		case PacketType::QueryUsers:
			packet = std::make_shared<ControlPacket>(std::in_place_type<MumbleProto::QueryUsers>);
			break;
		case PacketType::CryptSetup:
			packet = std::make_shared<ControlPacket>(std::in_place_type<MumbleProto::CryptSetup>);
			break;
		case PacketType::ContextActionModify:
			packet = std::make_shared<ControlPacket>(std::in_place_type<MumbleProto::ContextActionModify>);
			break;
		case PacketType::ContextAction:
			packet = std::make_shared<ControlPacket>(std::in_place_type<MumbleProto::ContextAction>);
			break;
		case PacketType::UserList:
			packet = std::make_shared<ControlPacket>(std::in_place_type<MumbleProto::UserList>);
			break;
		case PacketType::VoiceTarget:
			packet = std::make_shared<ControlPacket>(std::in_place_type<MumbleProto::VoiceTarget>);
			break;
		case PacketType::PermissionQuery:
			packet = std::make_shared<ControlPacket>(std::in_place_type<MumbleProto::PermissionQuery>);
			break;
		case PacketType::CodecVersion:
			packet = std::make_shared<ControlPacket>(std::in_place_type<MumbleProto::CodecVersion>);
			break;
		case PacketType::UserStats:
			packet = std::make_shared<ControlPacket>(std::in_place_type<MumbleProto::UserStats>);
			break;
		case PacketType::RequestBlob:
			packet = std::make_shared<ControlPacket>(std::in_place_type<MumbleProto::RequestBlob>);
			break;
		case PacketType::ServerConfig:
			packet = std::make_shared<ControlPacket>(std::in_place_type<MumbleProto::ServerConfig>);
			break;
		case PacketType::SuggestConfig:
			packet = std::make_shared<ControlPacket>(std::in_place_type<MumbleProto::SuggestConfig>);
			break;
		case PacketType::PluginDataTransmission:
			packet = std::make_shared<ControlPacket>(std::in_place_type<MumbleProto::PluginDataTransmission>);
			break;
	}
	std::visit([&](auto& message) { message.ParseFromArray(payload.data(), static_cast<int>(payload_length)); },
			   *packet);
	return packet;
}
}  // namespace gemurmel::detail