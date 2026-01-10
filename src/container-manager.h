/**
 * @file container-manager.h
 * @brief This file contains the container manager for service registration and retrieval.
 */

#ifndef CONTAINER_MANAGER_H
#define CONTAINER_MANAGER_H
#pragma once
#include <memory>

#include "services-interface.h"

namespace PureIOC {
    /**
     * @brief Registers a service container.
     * @param services A shared pointer to the service container.
     */
    void registerContainer(std::shared_ptr<IServices> services);

    /**
     * @brief Gets the registered service container.
     * @return A shared pointer to the service container.
     */
    std::shared_ptr<IServices> getContainer();

    template <class T>
    void registerContainer() {
        static_assert(std::is_convertible_v<T *, IServices *>, "T must be convertible to IServices");
        std::shared_ptr<T> container = std::make_shared<T>();
        std::shared_ptr<IServices> services = std::static_pointer_cast<IServices>(std::move(container));

        registerContainer(std::move(services));
    }

    template <class T>
    void registerContainer(std::function<std::shared_ptr<T>()> factory) {
        static_assert(std::is_convertible_v<T *, IServices *>, "T must be convertible to IServices");
        std::shared_ptr<T> container = factory();
        std::shared_ptr<IServices> services = std::static_pointer_cast<IServices>(std::move(container));

        registerContainer(std::move(services));
    }
}

#endif //CONTAINER_MANAGER_H