/**
 * @file default-services.h
 * @brief This file is for internal use only and must not be in final release in include directories.
 */

#ifndef DEFAULT_SERVICES_H
#define DEFAULT_SERVICES_H
#pragma once
#include <string>
#include <functional>
#include <any>
#include <functional>
#include <memory>
#include <optional>
#include <string>
#include <typeindex>

#include "services-interface.h"

namespace PureIOC {

/**
 * @class DefaultServices
 * @brief A default implementation of the IServices interface.
 * @internal
 */
class DefaultServices final : public IServices {
private:
    struct Impl;
    std::unique_ptr<Impl> _impl;

public:
    /**
     * @brief Default constructor.
     */
    DefaultServices();
    /**
     * @brief Default destructor.
     */
    ~DefaultServices() override;

    DefaultServices(const DefaultServices &) = delete;
    DefaultServices &operator=(const DefaultServices &) = delete;
    DefaultServices(DefaultServices &&) = delete;
    DefaultServices &operator=(DefaultServices &&) = delete;

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
};
}
#endif // DEFAULT_SERVICES_H
