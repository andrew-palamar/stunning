# stunning

very basic stun client function as a header-only library.

CMake understands two configuration variables:

- `-DBUILD_COMMAND=ON` builds and installs the stunning commmand
- `-DBUILD_TESTS=ON` builds the tests for the library

Consume in your own CMake project:

```CMakeLists.txt
cmake_minimum_required(VERSION 3.22)

project(stunning VERSION 0.0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)

include(FetchContent)

FetchContent_Declare(
	stunning
	GIT_REPOSITORY https://github.com/oliverepper/stunning.git
    GIT_TAG main)

FetchContent_MakeAvailable(stunning)

add_executable(run main.cpp)

target_link_libraries(run PRIVATE stunning::stunning)
```
Example usage:

```C++
#include <stunning.h>
#include <iostream>

int main(int argc, char *argv[]) {
    try {
        auto result = perform_binding_request("stun1.l.google.com", 19302);
        if (result.has_value()) {
            if (argc > 1 && std::strcmp(argv[1], "--ip") == 0)
                std::cout << result.value().value;
            else
                std::cout << result.value() << std::endl;
        }
    } catch (const stunning::exception& e) {
        std::cerr << e.what();
    }

    return EXIT_SUCCESS;
}
```
