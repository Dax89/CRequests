cmake_minimum_required(VERSION 3.11)
project(CRequestsTest LANGUAGES CXX)

include(../cmake/CPM.cmake) # https://github.com/TheLartians/CPM.cmake

CPMAddPackage("gh:onqtam/doctest#v2.4.8")
CPMAddPackage("gh:nlohmann/json#v3.10.5")
CPMAddPackage(NAME CRequests SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/..")

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "src/*.h" "src/*.cpp")
add_executable(${PROJECT_NAME} ${SOURCES})

target_link_libraries(
    ${PROJECT_NAME} PRIVATE
    doctest::doctest
    nlohmann_json::nlohmann_json
    CRequests)

target_include_directories(
  ${PROJECT_NAME} PRIVATE
  $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../include>
)

if(CMAKE_BUILD_TYPE MATCHES "Debug") # Enforce warnings as errors if not in Release mode
    if(MSVC)
        target_compile_options(${PROJECT_NAME} PRIVATE /W4 /WX /permissive)
    else()
        target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Werror -Wextra -Wno-error=extra -Wshadow -pedantic)
    endif()
endif()
