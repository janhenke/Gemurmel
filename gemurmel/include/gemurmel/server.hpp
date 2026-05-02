//
// Created by Jan Henke on 09.05.26.
//

#ifndef GEMURMEL_SERVER_HPP
#define GEMURMEL_SERVER_HPP

#include <memory>

#include <gemurmel/gemurmel_export.h>
#include <gemurmel/asio.hpp>
#include <gemurmel/protocol.hpp>

namespace gemurmel {
class GEMURMEL_EXPORT Server {
   public:
	Server() = default;
	explicit Server(asio::io_context& io_context);
};
}  // namespace gemurmel

#endif  //GEMURMEL_SERVER_HPP
