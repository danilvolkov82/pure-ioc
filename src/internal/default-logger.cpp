/**
 * @file default-logger.cpp
 * @brief This file is for internal use only and must not be in final release in include directories.
 * @internal
 */

#include <chrono>
#include <ctime>
#include <exception>
#include <iomanip>
#include <iostream>
#include <string>
#include "internal/default-logger.h"

using namespace PureIOC;

namespace {
std::string exception_message(const std::exception_ptr &e) {
    if (!e) {
        return "Unknown exception";
    }

    try {
        std::rethrow_exception(e);
    } catch (const std::exception &ex) {
        return ex.what();
    } catch (...) {
        return "Unknown exception";
    }
}
}

void log(const char *level,
                        std::string_view tag,
                        std::string_view message,
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
    std::string custom_message = std::string(message) + " Details: " + exception_message(e);
    log("ERROR", tag, custom_message, std::cerr);
}

LOG_METHOD_EXCEPTION(DefaultLogger::error) {
    log("ERROR", tag, exception_message(e), std::cerr);
}

LOG_METHOD_MESSAGE(DefaultLogger::fatal) {
    log("FATAL", tag, message, std::cerr);
}

LOG_METHOD_MESSAGE_AND_EXCEPTION(DefaultLogger::fatal) {
    std::string custom_message = std::string(message) + " Details: " + exception_message(e);
    log("FATAL", tag, custom_message, std::cerr);
}

LOG_METHOD_EXCEPTION(DefaultLogger::fatal) {
    log("FATAL", tag, exception_message(e), std::cerr);
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
    std::string custom_message = std::string(message) + " Details: " + exception_message(e);
    log("WARN", tag, custom_message, std::cerr);
}

LOG_METHOD_EXCEPTION(DefaultLogger::warn) {
    log("WARN", tag, exception_message(e), std::cerr);
}
