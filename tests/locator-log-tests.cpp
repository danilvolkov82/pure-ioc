#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <container-manager.h>
#include <locator.h>
#include <logger-interface.h>
#include <services-interface.h>
#include <enable-logger-interface.h>

namespace {

// This mock is also present in other test files. It would be beneficial to
// move it to a shared test utilities header to avoid duplication.
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

class MockLogger final : public PureIOC::ILogger {
public:
    MOCK_METHOD(void, verbose, (const std::string &), (override));
    MOCK_METHOD(void, info, (const std::string &), (override));
    MOCK_METHOD(void, warn, (const std::string &), (override));
    MOCK_METHOD(void, error, (const std::string &), (override));
    MOCK_METHOD(void, fatal, (const std::string &), (override));
    MOCK_METHOD(void, debug, (const std::string &), (override));
};

class LocatorLoggerUsageTest : public ::testing::Test {
protected:
    void SetUp() override {
        mock_services = std::make_shared<MockServices>();
        PureIOC::registerContainer(mock_services);
    }

    void TearDown() override {
        PureIOC::registerContainer(nullptr);
    }

    std::shared_ptr<MockServices> mock_services;
};

TEST_F(LocatorLoggerUsageTest, CanRetrieveAndUseLogger) {
    auto mock_logger = std::make_shared<MockLogger>();
    std::shared_ptr<PureIOC::ILogger> logger_interface = mock_logger;

    // Setup: When getService<ILogger> is called, the container should return our mock logger.
    EXPECT_CALL(*mock_services, getService(testing::Eq(std::type_index(typeid(PureIOC::ILogger)))))
        .WillOnce(testing::Return(std::any(logger_interface)));

    // Action: Retrieve the logger from the locator.
    auto logger = PureIOC::getService<PureIOC::ILogger>();
    ASSERT_NE(logger, nullptr);

    // Verification: Now that we have the logger, we can use it.
    EXPECT_CALL(*mock_logger, info(testing::StrEq("Test message"))).Times(1);
    logger->info("Test message");
}

TEST_F(LocatorLoggerUsageTest, GetServiceReturnsNullptrWhenNoLoggerIsRegistered) {
    // Setup: When getService<ILogger> is called, the container returns nothing.
    EXPECT_CALL(*mock_services, getService(testing::Eq(std::type_index(typeid(PureIOC::ILogger)))))
        .WillOnce(testing::Return(std::nullopt));

    // Action & Verification: Retrieving the logger should yield a nullptr.
    auto logger = PureIOC::getService<PureIOC::ILogger>();
    EXPECT_EQ(logger, nullptr);
}


#include <enable-logger-interface.h>
} // namespace

TEST_F(LocatorLoggerUsageTest, CanBeUsedByEnableLogger) {
    auto mock_logger = std::make_shared<MockLogger>();
    std::shared_ptr<PureIOC::ILogger> logger_interface = mock_logger;

    EXPECT_CALL(*mock_services, getService(testing::Eq(std::type_index(typeid(PureIOC::ILogger)))))
        .WillOnce(testing::Return(std::any(logger_interface)));

    auto logger = PureIOC::IEnableLogger::logger();
    ASSERT_NE(logger, nullptr);
    EXPECT_EQ(logger, mock_logger);
}