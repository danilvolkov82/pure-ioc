#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <container-manager.h>
#include <locator-mutable.h>

namespace {
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

class DummyLogger final : public PureIOC::ILogger {
public:
    void verbose(std::string_view, std::string_view) override {}
    void info(std::string_view, std::string_view) override {}
    void warn(std::string_view, std::string_view) override {}
    void warn(std::string_view, std::string_view, std::exception_ptr) override {}
    void warn(std::string_view, const std::exception_ptr &) override {}
    void error(std::string_view, std::string_view) override {}
    void error(std::string_view, std::string_view, std::exception_ptr) override {}
    void error(std::string_view, const std::exception_ptr &) override {}
    void fatal(std::string_view, std::string_view) override {}
    void fatal(std::string_view, std::string_view, std::exception_ptr) override {}
    void fatal(std::string_view, const std::exception_ptr &) override {}
    void debug(std::string_view, std::string_view) override {}
};

struct ITestService {
    virtual ~ITestService() = default;
};
struct TestService : public ITestService {};
struct AnotherService {};

} // namespace

TEST(LocatorMutable, RegisterServiceForwardsToContainer) {
    auto mock = std::make_shared<MockServices>();
    PureIOC::registerContainer(mock);

    EXPECT_CALL(*mock, registerService(testing::Eq(std::type_index(typeid(int))), testing::_))
        .Times(1)
        .WillOnce(testing::Return(true));

    EXPECT_TRUE(PureIOC::registerService(std::type_index(typeid(int)), [] {
        return std::any(std::make_shared<int>(42));
    }));
}

TEST(LocatorMutable, RegisterServiceWithContractForwardsToContainer) {
    auto mock = std::make_shared<MockServices>();
    PureIOC::registerContainer(mock);

    EXPECT_CALL(*mock,
        registerService(testing::Eq(std::type_index(typeid(int))), testing::StrEq("alpha"), testing::_))
        .Times(1)
        .WillOnce(testing::Return(true));

    EXPECT_TRUE(PureIOC::registerService(std::type_index(typeid(int)), "alpha", [] {
        return std::any(std::make_shared<int>(7));
    }));
}

TEST(LocatorMutable, RegisterLazySingletonForwardsToContainer) {
    auto mock = std::make_shared<MockServices>();
    PureIOC::registerContainer(mock);

    EXPECT_CALL(*mock, registerLazySingleton(testing::Eq(std::type_index(typeid(int))), testing::_))
        .Times(1)
        .WillOnce(testing::Return(true));

    EXPECT_TRUE(PureIOC::registerLazySingleton(std::type_index(typeid(int)), [] {
        return std::any(std::make_shared<int>(1));
    }));
}

TEST(LocatorMutable, RegisterLazySingletonWithContractForwardsToContainer) {
    auto mock = std::make_shared<MockServices>();
    PureIOC::registerContainer(mock);

    EXPECT_CALL(*mock,
        registerLazySingleton(testing::Eq(std::type_index(typeid(int))), testing::StrEq("beta"), testing::_))
        .Times(1)
        .WillOnce(testing::Return(true));

    EXPECT_TRUE(PureIOC::registerLazySingleton(std::type_index(typeid(int)), "beta", [] {
        return std::any(std::make_shared<int>(2));
    }));
}

TEST(LocatorMutable, RegisterConstantForwardsToContainer) {
    auto mock = std::make_shared<MockServices>();
    PureIOC::registerContainer(mock);

    auto value = std::make_shared<int>(9);
    EXPECT_CALL(*mock,
        registerConstant(testing::Eq(std::type_index(typeid(int))),
            testing::Truly([&](const std::any &payload) {
                auto ptr = std::any_cast<std::shared_ptr<int>>(&payload);
                return ptr && ptr->get() == value.get();
            })))
        .Times(1)
        .WillOnce(testing::Return(true));

    EXPECT_TRUE(PureIOC::registerConstant(std::type_index(typeid(int)), std::any(value)));
}

TEST(LocatorMutable, RegisterConstantWithContractForwardsToContainer) {
    auto mock = std::make_shared<MockServices>();
    PureIOC::registerContainer(mock);

    auto value = std::make_shared<int>(3);
    EXPECT_CALL(*mock,
        registerConstant(testing::Eq(std::type_index(typeid(int))), testing::StrEq("gamma"),
            testing::Truly([&](const std::any &payload) {
                auto ptr = std::any_cast<std::shared_ptr<int>>(&payload);
                return ptr && ptr->get() == value.get();
            })))
        .Times(1)
        .WillOnce(testing::Return(true));

    EXPECT_TRUE(PureIOC::registerConstant(std::type_index(typeid(int)), "gamma", std::any(value)));
}

