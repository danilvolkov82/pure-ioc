/**
 * @file enable-logger-interface.h
 * @brief This file contains the interface for classes that can provide a logger.
 */

#ifndef ENABLE_LOGGER_INTERFACE_H
#define ENABLE_LOGGER_INTERFACE_H
#include <memory>

#include "logger-interface.h"

namespace PureIOC {
/**
 * @brief An interface for classes that can provide a logger.
 */
class IEnableLogger {
protected:
    /**
     * @brief Default constructor.
     */
    IEnableLogger() = default;
    /**
     * @brief Default destructor.
     */
    virtual ~IEnableLogger() = default;

public:
    /**
     * @brief Gets the logger.
     * @return A shared pointer to the logger.
     */
    static std::shared_ptr<ILogger> logger();
#define LOG(level, ...)                                    \
    do {                                                   \
        auto logger = IEnableLogger::logger();             \
        if (logger) {                                      \
            logger->level(typeid(*this).name(), __VA_ARGS__); \
        }                                                  \
    } while (false)
};
}
#endif
