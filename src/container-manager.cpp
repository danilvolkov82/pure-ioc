/**
 * @file container-manager.cpp
 * @brief Manages the global service container.
 */

#include <mutex>

#include "container-manager.h"
#include "default-services.h"

namespace PureIOC {
namespace {
    std::mutex g_mutex; ///< Mutex to protect the global container.
    std::shared_ptr<IServices> g_container; ///< The global service container.
}

void registerContainer(std::shared_ptr<IServices> services) {
    std::lock_guard<std::mutex> lock(g_mutex);
    g_container = services ? std::move(services) : std::make_shared<DefaultServices>();
}

std::shared_ptr<IServices> getContainer() {
    std::lock_guard<std::mutex> lock(g_mutex);
    if (!g_container) {
        g_container = std::make_shared<DefaultServices>();
    }

    return g_container;
}
}
