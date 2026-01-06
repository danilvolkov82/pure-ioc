/**
 * @file locator.h
 * @brief This file contains the locator for service retrieval.
 */

#ifndef LOCATOR_H
#define LOCATOR_H
#pragma once
#include <any>
#include <memory>
#include <typeindex>
#include <string>
#include <optional>

namespace ReactiveUI {
/**
 * @brief Gets a service from the locator.
 * @param type The type of the service.
 * @return An optional containing the service if found.
 */
std::optional<std::any> getService(std::type_index type);
/**
 * @brief Gets a service from the locator with a contract.
 * @param type The type of the service.
 * @param contract The contract for the service.
 * @return An optional containing the service if found.
 */
std::optional<std::any> getService(std::type_index type, const std::string &contract);

/**
 * @brief Gets a service from the locator.
 * @tparam T The type of the service.
 * @return A shared pointer to the service, or nullptr if not found.
 */
template <class T>
std::shared_ptr<T> getService() {
    std::optional<std::any> service = getService(std::type_index(typeid(T)));
    if(!service.has_value()) {
        return nullptr;
    }

    return std::any_cast<std::shared_ptr<T>>(std::move(service));
};

/**
 * @brief Gets a service from the locator with a contract.
 * @tparam T The type of the service.
 * @param contract The contract for the service.
 * @return A shared pointer to the service, or nullptr if not found.
 */
template <class T>
std::shared_ptr<T> getService(const std::string &contract) {
    std::optional<std::any> service = getService(std::type_index(typeid(T)), contract);
    if (!service.has_value()) {
        return nullptr;
    }

    return std::any_cast<std::shared_ptr<T>>(std::move(service));
};
}
#endif //LOCATOR_H