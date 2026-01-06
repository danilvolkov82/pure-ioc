/**
 * @file locator-mutable.h
 * @brief This file contains the mutable locator for service registration.
 */

#ifndef LOCATOR_MUTABLE_H
#define LOCATOR_MUTABLE_H
#pragma once
#include <memory>
#include <functional>
#include <typeindex>
#include <string>
#include <utility>
#include <typeinfo>
#include <any>
#include <type_traits>

#include "services-interface.h"
#include "logger-interface.h"

namespace ReactiveUI {
/**
 * @brief Registers a service with the locator.
 * @param type The type of the service.
 * @param factory The factory function that creates the service.
 * @return True if the service was registered, false otherwise.
 */
bool registerService(std::type_index type, std::function<std::any()> factory);
/**
 * @brief Registers a service with the locator with a contract.
 * @param type The type of the service.
 * @param contract The contract for the service.
 * @param factory The factory function that creates the service.
 * @return True if the service was registered, false otherwise.
 */
bool registerService(std::type_index type, const std::string &contract, std::function<std::any()> factory);
/**
 * @brief Registers a lazy singleton service with the locator.
 * @param type The type of the service.
 * @param factory The factory function that creates the service.
 * @return True if the service was registered, false otherwise.
 */
bool registerLazySingleton(std::type_index type, std::function<std::any()> factory);
/**
 * @brief Registers a lazy singleton service with the locator with a contract.
 * @param type The type of the service.
 * @param contract The contract for the service.
 * @param factory The factory function that creates the service.
 * @return True if the service was registered, false otherwise.
 */
bool registerLazySingleton(std::type_index type, const std::string &contract, std::function<std::any()> factory);
/**
 * @brief Registers a constant service with the locator.
 * @param type The type of the service.
 * @param service The service instance.
 * @return True if the service was registered, false otherwise.
 */
bool registerConstant(std::type_index type, std::any service);
/**
 * @brief Registers a constant service with the locator with a contract.
 * @param type The type of the service.
 * @param contract The contract for the service.
 * @param service The service instance.
 * @return True if the service was registered, false otherwise.
 */
bool registerConstant(std::type_index type, const std::string &contract, std::any service);

/**
 * @brief Registers a logger with the locator.
 * @param logger The logger instance.
 * @return True if the logger was registered, false otherwise.
 */
bool registerLogger(std::shared_ptr<ILogger> logger);

/**
 * @brief Converts a factory function to a function that returns std::any.
 * @tparam T The type of the service.
 * @tparam RT The return type of the factory.
 * @param factory The factory function.
 * @return A function that returns std::any.
 */
template <class T, class RT>
std::function<std::any()> convertFunction(std::function<std::shared_ptr<RT>()> factory) {
    static_assert(std::is_convertible_v<RT*, T*>, "RT must be convertible to T");

    return [f = std::move(factory)]() mutable -> std::any {
        std::shared_ptr<RT> result = f();
        std::shared_ptr<T> casted = std::static_pointer_cast<T>(std::move(result));

        return std::any(std::move(casted));
    };
}

/**
 * @brief Converts a factory function to a function that returns std::any.
 * @tparam T The type of the service.
 * @param factory The factory function.
 * @return A function that returns std::any.
 */
template <class T>
std::function<std::any()> convertFunction(std::function<std::shared_ptr<T>()> factory) {
    return [f = std::move(factory)]() mutable -> std::any {
        return std::any(f());
    };
}

/**
 * @brief Registers a service with the locator.
 * @tparam T The type of the service.
 * @tparam RT The return type of the factory.
 * @param factory The factory function.
 * @return True if the service was registered, false otherwise.
 */
template <class T, class RT>
bool registerService(std::function<std::shared_ptr<RT>()> factory) {
    return (std::type_index(typeid(T)), convertFunction<T, RT>(std::move(factory)));
}

/**
 * @brief Registers a service with the locator.
 * @tparam T The type of the service.
 * @param factory The factory function.
 * @return True if the service was registered, false otherwise.
 */
template <class T>
bool registerService(std::function<std::shared_ptr<T>()> factory) {
    return registerService(std::type_index(typeid(T)), convertFunction<T>(std::move(factory)));
}

/**
 * @brief Registers a service with the locator with a contract.
 * @tparam T The type of the service.
 * @tparam RT The return type of the factory.
 * @param contract The contract for the service.
 * @param factory The factory function.
 * @return True if the service was registered, false otherwise.
 */
template <class T, class RT>
bool registerService(const std::string &contract, std::function<std::shared_ptr<RT>()> factory) {
    return registerService(std::type_index(typeid(T)), contract, convertFunction<T, RT>(std::move(factory)));
}

/**
 * @brief Registers a service with the locator with a contract.
 * @tparam T The type of the service.
 * @param contract The contract for the service.
 * @param factory The factory function.
 * @return True if the service was registered, false otherwise.
 */
template <class T>
bool registerService(const std::string &contract, std::function<std::shared_ptr<T>()> factory) {
    return registerService(std::type_index(typeid(T)), contract, convertFunction<T>(std::move(factory)));
}

/**
 * @brief Registers a lazy singleton service with the locator.
 * @tparam T The type of the service.
 * @tparam RT The return type of the factory.
 * @param factory The factory function.
 * @return True if the service was registered, false otherwise.
 */
template <class T, class RT>
bool registerLazySingleton(std::function<std::shared_ptr<RT>()> factory) {
    return registerLazySingleton(std::type_index(typeid(T)), convertFunction<T, RT>(std::move(factory)));
}

/**
 * @brief Registers a lazy singleton service with the locator.
 * @tparam T The type of the service.
 * @param factory The factory function.
 * @return True if the service was registered, false otherwise.
 */
template <class T>
bool registerLazySingleton(std::function<std::shared_ptr<T>()> factory) {
    return registerLazySingleton(std::type_index(typeid(T)), convertFunction<T>(std::move(factory)));
}

/**
 * @brief Registers a lazy singleton service with the locator with a contract.
 * @tparam T The type of the service.
 * @tparam RT The return type of the factory.
 * @param contract The contract for the service.
 * @param factory The factory function.
 * @return True if the service was registered, false otherwise.
 */
template <class T, class RT>
bool registerLazySingleton(const std::string &contract, std::function<std::shared_ptr<RT>()> factory) {
    return registerLazySingleton(std::type_index(typeid(T)), contract, convertFunction<T,RT>(std::move(factory)));
}

/**
 * @brief Registers a lazy singleton service with the locator with a contract.
 * @tparam T The type of the service.
 * @param contract The contract for the service.
 * @param factory The factory function.
 * @return True if the service was registered, false otherwise.
 */
template <class T>
bool registerLazySingleton(const std::string &contract, std::function<std::shared_ptr<T>()> factory) {
    return registerLazySingleton(std::type_index(typeid(T)), contract, convertFunction<T>(std::move(factory)));
}

/**
 * @brief Registers a constant service with the locator.
 * @tparam T The type of the service.
 * @param service The service instance.
 * @return True if the service was registered, false otherwise.
 */
template <class T>
bool registerConstant(std::shared_ptr<T> service) {
    return registerConstant(std::type_index(typeid(T)), std::any(std::move(service)));
}

/**
 * @brief Registers a constant service with the locator with a contract.
 * @tparam T The type of the service.
 * @param contract The contract for the service.
 * @param service The service instance.
 * @return True if the service was registered, false otherwise.
 */
template <class T>
bool registerConstant(const std::string &contract, std::shared_ptr<T> service) {
    return registerConstant(std::type_index(typeid(T)), contract, std::any(std::move(service)));
}

/**
 * @brief Registers a constant service with the locator.
 * @tparam T The type of the service.
 * @tparam RT The type of the service instance.
 * @param service The service instance.
 * @return True if the service was registered, false otherwise.
 */
template <class T, class RT>
bool registerConstant(std::shared_ptr<RT> service) {
    static_assert(std::is_convertible_v<RT *, T *>, "RT must be convertible to T");
    std::shared_ptr<T> casted = std::static_pointer_cast<T>(std::move(service));

    return registerConstant(std::type_index(typeid(T)), std::any(std::move(casted)));
}

/**
 * @brief Registers a constant service with the locator with a contract.
 * @tparam T The type of the service.
 * @tparam RT The type of the service instance.
 * @param contract The contract for the service.
 * @param service The service instance.
 * @return True if the service was registered, false otherwise.
 */
template <class T, class RT>
bool registerConstant(const std::string &contract, std::shared_ptr<RT> service) {
    static_assert(std::is_convertible_v<RT *, T *>, "RT must be convertible to T");
    std::shared_ptr<T> casted = std::static_pointer_cast<T>(std::move(service));

    return registerConstant(std::type_index(typeid(T)), contract, std::any(std::move(casted)));
}

/**
 * @brief Registers a logger with the locator.
 * @tparam T The type of the logger.
 * @return True if the logger was registered, false otherwise.
 */
template <class T>
bool registerLogger() {
    std::shared_ptr<T> logger = std::make_shared<T>();
    return registerConstant<ILogger, T>(std::move(logger));
}

/**
 * @brief Registers a logger with the locator.
 * @tparam T The type of the logger.
 * @param factory The factory function that creates the logger.
 * @return True if the logger was registered, false otherwise.
 */
template <class T>
bool registerLogger(std::function<std::shared_ptr<T>()> factory) {
    return registerLazySingleton<ILogger, T>(std::move(factory));
}
}
#endif // LOCATOR_MUTABLE_H