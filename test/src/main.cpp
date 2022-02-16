#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include <doctest/doctest.h>
#include <crequests/crequests.h>
#include <nlohmann/json.hpp>

inline nlohmann::json jsonify(const Response& r) { return nlohmann::json::parse(r.text()); }

TEST_CASE("GET")
{
    auto response = CRequests::get("https://jsonplaceholder.typicode.com/todos");
    REQUIRE(response.statusCode() == 200);
    REQUIRE(jsonify(response).is_array());

    response = CRequests::get("https://jsonplaceholder.typicode.com/comments", {
                                  {"postId", 1}
                              });

    REQUIRE(response.statusCode() == 200);
    REQUIRE(jsonify(response).is_array());
}

TEST_CASE("POST")
{
    auto response = CRequests::post("https://jsonplaceholder.typicode.com/todos", {
                                        {"title", "thetitle"},
                                        {"body", "thebody"},
                                        {"userid", 1},
                                    });

    REQUIRE(response.statusCode() == 201);
    REQUIRE(jsonify(response).is_object());
}

TEST_CASE("PUT")
{
    auto response = CRequests::put("https://jsonplaceholder.typicode.com/posts/1");
    REQUIRE(response.statusCode() == 200);
    REQUIRE(jsonify(response).is_object());
}

TEST_CASE("DELETE")
{
    auto response = CRequests::del("https://jsonplaceholder.typicode.com/posts/1");
    REQUIRE(response.statusCode() == 200);
    REQUIRE(jsonify(response).is_object());
}
