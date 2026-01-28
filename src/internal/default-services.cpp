/**
 * @file default-services.cpp
 * @brief This file is for internal use only and must not be in final release in include directories.
 */

#include "internal/default-services.h"

#include <memory>
#include <mutex>
#include <shared_mutex>
#include <unordered_map>
#include <utility>

#include <locator.h>
#include <logger-interface.h>

using namespace PureIOC;

namespace {

/**
 * @struct PairHash
 * @brief A hash function for pairs of type_index and optional string.
 */
struct PairHash {
    using is_transparent = void;

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

using ContractKey = std::optional<std::string>;
using Key = std::pair<std::type_index, ContractKey>;

/**
 * @brief A type alias for an unordered map with a Key, a value, a PairHash, and a PairEq.
 * @tparam V The value type.
 */
template <class V>
using Map = std::unordered_map<Key, V, PairHash, PairEq>;

} // namespace

struct DefaultServices::Impl {
    mutable std::shared_mutex mutex;
    mutable Map<std::any> services;
    mutable Map<std::function<std::any()>> singleton_factories;
    mutable Map<std::shared_ptr<std::once_flag>> singleton_once_flags;
    mutable Map<std::function<std::any()>> factories;

    std::optional<std::any> getService(const Key &key);
    std::optional<std::any> getLazySingleton(const Key &key);
    std::optional<std::any> getRegisteredConstant(const Key &key) const;
    std::optional<std::any> getRegisteredFactory(const Map<std::function<std::any()>> &map, const Key &key) const;

    template <class T>
    bool registerService(Map<T> &map, const Key &key, T value) {
        std::unique_lock<std::shared_mutex> lock(mutex);
        if (map.find(key) == map.end()) {
            map[key] = std::move(value);
            return true;
        }

        auto logger = PureIOC::getService<ILogger>();
        if (logger) {
            logger->warn<DefaultServices>("Service is already registered with contract");
        }

        return false;
    }

    bool registerLazySingleton(const Key &key, std::function<std::any()> factory) {
        std::unique_lock<std::shared_mutex> lock(mutex);
        if (singleton_factories.find(key) == singleton_factories.end()) {
            singleton_factories[key] = std::move(factory);
            singleton_once_flags[key] = std::make_shared<std::once_flag>();
            return true;
        }

        auto logger = PureIOC::getService<ILogger>();
        if (logger) {
            logger->warn<DefaultServices>("Service is already registered with contract");
        }

        return false;
    }

    void unregisterService(const Key &key);
};

DefaultServices::DefaultServices()
    : _impl(std::make_unique<DefaultServices::Impl>()) {}

DefaultServices::~DefaultServices() = default;

/**
 * @brief Gets the service.
 * @param type The type of the service.
 * @return The service.
 */
std::optional<std::any>
DefaultServices::getService(const std::type_index &type) {
    Key key(type, std::nullopt);
    return this->_impl->getService(key);
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
    return this->_impl->getService(key);
}

/**
 * @brief Gets the registered constant.
 * @param key The key.
 * @return The registered constant.
 */
std::optional<std::any>
DefaultServices::Impl::getRegisteredConstant(const Key &key) const {
    std::shared_lock<std::shared_mutex> lock(mutex);
    auto it = services.find(key);
    if (it != services.end()) {
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
DefaultServices::Impl::getRegisteredFactory(const Map<std::function<std::any()>> &map, const Key &key) const {
    std::optional<std::function<std::any()>> factory = std::nullopt;
    {
        std::shared_lock<std::shared_mutex> lock(mutex);
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
DefaultServices::Impl::getService(const Key &key) {
    auto service = getRegisteredConstant(key);
    if (service) {
        return service;
    }

    service = getLazySingleton(key);
    if (service) {
        return service;
    }

    return getRegisteredFactory(factories, key);
}

/**
 * @brief Gets the lazy singleton.
 * @param key The key.
 * @return The service.
 */
std::optional<std::any>
DefaultServices::Impl::getLazySingleton(const Key &key) {
    std::function<std::any()> factory;
    std::shared_ptr<std::once_flag> once_flag;
    {
        std::shared_lock<std::shared_mutex> lock(mutex);
        auto it = singleton_factories.find(key);
        if (it == singleton_factories.end()) {
            return std::nullopt;
        }
        factory = it->second;

        auto flag_it = singleton_once_flags.find(key);
        if (flag_it != singleton_once_flags.end()) {
            once_flag = flag_it->second;
        }
    }

    if (!once_flag) {
        std::unique_lock<std::shared_mutex> lock(mutex);
        auto &flag_slot = singleton_once_flags[key];
        if (!flag_slot) {
            flag_slot = std::make_shared<std::once_flag>();
        }
        once_flag = flag_slot;
    }

    std::call_once(*once_flag, [&] {
        registerService<std::any>(services, key, factory());
    });

    return getRegisteredConstant(key);
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
    return this->_impl->registerService<std::function<std::any()>>(
        this->_impl->factories, key, std::move(factory));
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
    return this->_impl->registerService<std::function<std::any()>>(
        this->_impl->factories, key, std::move(factory));
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
    return this->_impl->registerLazySingleton(key, std::move(factory));
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
    return this->_impl->registerLazySingleton(key, std::move(factory));
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
    return this->_impl->registerService<std::any>(this->_impl->services, key, std::move(service));
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
    return this->_impl->registerService<std::any>(this->_impl->services, key, std::move(service));
}

/**
 * @brief Unregisters the service.
 * @param type The type of the service.
 */
void
DefaultServices::unregisterService(const std::type_index &type) {
    Key key(type, std::nullopt);
    this->_impl->unregisterService(key);
}

/**
 * @brief Unregisters the service.
 * @param type The type of the service.
 * @param contract The contract.
 */
void
DefaultServices::unregisterService(const std::type_index &type, const std::string &contract) {
    Key key(type, std::optional<std::string>(contract));
    this->_impl->unregisterService(key);
}

/**
 * @brief Unregisters the service.
 * @param key The key.
 */
void
DefaultServices::Impl::unregisterService(const Key &key) {
    std::unique_lock<std::shared_mutex> lock(mutex);

    services.erase(key);
    singleton_factories.erase(key);
    singleton_once_flags.erase(key);
    factories.erase(key);
}
