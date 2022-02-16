#pragma once

#include <stdexcept>
#include <curl/curl.h>
#include <crequests/response.h>
#include <crequests/options.h>

class CURLException: public std::runtime_error
{
    public:
        CURLException(CURLcode code);
};

class Request
{
    private:
        using CURLOptionValue = std::variant<int, size_t, std::string, curl_write_callback, curl_slist*, Response*>;

    public:
        Request();
        Request(const Request& rhs) = delete;
        ~Request() noexcept;
        Response send(const std::string& url, const RequestOptions& options);

    private:
        void setOption(CURLoption option, CURLOptionValue value) const;
        void processOptions(const RequestOptions& options);
        std::vector<std::string> processHeader(const RequestOptions& options);

    private:
        static inline void checkCode(CURLcode code) { if(code != CURLE_OK) throw CURLException(code); }
        static size_t headerCallback(char *buffer, size_t size, size_t nitems, void *userdata);
        static size_t writeCallback(char* ptr, size_t size, size_t nmemb, void* userdata);

    private:
        std::string m_url;
        CURL* m_curl;
};
