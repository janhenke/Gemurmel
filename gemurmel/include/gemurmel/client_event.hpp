//
// Created by Jan Henke on 10.05.26.
//

#ifndef GEMURMEL_CLIENT_EVENT_HPP
#define GEMURMEL_CLIENT_EVENT_HPP

#include <variant>

#include <gemurmel/gemurmel_export.h>
#include <gemurmel/asio.hpp>

namespace gemurmel {

struct GEMURMEL_EXPORT ClientConnectedEvent {};

struct GEMURMEL_EXPORT ClientDisconnectedEvent {
	asio::error_code ec;
};

using ClientEvent = std::variant<ClientConnectedEvent, ClientDisconnectedEvent>;

}  // namespace gemurmel

#endif  //GEMURMEL_CLIENT_EVENT_HPP
