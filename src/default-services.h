/**
 * @file default-services.h
 * @brief This file is for internal use only and must not be in final release in include directories.
 */

#ifndef DEFAULT_SERVICES_H
#define DEFAULT_SERVICES_H
#pragma once
#include <string>
#include <functional>
#include <unordered_map>
#include <mutex>
#include <typeindex>
#include <optional>
#include <any>

#include "services-interface.h"
#include <locator.h>
#include <logger-interface.h>

namespace ReactiveUI {

/**
 * @struct PairHash
 * @brief A hash function for pairs of type_index and optional string.
 */
struct PairHash {
    using is_transparent = void; // optional (if you later want hetero lookup)

    size_t operator()(const std::pair<std::type_index, std::optional<std::string>> &v) const noexcept {
        const size_t h1 = std::hash<std::type_index>{}(v.first);
        const size_t h2 = !v.second ? 0x9e3779b97f4a7c15ull : std::hash<std::string>{}(*v.second);

        return h1 ^ (h2 + 0x9e3779b97f4a7c15ULL + (h1 << 6) + (h1 >> 2));
    }
};

/**
 * @struct PairEq
 * @brief An equality function for pairs of type_index and optional string.
 */
struct PairEq {
    bool operator()(const std::pair<std::type_index, std::optional<std::string>> &a,
        const std::pair<std::type_index, std::optional<std::string>> &b) const noexcept {

        return a.first == b.first && a.second == b.second;
    }
};

using ContractKey = std::optional<std::string>; ///< A type alias for an optional string representing a contract.
using Key = std::pair<std::type_index, ContractKey>; ///< A type alias for a pair of type_index and ContractKey.

/**
 * @brief A type alias for an unordered map with a Key, a value, a PairHash, and a PairEq.
 * @tparam V The value type.
 */
template <class V>
using Map = std::unordered_map<Key, V, PairHash, PairEq>;

/**
 * @class DefaultServices
 * @brief A default implementation of the IServices interface.
 * @internal
 */
class DefaultServices : public IServices {
private:

    mutable std::mutex _mutex; ///< Mutex to protect the service maps.
    mutable Map<std::any> _services; ///< Map of registered singleton services.
    mutable Map<std::function<std::any()>> _singleton_factories;      ///< Map of registered lazy singleton factories.
    mutable Map<std::function<std::any()>> _factories;                ///< Map of registered factories.

public:
    /**
     * @brief Default constructor.
     */
    DefaultServices() = default;
    /**
     * @brief Default destructor.
     */
    virtual ~DefaultServices() = default;

    /**
     * @brief Gets a service of the specified type.
     * @param type The type of the service to get.
     * @return An optional containing the service if found.
     */
    std::optional<std::any> getService(const std::type_index &type) override;
    /**
     * @brief Gets a service of the specified type with a contract.
     * @param type The type of the service to get.
     * @param contract The contract for the service.
     * @return An optional containing the service if found.
     */
    std::optional<std::any> getService(const std::type_index &type, const std::string &contract) override;

    /**
     * @brief Registers a factory for a service.
     * @param type The type of the service.
     * @param factory The factory function.
     * @return True if the service was registered, false otherwise.
     */
    bool registerService(const std::type_index &type, std::function<std::any()> factory) override;
    /**
     * @brief Registers a factory for a service with a contract.
     * @param type The type of the service.
     * @param contract The contract for the service.
     * @param factory The factory function.
     * @return True if the service was registered, false otherwise.
     */
    bool registerService(const std::type_index &type, const std::string &contract, std::function<std::any()> factory) override;
    /**
     * @brief Registers a lazy singleton factory for a service.
     * @param type The type of the service.
     * @param factory The factory function.
     * @return True if the service was registered, false otherwise.
     */
    bool registerLazySingleton(const std::type_index &type, std::function<std::any()> factory) override;
    /**
     * @brief Registers a lazy singleton factory for a service with a contract.
     * @param type The type of the service.
     * @param contract The contract for the service.
     * @param factory The factory function.
     * @return True if the service was registered, false otherwise.
     */
    bool registerLazySingleton(const std::type_index &type, const std::string &contract, std::function<std::any()> factory) override;
    /**
     * @brief Registers a constant service.
     * @param type The type of the service.
     * @param service The service instance.
     * @return True if the service was registered, false otherwise.
     */
    bool registerConstant(const std::type_index &type, std::any service) override;
    /**
     * @brief Registers a constant service with a contract.
     * @param type The type of the service.
     * @param contract The contract for the service.
     * @param service The service instance.
     * @return True if the service was registered, false otherwise.
     */
    bool registerConstant(const std::type_index &type, const std::string &contract, std::any service) override;

    /**
     * @brief Unregisters a service.
     * @param type The type of the service.
     */
    void unregisterService(const std::type_index &type) override;
    /**
     * @brief Unregisters a service with a contract.
     * @param type The type of the service.
     * @param contract The contract for the service.
     */
    void unregisterService(const std::type_index &type, const std::string &contract) override;

private:
    /**
     * @brief Gets a service of the specified type with a contract.
     * @param key The key for the service.
     * @return An optional containing the service if found.
     */
    std::optional<std::any> getService(const Key &key);
    /**
     * @brief Gets a registered constant service.
     * @param key The key for the service.
     * @return An optional containing the service if found.
     */
    std::optional<std::any> getRegisteredConstant(const Key &key) const;
    /**
     * @brief Gets a registered factory.
     * @param map The map of factories.
     * @param key The key for the service.
     * @return An optional containing the factory if found.
     */
    std::optional<std::any> getRegisteredFactory(const Map<std::function<std::any()>> &map, const Key &key) const;

    /**
     * @brief Registers a service.
     * @tparam T The type of the service.
     * @param map The map to register the service in.
     * @param key The key for the service.
     * @param value The value to register.
     * @return True if the service was registered, false otherwise.
     */
    template <class T>
    bool registerService(Map<T> &map, const Key &key, T value) {
        std::lock_guard<std::mutex> lock(_mutex);
        if(map.find(key) == map.end()) {
            map[key] = value;
            return true;
        }

        auto logger = ReactiveUI::getService<ILogger>();
        if (logger) {
            logger->warn("Service is already registered with contract");
        }

        return false;
    }

    /**
     * @brief Unregisters a service.
     * @param key The key for the service.
     */
    void unregisterService(const Key &key);
};
}
#endif // DEFAULT_SERVICES_H