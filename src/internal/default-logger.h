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

namespace PureIOC::internal {
/**
 * @brief A default logger implementation that writes to standard output and standard error.
 * @internal
 */
class DefaultLogger final : public ILogger {
public:
    using ILogger::verbose;
    using ILogger::info;
    using ILogger::warn;
    using ILogger::error;
    using ILogger::fatal;
    using ILogger::debug;

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
    LOG_METHOD_MESSAGE(verbose) override;

    /**
     * @brief Logs an info message.
     * @param message The message to log.
     */
    LOG_METHOD_MESSAGE(info) override;

    /**
     * @brief Logs a warning message.
     * @param message The message to log.
     */
    LOG_METHOD_MESSAGE(warn) override;
    
    /**
     * @brief Logs a warning message with exception details.
     * @param message The message to log.
     * @param e The exception to log.
     */
    LOG_METHOD_MESSAGE_AND_EXCEPTION(warn) override;

    /**
     * @brief Logs a warning exception.
     * @param e The exception to log.
     */
    LOG_METHOD_EXCEPTION(warn) override;

    /**
     * @brief Logs an error message.
     * @param message The message to log.
     */
    LOG_METHOD_MESSAGE(error) override;

    /**
     * @brief Logs an error message with exception details.
     * @param message The contextual message to log.
     * @param e The exception to log.
     */
    LOG_METHOD_MESSAGE_AND_EXCEPTION(error) override;

    /**
     * @brief Logs an error exception.
     * @param e The exception to log.
     */
    LOG_METHOD_EXCEPTION(error) override;

    /**
     * @brief Logs a fatal message.
     * @param message The message to log.
     */
    LOG_METHOD_MESSAGE(fatal) override;
    
    /**
     * @brief Logs a fatal message with exception details.
     * @param message The contextual message to log.
     * @param e The exception to log.
     */
    LOG_METHOD_MESSAGE_AND_EXCEPTION(fatal) override;

    /**
     * @brief Logs a fatal exception.
     * @param e The exception to log.
     */
    LOG_METHOD_EXCEPTION(fatal) override;

    /**
     * @brief Logs a debug message.
     * @param message The message to log.
     */
    LOG_METHOD_MESSAGE(debug) override;
};
}

#endif // DEFAULT_LOGGER_H
