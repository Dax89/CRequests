# CRequests

CRequests is a simple synchronous CURL based HTTP Client, this library is inspired by the famous Python [requests](https://docs.python-requests.org/en/latest/) module.

# Usage
```cpp
#include <iostream>
#include <crequests/crequests.h>

int main() {
    auto response = CRequests::post("https://example.com/todo", {
                                      {"field1", "value"},
                                      {"field2", 42}
                                    });
                                    
   if(response.statusCode() == 200)
     std::cout << "OK, response is: " << response.text() << std::endl;
   else
     std::cout << "ERROR, status code: " << response.statusCode() << std::endl;
}
```

### Requirements & Support
* A C++17 capable compiler
* Tested on Windows 10 and Linux
