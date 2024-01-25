//
// Created by Oliver Epper on 19.01.24.
//

#ifndef STUNNING_EXCEPTION_H
#define STUNNING_EXCEPTION_H

#include <exception>
#include <string>

namespace stunning {
    struct exception : public std::exception {
        explicit exception(std::string message) : m_message{std::move(message)} {};

        [[nodiscard]] const char * what() const noexcept override {
            return m_message.c_str();
        }

    private:
        std::string m_message;
    };
}


#endif //STUNNING_EXCEPTION_H
