#ifndef SERVICES_INTERFACE_H
#define SERVICES_INTERFACE_H
#pragma once
#include <string>
#include <typeindex>
#include <functional>
#include <any>
#include <optional>

namespace ReactiveUI {
/**
 * @brief An interface for a service locator.
 */
class IServices {
protected:
    /**
     * @brief Default constructor.
     */
    IServices() = default;

public:
    /**
     * @brief Default destructor.
     */
    virtual ~IServices() = default;
    /**
     * @brief Gets a service of the specified type.
     * @param type The type of the service to get.
     * @return An optional containing the service if found.
     */
    virtual std::optional<std::any> getService(const std::type_index &type) = 0;
    /**
     * @brief Gets a service of the specified type with a contract.
     * @param type The type of the service to get.
     * @param contract The contract for the service.
     * @return An optional containing the service if found.
     */
    virtual std::optional<std::any> getService(const std::type_index &type, const std::string &contract) = 0;

    /**
     * @brief Registers a factory for a service.
     * @param type The type of the service.
     * @param factory The factory function.
     * @return True if the service was registered, false otherwise.
     */
    virtual bool registerService(const std::type_index &type, std::function<std::any()> factory) = 0;
    /**
     * @brief Registers a factory for a service with a contract.
     * @param type The type of the service.
     * @param contract The contract for the service.
     * @param factory The factory function.
     * @return True if the service was registered, false otherwise.
     */
    virtual bool registerService(const std::type_index &type, const std::string &contract, std::function<std::any()> factory) = 0;
    /**
     * @brief Registers a lazy singleton factory for a service.
     * @param type The type of the service.
     * @param factory The factory function.
     * @return True if the service was registered, false otherwise.
     */
    virtual bool registerLazySingleton(const std::type_index &type, std::function<std::any()> factory) = 0;
    /**
     * @brief Registers a lazy singleton factory for a service with a contract.
     * @param type The type of the service.
     * @param contract The contract for the service.
     * @param factory The factory function.
     * @return True if the service was registered, false otherwise.
     */
    virtual bool registerLazySingleton(const std::type_index &type, const std::string &contract, std::function<std::any()> factory) = 0;
    /**
     * @brief Registers a constant service.
     * @param type The type of the service.
     * @param service The service instance.
     * @return True if the service was registered, false otherwise.
     */
    virtual bool registerConstant(const std::type_index &type, std::any service) = 0;
    /**
     * @brief Registers a constant service with a contract.
     * @param type The type of the service.
     * @param contract The contract for the service.
     * @param service The service instance.
     * @return True if the service was registered, false otherwise.
     */
    virtual bool registerConstant(const std::type_index &type, const std::string &contract, std::any service) = 0;

    /**
     * @brief Unregisters a service.
     * @param type The type of the service.
     */
    virtual void unregisterService(const std::type_index &type) = 0;
    /**
     * @brief Unregisters a service with a contract.
     * @param type The type of the service.
     * @param contract The contract for the service.
     */
    virtual void unregisterService(const std::type_index &type, const std::string &contract) = 0;
};
}
#endif // SERVICES_INTERFACE_H