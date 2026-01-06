/**
 * @file default-services.cpp
 * @brief This file is for internal use only and must not be in final release in include directories.
 */

#include "default-services.h"

using namespace PureIOC;

/**
 * @brief Gets the service.
 * @param type The type of the service.
 * @return The service.
 */
std::optional<std::any>
DefaultServices::getService(const std::type_index &type) {
    Key key(type, std::nullopt);
    return this->getService(key);
}

/**
 * @brief Gets the service.
 * @param type The type of the service.
 * @param contract The contract.
 * @return The service.
 */
std::optional<std::any>
DefaultServices::getService(const std::type_index &type, const std::string &contract) {
    Key key(type, std::optional<std::string>(contract));
    return this->getService(key);
}

/**
 * @brief Gets the registered constant.
 * @param key The key.
 * @return The registered constant.
 */
std::optional<std::any>
DefaultServices::getRegisteredConstant(const Key &key) const {
    std::lock_guard<std::mutex> lock(this->_mutex);
    auto it = this->_services.find(key);
    if(it != this->_services.end()) {
        return std::optional<std::any>(it->second);
    }

    return std::nullopt;
}

/**
 * @brief Gets the registered factory.
 * @param map The map.
 * @param key The key.
 * @return The registered factory.
 */
std::optional<std::any>
DefaultServices::getRegisteredFactory(const Map<std::function<std::any()>> &map, const Key &key) const {
    std::optional<std::function<std::any()>> factory = std::nullopt;
    {
        std::lock_guard<std::mutex> lock(this->_mutex);
        auto it = map.find(key);
        if (it != map.end()) {
            factory = std::optional<std::function<std::any()>>(it->second);
        }
    }

    return factory ? std::optional<std::any>(factory.value()()) : std::nullopt;
}

/**
 * @brief Gets the service.
 * @param key The key.
 * @return The service.
 */
std::optional<std::any>
DefaultServices::getService(const Key &key) {
    auto service = this->getRegisteredConstant(key);
    if (service) {
        return service;
    }

    service = this->getRegisteredFactory(this->_singleton_factories, key);
    if (service) {
        this->registerService<std::any>(this->_services, key, service.value());
        return this->getRegisteredConstant(key);
    }

    return this->getRegisteredFactory(this->_factories, key);
}

/**
 * @brief Registers the service.
 * @param type The type of the service.
 * @param factory The factory.
 * @return True if the service was registered, false otherwise.
 */
bool
DefaultServices::registerService(const std::type_index &type, std::function<std::any()> factory) {
    Key key(type, std::nullopt);
    return this->registerService<std::function<std::any()>>(this->_factories, key, std::move(factory));
}

/**
 * @brief Registers the service.
 * @param type The type of the service.
 * @param contract The contract.
 * @param factory The factory.
 * @return True if the service was registered, false otherwise.
 */
bool
DefaultServices::registerService(const std::type_index &type, const std::string &contract, std::function<std::any()> factory) {
    Key key(type, std::optional<std::string>(contract));
    return this->registerService<std::function<std::any()>>(this->_factories, key, std::move(factory));
}

/**
 * @brief Registers the lazy singleton.
 * @param type The type of the service.
 * @param factory The factory.
 * @return True if the lazy singleton was registered, false otherwise.
 */
bool
DefaultServices::registerLazySingleton(const std::type_index &type, std::function<std::any()> factory) {
    Key key(type, std::nullopt);
    return this->registerService<std::function<std::any()>>(this->_singleton_factories, key, std::move(factory));
}

/**
 * @brief Registers the lazy singleton.
 * @param type The type of the service.
 * @param contract The contract.
 * @param factory The factory.
 * @return True if the lazy singleton was registered, false otherwise.
 */
bool
DefaultServices::registerLazySingleton(const std::type_index &type, const std::string &contract, std::function<std::any()> factory) {
    Key key(type, std::optional<std::string>(contract));
    return this->registerService<std::function<std::any()>>(this->_singleton_factories, key, std::move(factory));
}

/**
 * @brief Registers the constant.
 * @param type The type of the service.
 * @param service The service.
 * @return True if the constant was registered, false otherwise.
 */
bool
DefaultServices::registerConstant(const std::type_index &type, std::any service) {
    Key key(type, std::nullopt);
    return this->registerService<std::any>(this->_services, key, std::move(service));
}

/**
 * @brief Registers the constant.
 * @param type The type of the service.
 * @param contract The contract.
 * @param service The service.
 * @return True if the constant was registered, false otherwise.
 */
bool
DefaultServices::registerConstant(const std::type_index &type, const std::string &contract, std::any service) {
    Key key(type, std::optional<std::string>(contract));
    return this->registerService<std::any>(this->_services, key, std::move(service));
}

/**
 * @brief Unregisters the service.
 * @param type The type of the service.
 */
void
DefaultServices::unregisterService(const std::type_index &type) {
    Key key(type, std::nullopt);
    this->unregisterService(key);
}

/**
 * @brief Unregisters the service.
 * @param type The type of the service.
 * @param contract The contract.
 */
void
DefaultServices::unregisterService(const std::type_index &type, const std::string &contract) {
    Key key(type, std::optional<std::string>(contract));
    this->unregisterService(key);
}

/**
 * @brief Unregisters the service.
 * @param key The key.
 */
void
DefaultServices::unregisterService(const Key &key) {
    std::lock_guard<std::mutex> lock(this->_mutex);

    this->_services.erase(key);
    this->_singleton_factories.erase(key);
    this->_factories.erase(key);
}