#include <crequests/crequests.h>
#include "request.h"

namespace CRequests {

Response get(const std::string& url, const RequestOptions::Params& params, const std::string& useragent) { return Request().send(url, { RequestType::GET, useragent, std::nullopt, params, std::nullopt}); }
Response post(const std::string& url, const RequestOptions::Params& body, const std::string& useragent) { return Request().send(url, { RequestType::POST, useragent, std::nullopt, std::nullopt, body}); }
Response put(const std::string& url, const RequestOptions::Params& body, const std::string& useragent) { return Request().send(url, { RequestType::PUT, useragent, std::nullopt, std::nullopt, body}); }
Response del(const std::string& url, const RequestOptions::Params& params, const RequestOptions::Params& body, const std::string& useragent) { return Request().send(url, { RequestType::DEL, useragent, std::nullopt, params, body}); }
Response send(const std::string& url, const RequestOptions& options) { return Request().send(url, options);  }

}
