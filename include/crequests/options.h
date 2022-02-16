#pragma once

#include <cstddef>
#include <unordered_map>
#include <optional>
#include <string>
#include <variant>

enum class RequestType { GET, POST, PUT, DEL };

struct RequestOptions
{
    using Header = std::unordered_map<std::string, std::variant<int, size_t, std::string>>;
    using Params = std::unordered_map<std::string, std::variant<int, size_t, std::string>>;

    RequestType type{RequestType::GET};
    std::optional<std::string> useragent;
    std::optional<Header> header;
    std::optional<Params> params;
    std::optional<Params> body;
};
