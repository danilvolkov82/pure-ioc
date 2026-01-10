/**
 * @file default-logger.h
 * @brief This file is for internal use only and must not be in final release in include directories.
 */

#ifndef DEFAULT_LOGGER_H
#define DEFAULT_LOGGER_H
#pragma once
#include <iostream>
#include <string>

#include "logger-interface.h"

namespace PureIOC {
/**
 * @brief A default logger implementation that writes to standard output and standard error.
 * @internal
 */
class DefaultLogger final : public ILogger {
private:
    /**
     * @brief Logs a message to the specified output stream.
     * @param level The log level.
     * @param message The message to log.
     * @param os The output stream.
     */
    static void log(const char *level,
                    const std::string &message,
                    std::ostream &os) noexcept;

public:
    /**
     * @brief Default constructor.
     */
    DefaultLogger() = default;

    /**
     * @brief Default destructor.
     */
    ~DefaultLogger() = default;

    /**
     * @brief Logs a verbose message.
     * @param message The message to log.
     */
    void verbose(const std::string &message) override;

    /**
     * @brief Logs an info message.
     * @param message The message to log.
     */
    void info(const std::string &message) override;

    /**
     * @brief Logs a warning message.
     * @param message The message to log.
     */
    void warn(const std::string &message) override;

    /**
     * @brief Logs an error message.
     * @param message The message to log.
     */
    void error(const std::string &message) override;

    /**
     * @brief Logs a fatal message.
     * @param message The message to log.
     */
    void fatal(const std::string &message) override;

    /**
     * @brief Logs a debug message.
     * @param message The message to log.
     */
    void debug(const std::string &message) override;
};
}

#endif // DEFAULT_LOGGER_H