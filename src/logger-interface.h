/**
 * @file logger-interface.h
 * @brief Logger abstraction for PureIOC.
 */
#ifndef LOGGER_INTERFACE_H
#define LOGGER_INTERFACE_H
#include <string_view>
#include <exception>
#include <typeinfo>

namespace PureIOC {
/**
 * @def LOG_METHOD_MESSAGE
 * @brief Declares a logger method that accepts a tag and message.
 */
#ifndef LOG_METHOD_MESSAGE
#define LOG_METHOD_MESSAGE(name) \
    void name(std::string_view tag, std::string_view message)
#endif

/**
 * @def LOG_METHOD_MESSAGE_AND_EXCEPTION
 * @brief Declares a logger method that accepts a tag, message, and exception.
 */
#ifndef LOG_METHOD_MESSAGE_AND_EXCEPTION
#define LOG_METHOD_MESSAGE_AND_EXCEPTION(name) \
    void name(std::string_view tag, std::string_view message, const std::exception_ptr &e)
#endif

/**
 * @def LOG_METHOD_EXCEPTION
 * @brief Declares a logger method that accepts a tag and exception.
 */
#ifndef LOG_METHOD_EXCEPTION
#define LOG_METHOD_EXCEPTION(name) \
    void name(std::string_view tag, const std::exception_ptr &e)
#endif

/**
 * @brief Abstract logging interface used by IOC components.
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
     * @param tag Logical source/category tag.
     * @param message The message to log.
     */
    virtual LOG_METHOD_MESSAGE(verbose) = 0;
    /**
     * @brief Logs an info message.
     * @param tag Logical source/category tag.
     * @param message The message to log.
     */
    virtual LOG_METHOD_MESSAGE(info) = 0;
    /**
     * @brief Logs a warning message.
     * @param tag Logical source/category tag.
     * @param message The message to log.
     */
    virtual LOG_METHOD_MESSAGE(warn) = 0;
    /**
     * @brief Logs a warning message with exception details.
     * @param tag Logical source/category tag.
     * @param message The message to log.
     * @param e The exception to log.
     */
    virtual LOG_METHOD_MESSAGE_AND_EXCEPTION(warn) = 0;
    /**
     * @brief Logs a warning exception.
     * @param tag Logical source/category tag.
     * @param e The exception to log.
     */
    virtual LOG_METHOD_EXCEPTION(warn) = 0;
    /**
     * @brief Logs an error message.
     * @param tag Logical source/category tag.
     * @param message The message to log.
     */
    virtual LOG_METHOD_MESSAGE(error) = 0;
    /**
     * @brief Logs an error message with exception details.
     * @param tag Logical source/category tag.
     * @param message The contextual message to log.
     * @param e The exception to log.
     */
    virtual LOG_METHOD_MESSAGE_AND_EXCEPTION(error) = 0;
    /**
     * @brief Logs an error exception.
     * @param tag Logical source/category tag.
     * @param e The exception to log.
     */
    virtual LOG_METHOD_EXCEPTION(error) = 0;
    /**
     * @brief Logs a fatal message.
     * @param tag Logical source/category tag.
     * @param message The message to log.
     */
    virtual LOG_METHOD_MESSAGE(fatal) = 0;
    /**
     * @brief Logs a fatal message with exception details.
     * @param tag Logical source/category tag.
     * @param message The contextual message to log.
     * @param e The exception to log.
     */
    virtual LOG_METHOD_MESSAGE_AND_EXCEPTION(fatal) = 0;
    /**
     * @brief Logs a fatal exception.
     * @param tag Logical source/category tag.
     * @param e The exception to log.
     */
    virtual LOG_METHOD_EXCEPTION(fatal) = 0;
    /**
     * @brief Logs a debug message.
     * @param tag Logical source/category tag.
     * @param message The message to log.
     */
    virtual LOG_METHOD_MESSAGE(debug) = 0;

    /**
     * @brief Defines templated message helpers that use `typeid(T).name()` as tag.
     */
#define LOG_TEMPLATE_MESSAGE(mname)         \
template <class T>                          \
void mname(std::string_view message) {    \
    this->mname(typeid(T).name(), message); \
}
    /**
     * @brief Defines templated message+exception helpers with type-based tag.
     */
#define LOG_TEMPLATE_MESSAGE_AND_EXCEPTION(mname)                 \
template <class T>                                                \
void mname(std::string_view message, const std::exception_ptr &e) { \
    this->mname(typeid(T).name(), message, e);                    \
}
    /**
     * @brief Defines templated exception-only helpers with type-based tag.
     */
#define LOG_TEMPLATE_EXCEPTION(mname) \
template <class T>                    \
void mname(const std::exception_ptr &e) { \
    this->mname(typeid(T).name(), e); \
}

    LOG_TEMPLATE_MESSAGE(verbose)
    LOG_TEMPLATE_MESSAGE(info)
    LOG_TEMPLATE_MESSAGE(warn)
    LOG_TEMPLATE_MESSAGE_AND_EXCEPTION(warn)
    LOG_TEMPLATE_EXCEPTION(warn)
    LOG_TEMPLATE_MESSAGE(error)
    LOG_TEMPLATE_MESSAGE_AND_EXCEPTION(error)
    LOG_TEMPLATE_EXCEPTION(error)
    LOG_TEMPLATE_MESSAGE(fatal)
    LOG_TEMPLATE_MESSAGE_AND_EXCEPTION(fatal)
    LOG_TEMPLATE_EXCEPTION(fatal)
    LOG_TEMPLATE_MESSAGE(debug)

#undef LOG_TEMPLATE_MESSAGE
#undef LOG_TEMPLATE_MESSAGE_AND_EXCEPTION
#undef LOG_TEMPLATE_EXCEPTION
};
}
#endif // LOGGER_INTERFACE_H
