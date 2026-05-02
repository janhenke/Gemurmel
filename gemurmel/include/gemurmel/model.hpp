//
// Created by Junie on 18.05.26.
//

#ifndef GEMURMEL_MODEL_HPP
#define GEMURMEL_MODEL_HPP

#include <string>
#include <vector>

#include <gemurmel/gemurmel_export.h>
#include <gemurmel/strong_id.hpp>

namespace gemurmel {

struct ChannelTag {};

/**
 * ID type for channels.
 */
using ChannelId = StrongId<ChannelTag, std::uint16_t>;

GEMURMEL_EXPORT struct Channel {
	ChannelId id;
	std::string name;
	std::vector<ChannelId> child_ids;
};

}  // namespace gemurmel

#endif  // GEMURMEL_MODEL_HPP
