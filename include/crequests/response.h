#pragma once

#include <unordered_map>
#include <string_view>
#include <string>
#include <vector>

class Response
{
    private:
        Response(const std::string& url);

    public:
        int statusCode() const;
        const std::unordered_map<std::string, std::string>& header() const;
        const std::vector<uint8_t>& data() const;
        const std::string& url() const;
        std::string_view text() const;

    private:
        int m_statuscode{};
        std::unordered_map<std::string, std::string> m_header;
        std::vector<uint8_t> m_data;
        std::string m_url;

    friend class Request;
};

