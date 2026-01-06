/**
 * @file locator-mutable.cpp
 * @brief Implements the mutable locator functions for service registration.
 */

#include "locator-mutable.h"
#include "container-manager.h"

namespace PureIOC {
/**
 * @brief Registers a service with the locator.
 * @param type The type of the service.
 * @param factory The factory function that creates the service.
 * @return True if the service was registered, false otherwise.
 */
bool registerService(const std::type_index &type, std::function<std::any()> factory) {
    return getContainer()->registerService(type, std::move(factory));
}

/**
 * @brief Registers a service with the locator with a contract.
 * @param type The type of the service.
 * @param contract The contract for the service.
 * @param factory The factory function that creates the service.
 * @return True if the service was registered, false otherwise.
 */
bool registerService(const std::type_index &type, const std::string &contract, std::function<std::any()> factory) {
    return getContainer()->registerService(type, contract, std::move(factory));
}

/**
 * @brief Registers a lazy singleton service with the locator.
 * @param type The type of the service.
 * @param factory The factory function that creates the service.
 * @return True if the service was registered, false otherwise.
 */
bool registerLazySingleton(const std::type_index &type, std::function<std::any()> factory) {
    return getContainer()->registerLazySingleton(type, std::move(factory));
}

/**
 * @brief Registers a lazy singleton service with the locator with a contract.
 * @param type The type of the service.
 * @param contract The contract for the service.
 * @param factory The factory function that creates the service.
 * @return True if the service was registered, false otherwise.
 */
bool registerLazySingleton(const std::type_index &type, const std::string &contract, std::function<std::any()> factory) {
    return getContainer()->registerLazySingleton(type, contract, std::move(factory));
}

/**
 * @brief Registers a constant service with the locator.
 * @param type The type of the service.
 * @param service The service instance.
 * @return True if the service was registered, false otherwise.
 */
bool registerConstant(const std::type_index &type, std::any service) {
    return getContainer()->registerConstant(type, std::move(service));
}

/**
 * @brief Registers a constant service with the locator with a contract.
 * @param type The type of the service.
 * @param contract The contract for the service.
 * @param service The service instance.
 * @return True if the service was registered, false otherwise.
 */
bool registerConstant(const std::type_index &type, const std::string &contract, std::any service) {
    return getContainer()->registerConstant(type, contract, std::move(service));
}

/**
 * @brief Registers a logger with the locator.
 * @param logger The logger instance.
 * @return True if the logger was registered, false otherwise.
 */
bool registerLogger(std::shared_ptr<ILogger> logger) {
    std::any logger_any = std::any(std::move(logger));
    return getContainer()->registerConstant(std::type_index(typeid(ILogger)), std::move(logger_any));
}
}