//
// Created by Jan Henke on 09.05.26.
//

#ifndef GEMURMEL_CLIENT_HPP
#define GEMURMEL_CLIENT_HPP

#include <expected>
#include <memory>
#include <optional>
#include <string_view>

#include <gemurmel/gemurmel_export.h>
#include <gemurmel/asio.hpp>
#include <gemurmel/client_event.hpp>
#include <gemurmel/protocol.hpp>

namespace gemurmel {

class GEMURMEL_EXPORT Client {
   public:
	Client();
	explicit Client(asio::io_context& io_context);

	~Client();
	Client(Client&& other) noexcept;
	Client& operator=(Client&& other) noexcept;

	Client(const Client& other) = delete;
	Client& operator=(const Client& other) = delete;

	auto connect(const std::string_view& host, std::uint16_t port = kDefaultPort) noexcept -> std::expected<void, asio::error_code>;
	auto disconnect() -> void;

	void set_verify_server_certificate(bool verify_server_certificate);

	[[nodiscard]] auto is_connected() const -> bool;

	auto poll_event() -> std::optional<ClientEvent>;

   private:
	struct Impl;
	std::unique_ptr<Impl> impl_;

	Impl& get_impl();
	[[nodiscard]] const Impl& get_impl() const;
};

}  // namespace gemurmel

#endif  //GEMURMEL_CLIENT_HPP