TEST(LocatorMutable, RegisterLoggerForwardsToContainer) {
    auto mock = std::make_shared<MockServices>();
    PureIOC::registerContainer(mock);

    auto logger = std::make_shared<DummyLogger>();
    EXPECT_CALL(*mock,
        registerConstant(testing::Eq(std::type_index(typeid(PureIOC::ILogger))),
            testing::Truly([&](const std::any &payload) {
                auto ptr = std::any_cast<std::shared_ptr<PureIOC::ILogger>>(&payload);
                return ptr && ptr->get() == logger.get();
            })))
        .Times(1)
        .WillOnce(testing::Return(true));

    EXPECT_TRUE(PureIOC::registerLogger(logger));
}

// Template functions tests
TEST(LocatorMutable, TemplateRegisterService) {
    auto mock = std::make_shared<MockServices>();
    PureIOC::registerContainer(mock);

    EXPECT_CALL(*mock, registerService(testing::Eq(std::type_index(typeid(ITestService))), testing::_))
        .Times(1)
        .WillOnce(testing::Return(true));

    EXPECT_TRUE((PureIOC::registerService<ITestService, TestService>([] {
        return std::make_shared<TestService>();
    })));
}

TEST(LocatorMutable, TemplateRegisterServiceWithContract) {
    auto mock = std::make_shared<MockServices>();
    PureIOC::registerContainer(mock);

    EXPECT_CALL(*mock,
        registerService(testing::Eq(std::type_index(typeid(ITestService))), testing::StrEq("test"), testing::_))
        .Times(1)
        .WillOnce(testing::Return(true));

    EXPECT_TRUE((PureIOC::registerService<ITestService, TestService>("test", [] {
        return std::make_shared<TestService>();
    })));
}

TEST(LocatorMutable, TemplateRegisterLazySingleton) {
    auto mock = std::make_shared<MockServices>();
    PureIOC::registerContainer(mock);

    EXPECT_CALL(*mock, registerLazySingleton(testing::Eq(std::type_index(typeid(ITestService))), testing::_))
        .Times(1)
        .WillOnce(testing::Return(true));

    EXPECT_TRUE((PureIOC::registerLazySingleton<ITestService, TestService>([] {
        return std::make_shared<TestService>();
    })));
}

TEST(LocatorMutable, TemplateRegisterLazySingletonWithContract) {
    auto mock = std::make_shared<MockServices>();
    PureIOC::registerContainer(mock);

    EXPECT_CALL(*mock,
        registerLazySingleton(testing::Eq(std::type_index(typeid(ITestService))), testing::StrEq("test"), testing::_))
        .Times(1)
        .WillOnce(testing::Return(true));

    EXPECT_TRUE((PureIOC::registerLazySingleton<ITestService, TestService>("test", [] {
        return std::make_shared<TestService>();
    })));
}

TEST(LocatorMutable, TemplateRegisterConstant) {
    auto mock = std::make_shared<MockServices>();
    PureIOC::registerContainer(mock);

    EXPECT_CALL(*mock, registerConstant(testing::Eq(std::type_index(typeid(ITestService))), testing::_))
        .Times(1)
        .WillOnce(testing::Return(true));

    EXPECT_TRUE((PureIOC::registerConstant<ITestService, TestService>(std::make_shared<TestService>())));
}

TEST(LocatorMutable, TemplateRegisterConstantWithContract) {
    auto mock = std::make_shared<MockServices>();
    PureIOC::registerContainer(mock);

    EXPECT_CALL(*mock,
        registerConstant(testing::Eq(std::type_index(typeid(ITestService))), testing::StrEq("test"), testing::_))
        .Times(1)
        .WillOnce(testing::Return(true));

    EXPECT_TRUE((PureIOC::registerConstant<ITestService, TestService>("test", std::make_shared<TestService>())));
}

TEST(LocatorMutable, TemplateRegisterLogger) {
    auto mock = std::make_shared<MockServices>();
    PureIOC::registerContainer(mock);

    EXPECT_CALL(*mock, registerConstant(testing::Eq(std::type_index(typeid(PureIOC::ILogger))), testing::_))
        .Times(1)
        .WillOnce(testing::Return(true));

    EXPECT_TRUE((PureIOC::registerLogger<DummyLogger>()));
}

TEST(LocatorMutable, TemplateRegisterLoggerWithFactory) {
    auto mock = std::make_shared<MockServices>();
    PureIOC::registerContainer(mock);

    EXPECT_CALL(*mock, registerLazySingleton(testing::Eq(std::type_index(typeid(PureIOC::ILogger))), testing::_))
        .Times(1)
        .WillOnce(testing::Return(true));

    EXPECT_TRUE((PureIOC::registerLogger<DummyLogger>([] {
        return std::make_shared<DummyLogger>();
    })));
}
