#pragma once

#include "options.h"
#include "response.h"

namespace CRequests {

Response get(const std::string& url, const RequestOptions::Params& params = { }, const std::string& useragent = std::string());
Response post(const std::string& url, const RequestOptions::Params& body = { }, const std::string& useragent = std::string());
Response put(const std::string& url, const RequestOptions::Params& body = { }, const std::string& useragent = std::string());
Response del(const std::string& url, const RequestOptions::Params& params = { }, const RequestOptions::Params& body = { }, const std::string& useragent = std::string());
Response send(const std::string& url, const RequestOptions& options);

}
