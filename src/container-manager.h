/**
 * @file container-manager.h
 * @brief This file contains the container manager for service registration and retrieval.
 */

#ifndef CONTAINER_MANAGER_H
#define CONTAINER_MANAGER_H
#pragma once
#include <memory>

#include "services-interface.h"

namespace ReactiveUI {
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
}

#endif //CONTAINER_MANAGER_H