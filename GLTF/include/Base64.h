#pragma once

#include <string>
#include <memory>
#include <vector>

namespace Base64 {
	std::string encode(std::vector<uint8_t> *data, size_t length);
	std::string decode(std::string uri);
}