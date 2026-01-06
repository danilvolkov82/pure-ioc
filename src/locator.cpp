/**
 * @file locator.cpp
 * @brief Implements the locator functions for service retrieval.
 */

#include "locator.h"
#include "container-manager.h"

namespace PureIOC {
std::optional<std::any> getService(std::type_index type) {
    return getContainer()->getService(type);
}

std::optional<std::any> getService(std::type_index type, const std::string &contract) {
    return getContainer()->getService(type, contract);
}
}
