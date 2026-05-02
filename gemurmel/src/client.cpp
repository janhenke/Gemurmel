//
// Created by Jan Henke on 10.05.26.
//

#include <gemurmel/client.hpp>

#include <atomic>
#include <thread>

#include <spdlog/spdlog.h>

#include <gemurmel/detail/protocol.hpp>
#include <gemurmel/detail/thread_safe_queue.hpp>

namespace gemurmel {

struct Client::Impl final {
	std::unique_ptr<asio::io_context> owned_io_context;
	asio::io_context* io_context;
	asio::ssl::context tls_context;
	asio::ssl::stream<asio::ip::tcp::socket> tls_socket;
	bool verify_server_certificate = true;
	std::unique_ptr<std::jthread> io_thread;

	asio::steady_timer ping_timer;

	std::atomic_bool connected{false};
	detail::ThreadSafeQueue<ClientEvent> event_queue;

	explicit Impl(asio::io_context* external_context = nullptr)
		: owned_io_context(external_context ? nullptr : std::make_unique<asio::io_context>()),
		  io_context(external_context ? external_context : owned_io_context.get()),
		  tls_context(asio::ssl::context_base::tlsv13_client),
		  tls_socket(*io_context, tls_context),
		  ping_timer(*io_context) {
		GOOGLE_PROTOBUF_VERIFY_VERSION;

		tls_context.set_default_verify_paths();

		if (owned_io_context) {
			io_thread = std::make_unique<std::jthread>([this](const std::stop_token&) {
				auto work = asio::make_work_guard(*io_context);
				// Run until context is stopped or thread is requested to stop
				io_context->run();
			});
		}
	}

	~Impl() {
		if (owned_io_context) {
			owned_io_context->stop();
		}
	}

	auto connect(const std::string_view& host, const std::uint16_t port) noexcept -> std::expected<void, asio::error_code> {

		asio::error_code ec;

		asio::ip::tcp::resolver resolver{*io_context};
		const std::string service = std::to_string(port);
		const auto& endpoints = resolver.resolve(host, service, ec);
		if (ec) {
			return std::unexpected(ec);
		}

		const auto connectedEndpoint = asio::connect(tls_socket.next_layer(), endpoints, ec);
		if (ec) {
			return std::unexpected(ec);
		}
		spdlog::debug("Connected to endpoint: {}, port: {}", connectedEndpoint.address().to_string(),
					  connectedEndpoint.port());
		tls_socket.lowest_layer().set_option(asio::ip::tcp::no_delay(true));
		// Perform SSL handshake and verify the remote host's certificate.
		tls_socket.set_verify_mode(verify_server_certificate ? asio::ssl::verify_peer : asio::ssl::verify_none);
		tls_socket.set_verify_callback(asio::ssl::host_name_verification(std::string{host}));
		tls_socket.handshake(asio::ssl::stream_base::client, ec);
		if (ec) {
			return std::unexpected(ec);
		}

		return {};
	}

	void disconnect();
};

Client::Client() : impl_(std::make_unique<Impl>()) {}

Client::Client(asio::io_context& io_context) : impl_(std::make_unique<Impl>(&io_context)) {}

Client::~Client() = default;
Client::Client(Client&& other) noexcept = default;
Client& Client::operator=(Client&& other) noexcept = default;

Client::Impl& Client::get_impl() {
	return *impl_;
}

const Client::Impl& Client::get_impl() const {
	return *impl_;
}

auto Client::connect(const std::string_view& host, const std::uint16_t port) noexcept
	-> std::expected<void, asio::error_code> {
	return get_impl().connect(host, port);
}

auto Client::disconnect() -> void {
	get_impl().disconnect();
}

void Client::set_verify_server_certificate(const bool verify_server_certificate) {
	get_impl().verify_server_certificate = verify_server_certificate;
}

auto Client::is_connected() const -> bool {
	return get_impl().connected;
}

auto Client::poll_event() -> std::optional<ClientEvent> {
	if (get_impl().event_queue.empty()) {
		return std::nullopt;
	}
	return get_impl().event_queue.try_pop();
}

}  // namespace gemurmel
