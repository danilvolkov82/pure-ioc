# PureIoC

PureIoC is a lightweight, header-only, dependency injection (IoC) container for C++17. It is designed to be simple, easy to use, and non-intrusive.

## Features

- **Lightweight and Header-Only:** Simply include `pure-ioc.h` in your project.
- **Service Registration:** Register services as transient, lazy singletons, or constants.
- **Service Retrieval:** Retrieve services by type or by a string-based contract.
- **Custom Containers:** Implement your own service container by inheriting from `PureIOC::IServices`.
- **Logging:** Provides a simple logging interface.

## Getting Started

### Prerequisites

- C++17 compatible compiler
- CMake 3.10 or higher

### Building

To build the library, run the following command:

```bash
./build.sh
```

To build with tests, run the following command:

```bash
./build-with-tests.sh
```

### Usage

To use PureIoC in your project, you just need to include the `pure-ioc.h` header file. If you are not using a single-header build, include `locator.h` and `locator-mutable.h`.

Here is a simple example of how to use PureIoC:

```cpp
#include <iostream>
#include <string>
#include <memory>

#include <pure-ioc/locator.h>
#include <pure-ioc/locator-mutable.h>

// 1. Define your interfaces and implementations

struct IMyService {
    virtual ~IMyService() = default;
    virtual void doSomething() = 0;
};

struct MyService : public IMyService {
    void doSomething() override {
        std::cout << "MyService is doing something." << std::endl;
    }
};

int main() {
    // 2. Register your services

    // Register as a transient service
    PureIOC::registerService<IMyService, MyService>([] {
        return std::make_shared<MyService>();
    });

    // 3. Retrieve your services

    auto myService = PureIOC::getService<IMyService>();

    if (myService) {
        myService->doSomething();
    }

    return 0;
}
```

## API Overview

### Service Registration

All registration functions are available in the `PureIOC` namespace and are defined in `locator-mutable.h`.

- **`registerService<T, RT>(factory)`:** Registers a transient service. A new instance is created every time it is requested.
- **`registerLazySingleton<T, RT>(factory)`:** Registers a service that is created only once when it is first requested.
- **`registerConstant<T, RT>(instance)`:** Registers a service with a pre-existing instance.

You can also register services with a string contract:

- **`registerService<T, RT>(contract, factory)`**
- **`registerLazySingleton<T, RT>(contract, factory)`**
- **`registerConstant<T, RT>(contract, instance)`**

### Service Retrieval

All retrieval functions are available in the `PureIOC` namespace and are defined in `locator.h`.

- **`getService<T>()`:** Retrieves a service by its type `T`.
- **`getService<T>(contract)`:** Retrieves a service by its type `T` and a string contract.

### Logging

PureIoC provides a simple logging mechanism.

- **`ILogger`:** An interface for logging.
- **`registerLogger(logger)`:** Registers a custom logger.
- **`IEnableLogger`:** An interface for classes that can provide a logger.

### Custom Containers

You can provide your own container implementation by inheriting from `PureIOC::IServices` and registering it with `PureIOC::registerContainer`.

## License

This project is licensed under the LGPL License - see the [LICENSE](LICENSE) file for details.
