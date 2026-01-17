/**
 * @file default-logger.cpp
 * @brief This file is for internal use only and must not be in final release in include directories.
 * @internal
 */

#include <iostream>
#include <chrono>
#include <ctime>
#include <iomanip>
#include "default-logger.h"

using namespace PureIOC;
void log(const char *level,
                        const std::string &tag,
                        const std::string &message,
                        std::ostream &os) noexcept {
    auto now = std::chrono::system_clock::now();
    auto in_time_t = std::chrono::system_clock::to_time_t(now);
    os << std::put_time(std::localtime(&in_time_t), "[%Y-%m-%d %X]") << "[" << level << "][" << tag << "] " << message << '\n';
}

LOG_METHOD_MESSAGE(DefaultLogger::debug) {
    log("DEBUG", tag, message, std::cout);
}

LOG_METHOD_MESSAGE(DefaultLogger::error) {
    log("ERROR", tag, message, std::cerr);
}

LOG_METHOD_MESSAGE_AND_EXCEPTION(DefaultLogger::error) {
    std::string custom_message = message + " Details: " + e.what();
    log("ERROR", tag, custom_message, std::cerr);
}

LOG_METHOD_EXCEPTION(DefaultLogger::error) {
    log("ERROR", tag, e.what(), std::cerr);
}

LOG_METHOD_MESSAGE(DefaultLogger::fatal) {
    log("FATAL", tag, message, std::cerr);
}

LOG_METHOD_MESSAGE_AND_EXCEPTION(DefaultLogger::fatal) {
    std::string custom_message = message + " Details: " + e.what();
    log("FATAL", tag, custom_message, std::cerr);
}

LOG_METHOD_EXCEPTION(DefaultLogger::fatal) {
    log("FATAL", tag, e.what(), std::cerr);
}

LOG_METHOD_MESSAGE(DefaultLogger::info) {
    log("INFO", tag, message, std::cout);
}

LOG_METHOD_MESSAGE(DefaultLogger::verbose) {
    log("VERBOSE", tag, message, std::cout);
}

LOG_METHOD_MESSAGE(DefaultLogger::warn) {
    log("WARN", tag, message, std::cerr);
}

LOG_METHOD_MESSAGE_AND_EXCEPTION(DefaultLogger::warn) {
    std::string custom_message = message + " Details: " + e.what();
    log("WARN", tag, custom_message, std::cerr);
}

LOG_METHOD_EXCEPTION(DefaultLogger::warn) {
    log("WARN", tag, e.what(), std::cerr);
}
