# stunning

very basic stun client function as a header-only library

```C++
#include <stunning.h>
#include <iostream>

int main(int argc, char *argv[]) {
    try {
        auto result = perform_binding_request("stun.teamfon.de");
        if (result.has_value()) {
            if (argc > 1 && std::strcmp(argv[1], "--ip") == 0)
                std::cout << result.value().value;
            else
                std::cout << result.value() << std::endl;
        }
    } catch (const std::exception& e) {
        std::cerr << e.what();
    }

    return EXIT_SUCCESS;
}
```

