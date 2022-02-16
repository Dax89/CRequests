#include <crequests/response.h>

Response::Response(const std::string& url): m_url(url) { }
int Response::statusCode() const { return m_statuscode; }
const std::unordered_map<std::string, std::string>& Response::header() const { return m_header; }
const std::string& Response::url() const { return m_url; }
const std::vector<uint8_t>& Response::data() const { return m_data; }
std::string_view Response::text() const { return std::string_view(reinterpret_cast<const char*>(m_data.data()), m_data.size()); }
