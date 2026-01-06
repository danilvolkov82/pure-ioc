#ifndef LOGGER_INTERFACE_H
#define LOGGER_INTERFACE_H
#include <string>

namespace ReactiveUI {
/**
 * @brief An interface for logging.
 */
class ILogger {
protected:
    /**
     * @brief Default constructor.
     */
    ILogger() = default;

public:
    /**
     * @brief Default destructor.
     */
    virtual ~ILogger() = default;
    /**
     * @brief Logs a verbose message.
     * @param message The message to log.
     */
    virtual void verbose(const std::string &message) = 0;
    /**
     * @brief Logs an info message.
     * @param message The message to log.
     */
    virtual void info(const std::string &message) = 0;
    /**
     * @brief Logs a warning message.
     * @param message The message to log.
     */
    virtual void warn(const std::string &message) = 0;
    /**
     * @brief Logs an error message.
     * @param message The message to log.
     */
    virtual void error(const std::string &message) = 0;
    /**
     * @brief Logs a fatal message.
     * @param message The message to log.
     */
    virtual void fatal(const std::string &message) = 0;
    /**
     * @brief Logs a debug message.
     * @param message The message to log.
     */
    virtual void debug(const std::string &message) = 0;
};
}
#endif // LOGGER_INTERFACE_H