//
// Created by Oliver Epper on 19.01.24.
//

#ifndef STUNNING_DEBUG_H
#define STUNNING_DEBUG_H

#include <ostream>
#include <iomanip>

void print_bytes(const void *data, size_t length, std::ostream& oss) {
    const uint8_t *bytes = reinterpret_cast<const uint8_t *>(data);
    for (size_t i = 0; i < length; ++i) {
        oss << std::hex
                  << std::setw(2)
                  << std::setfill('0')
                  << static_cast<int>(bytes[i]);

        if ((i + 1) % 8 == 0) {
            oss << "\n";
        }
        else if ((i + 1) % 4 == 0 && i < length - 1) {
            oss << "\t";
        } else {
            oss << " ";
        }
    }
    oss << std::endl;
}

#endif //STUNNING_DEBUG_H
