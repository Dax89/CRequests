cmake_minimum_required(VERSION 3.14 FATAL_ERROR)

project(CRequests LANGUAGES CXX)

option(ENABLE_ASAN "Enable Address Sanitizer" OFF)

set(CPM_USE_LOCAL_PACKAGES ON)
include(cmake/CPM.cmake) # https://github.com/TheLartians/CPM.cmake

if(WIN32)
    set(CURL_OPTIONS "BUILD_CURL_EXE OFF" "BUILD_SHARED_LIBS OFF" "CURL_USE_SCHANNEL ON" "HTTP_ONLY ON")
else()
    set(CURL_OPTIONS "BUILD_CURL_EXE OFF" "BUILD_SHARED_LIBS OFF" "HTTP_ONLY ON")
endif()

CPMAddPackage(
    NAME CURL
    GIT_REPOSITORY https://github.com/curl/curl.git
    GIT_TAG curl-7_81_0
    VERSION 7.81.0
    OPTIONS ${CURL_OPTIONS}
    )

file(GLOB_RECURSE PUBLIC_HEADERS CONFIGURE_DEPENDS "include/*.h")
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "src/*.h" "src/*.cpp")
add_library(${PROJECT_NAME} STATIC ${PUBLIC_HEADERS} ${SOURCES})
target_link_libraries(${PROJECT_NAME} PRIVATE CURL::libcurl)

target_include_directories(
    ${PROJECT_NAME} PUBLIC
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
    $<BUILD_INTERFACE:${CURL_SOURCE_DIR}/include>
    )

set_target_properties(
    ${PROJECT_NAME} PROPERTIES
    CXX_STANDARD 17
    PUBLIC_HEADER "${PUBLIC_HEADERS}"
    )

if(CMAKE_BUILD_TYPE MATCHES "Debug") # Enforce warnings as errors if not in Release mode
    if(MSVC)
        target_compile_options(${PROJECT_NAME} PRIVATE /W3 /WX /permissive)
        if(ENABLE_ASAN)
            target_compile_options(${PROJECT_NAME} PRIVATE /fsanitize=address)
        endif()
    else()
        target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Werror -Wextra -Wno-error=extra -Wshadow -pedantic)
        if(ENABLE_ASAN)
            target_compile_options(${PROJECT_NAME} PRIVATE -fno-omit-frame-pointer -fsanitize=address)
            target_link_libraries(${PROJECT_NAME} PRIVATE -fsanitize=address)
        endif()
    endif()
endif()

install(TARGETS ${PROJECT_NAME})
