#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <container-manager.h>
#include <services-interface.h>

namespace {
class DummyServices final : public PureIOC::IServices {
public:
    std::optional<std::any> getService(const std::type_index &) override {
        return std::nullopt;
    }

    std::optional<std::any> getService(const std::type_index &, const std::string &) override {
        return std::nullopt;
    }

    bool registerService(const std::type_index &, std::function<std::any()>) override {
        return false;
    }

    bool registerService(const std::type_index &, const std::string &, std::function<std::any()>) override {
        return false;
    }

    bool registerLazySingleton(const std::type_index &, std::function<std::any()>) override {
        return false;
    }

    bool registerLazySingleton(const std::type_index &, const std::string &, std::function<std::any()>) override {
        return false;
    }

    bool registerConstant(const std::type_index &, std::any) override {
        return false;
    }

    bool registerConstant(const std::type_index &, const std::string &, std::any) override {
        return false;
    }

    void unregisterService(const std::type_index &) override {}

    void unregisterService(const std::type_index &, const std::string &) override {}
};

class MockServices final : public PureIOC::IServices {
public:
    MOCK_METHOD(std::optional<std::any>, getService, (const std::type_index &), (override));
    MOCK_METHOD(std::optional<std::any>, getService, (const std::type_index &, const std::string &), (override));
    MOCK_METHOD(bool, registerService, (const std::type_index &, std::function<std::any()>), (override));
    MOCK_METHOD(bool, registerService, (const std::type_index &, const std::string &, std::function<std::any()>), (override));
    MOCK_METHOD(bool, registerLazySingleton, (const std::type_index &, std::function<std::any()>), (override));
    MOCK_METHOD(bool, registerLazySingleton, (const std::type_index &, const std::string &, std::function<std::any()>), (override));
    MOCK_METHOD(bool, registerConstant, (const std::type_index &, std::any), (override));
    MOCK_METHOD(bool, registerConstant, (const std::type_index &, const std::string &, std::any), (override));
    MOCK_METHOD(void, unregisterService, (const std::type_index &), (override));
    MOCK_METHOD(void, unregisterService, (const std::type_index &, const std::string &), (override));
};
} // namespace

TEST(ContainerManager, DefaultContainerCreatedWhenNullRegistered) {
    PureIOC::registerContainer(nullptr);
    auto container = PureIOC::getContainer();
    ASSERT_NE(container, nullptr);

    auto container_again = PureIOC::getContainer();
    EXPECT_EQ(container_again.get(), container.get());
}

TEST(ContainerManager, UsesRegisteredContainer) {
    auto custom = std::make_shared<DummyServices>();
    PureIOC::registerContainer(custom);
    auto container = PureIOC::getContainer();
    ASSERT_NE(container, nullptr);
    EXPECT_EQ(container.get(), custom.get());
}

TEST(ContainerManager, NullRegistrationReplacesCustomContainer) {
    auto custom = std::make_shared<DummyServices>();
    PureIOC::registerContainer(custom);
    auto before_reset = PureIOC::getContainer();
    ASSERT_EQ(before_reset.get(), custom.get());

    PureIOC::registerContainer(nullptr);
    auto after_reset = PureIOC::getContainer();
    ASSERT_NE(after_reset, nullptr);
    EXPECT_NE(after_reset.get(), custom.get());
}

TEST(ContainerManager, UsesRegisteredContainerMethods) {
    auto mock = std::make_shared<MockServices>();
    PureIOC::registerContainer(mock);

    EXPECT_CALL(*mock, getService(testing::_))
        .Times(1)
        .WillOnce(testing::Return(std::optional<std::any>{}));

    auto container = PureIOC::getContainer();
    container->getService(std::type_index(typeid(int)));
}
