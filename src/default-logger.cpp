/**
 * @file default-logger.cpp
 * @brief This file is for internal use only and must not be in final release in include directories.
 * @internal
 */

#include "default-logger.h"

using namespace ReactiveUI;
void
DefaultLogger::log(const char *level, const std::string &message, std::ostream &os) {
    os << "[" << level << "] " << message << '\n';
}

void
DefaultLogger::debug(const std::string &message) {
    log("DEBUG", message, std::cout);
}

void
DefaultLogger::error(const std::string &message) {
    log("ERROR", message, std::cerr);
}

void
DefaultLogger::fatal(const std::string &message) {
    log("FATAL", message, std::cerr);
}

void
DefaultLogger::info(const std::string &message) {
    log("INFO", message, std::cout);
}

void
DefaultLogger::verbose(const std::string &message) {
    log("VERBOSE", message, std::cout);
}

void 
DefaultLogger::warn(const std::string &message) {
    log("WARN", message, std::cerr);
}