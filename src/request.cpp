#include "request.h"
#include <string_view>
#include <sstream>

CURLException::CURLException(CURLcode code): std::runtime_error(curl_easy_strerror(code)) { }

Request::Request()
{
    m_curl = curl_easy_init();
    if(!m_curl) throw std::runtime_error("CURL initialization failed");

    this->setOption(CURLOPT_HEADERFUNCTION, &Request::headerCallback);
    this->setOption(CURLOPT_WRITEFUNCTION, &Request::writeCallback);
}

Request::~Request() noexcept { curl_easy_cleanup(m_curl); }

Response Request::send(const std::string& url, const RequestOptions& options)
{
    m_url = url;

    switch(options.type)
    {
        case RequestType::POST: this->setOption(CURLOPT_POST, 1); break;
        case RequestType::PUT: this->setOption(CURLOPT_CUSTOMREQUEST, "PUT"); break;
        case RequestType::DEL: this->setOption(CURLOPT_CUSTOMREQUEST, "DELETE"); break;
        case RequestType::GET: break;
    }

    if(options.useragent) this->setOption(CURLOPT_USERAGENT, *options.useragent);

    this->processOptions(options);

    auto header = this->processHeader(options);
    curl_slist* pheader = nullptr;

    if(!header.empty())
    {
        for(const auto& h : header)
            pheader = curl_slist_append(pheader, h.c_str());

        try {
            this->setOption(CURLOPT_HTTPHEADER, pheader);
        } catch(const std::exception& e) {
            curl_slist_free_all(pheader);
            throw;
        }
    }

    this->setOption(CURLOPT_URL, m_url);

    Response resp(m_url);
    this->setOption(CURLOPT_HEADERDATA, &resp);
    this->setOption(CURLOPT_WRITEDATA, &resp);
    Request::checkCode(curl_easy_perform(m_curl));
    curl_slist_free_all(pheader);
    return resp;
}

void Request::setOption(CURLoption option, CURLOptionValue value) const
{
    std::visit([=](auto&& arg) {
        using T = std::decay_t<decltype(arg)>;
        if constexpr(std::is_same_v<T, std::string>) Request::checkCode(curl_easy_setopt(m_curl, option, arg.c_str()));
        else Request::checkCode(curl_easy_setopt(m_curl, option, arg));
    }, value);
}

void Request::processOptions(const RequestOptions& options)
{
    if(options.params)
    {
        std::ostringstream ss;

        for(const auto& [k, v] : *options.params)
        {
            std::visit([k = k, &ss](auto&& arg) {
                if(ss.tellp()) ss << "&";
                ss << k << "=" << arg;
            }, v);
        }

        if(ss.tellp()) m_url += "?" + ss.str();
    }

    if(options.body)
    {
        std::ostringstream ss;

        for(const auto& [k, v] : *options.body)
        {
            std::visit([k = k, &ss](auto&& arg) {
                using T = std::decay_t<decltype(arg)>;
                if(ss.tellp()) ss << ",";

                ss << "\"" << k << "\"" << ":";
                if constexpr(std::is_same_v<T, std::string>) ss << "\"" << arg << "\"";
                else ss << arg;
            }, v);
        }

        std::string body = "{" + ss.str() + "}";
        this->setOption(CURLOPT_POSTFIELDSIZE, body.size());
        this->setOption(CURLOPT_COPYPOSTFIELDS, body);
    }
}

std::vector<std::string> Request::processHeader(const RequestOptions& options)
{
    std::vector<std::string> header;

    if(options.body && (!options.header || !options.header->count("content-type")))
        header.push_back("content-type: 'application/json; charset=UTF-8'");

    if(options.header)
    {
        for(const auto& [k, v] : *options.header)
        {
            std::ostringstream ss;
            ss << k << ": ";
            std::visit([&ss](auto&& arg) { ss << arg; }, v);
            header.push_back(ss.str());
        }
    }

    return header;
}

size_t Request::headerCallback(char* buffer, size_t size, size_t nitems, void* userdata)
{
    using namespace std::string_literals;

    size_t totsize = size * nitems;
    if(totsize <= 2) return totsize; // Ignore "empty" buffer

    Response* response = reinterpret_cast<Response*>(userdata);
    std::string_view sv(buffer, totsize);
    auto idx = sv.find(":");

    if(idx == std::string_view::npos)
    {
        idx = sv.find(" ");
        if(idx == std::string_view::npos) throw std::runtime_error("Unhandler header '"s + buffer + "'");
        response->m_statuscode = std::stoi(static_cast<std::string>(sv.substr(++idx)), &idx);
    }
    else response->m_header[static_cast<std::string>(sv.substr(0, idx))] = sv.substr(idx + 2, sv.size() - (idx + 4));
    return totsize;
}

size_t Request::writeCallback(char* ptr, size_t size, size_t nmemb, void* userdata)
{
    Response* response = reinterpret_cast<Response*>(userdata);

    size_t totsize = size * nmemb;
    response->m_data.insert(response->m_data.end(), ptr, ptr + totsize);
    return totsize;
}
