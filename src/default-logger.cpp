/**
 * @file default-logger.cpp
 * @brief This file is for internal use only and must not be in final release in include directories.
 * @internal
 */

#include "default-logger.h"

using namespace PureIOC;
void DefaultLogger::log(const char *level,
                        const std::string &tag,
                        const std::string &message,
                        std::ostream &os) noexcept {
    os << "[" << level << "][" << tag << "] " << message << '\n';
}

LOG_METHOD_MESSAGE(DefaultLogger::debug) {
    DefaultLogger::log("DEBUG", tag, message, std::cout);
}

LOG_METHOD_MESSAGE(DefaultLogger::error) {
    DefaultLogger::log("ERROR", tag, message, std::cerr);
}

LOG_METHOD_MESSAGE_AND_EXCEPTION(DefaultLogger::error) {
    std::string custom_message = message + " Details: " + e.what();
    DefaultLogger::log("ERROR", tag, custom_message, std::cerr);
}

LOG_METHOD_EXCEPTION(DefaultLogger::error) {
    DefaultLogger::log("ERROR", tag, e.what(), std::cerr);
}

LOG_METHOD_MESSAGE(DefaultLogger::fatal) {
    DefaultLogger::log("FATAL", tag, message, std::cerr);
}

LOG_METHOD_MESSAGE_AND_EXCEPTION(DefaultLogger::fatal) {
    std::string custom_message = message + " Details: " + e.what();
    DefaultLogger::log("FATAL", tag, custom_message, std::cerr);
}

LOG_METHOD_EXCEPTION(DefaultLogger::fatal) {
    DefaultLogger::log("FATAL", tag, e.what(), std::cerr);
}

LOG_METHOD_MESSAGE(DefaultLogger::info) {
    DefaultLogger::log("INFO", tag, message, std::cout);
}

LOG_METHOD_MESSAGE(DefaultLogger::verbose) {
    DefaultLogger::log("VERBOSE", tag, message, std::cout);
}

LOG_METHOD_MESSAGE(DefaultLogger::warn) {
    DefaultLogger::log("WARN", tag, message, std::cerr);
}

LOG_METHOD_MESSAGE_AND_EXCEPTION(DefaultLogger::warn) {
    std::string custom_message = message + " Details: " + e.what();
    DefaultLogger::log("WARN", tag, custom_message, std::cerr);
}

LOG_METHOD_EXCEPTION(DefaultLogger::warn) {
    DefaultLogger::log("WARN", tag, e.what(), std::cerr);
}
