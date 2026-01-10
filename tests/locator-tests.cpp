#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <string>
#include <memory>
#include <container-manager.h>
#include <services-interface.h>
#include <locator.h>

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

struct TestService {
    virtual ~TestService() = default;
};

struct TestServiceImpl : public TestService {};
}

class LocatorTest : public ::testing::Test {
protected:
    std::shared_ptr<MockServices> mockServices;

    void SetUp() override {
        mockServices = std::make_shared<MockServices>();
        PureIOC::registerContainer(mockServices);
    }

    void TearDown() override {
        PureIOC::registerContainer(nullptr);
    }
};

TEST_F(LocatorTest, GetServiceByTypeIndex) {
    auto instance = std::make_shared<TestServiceImpl>();
    EXPECT_CALL(*mockServices, getService(std::type_index(typeid(TestService))))
        .WillOnce(testing::Return(std::make_any<std::shared_ptr<TestService>>(instance)));

    auto service = PureIOC::getService(std::type_index(typeid(TestService)));
    ASSERT_TRUE(service.has_value());
    auto casted = std::any_cast<std::shared_ptr<TestService>>(*service);
    EXPECT_EQ(instance, casted);
}

TEST_F(LocatorTest, GetServiceByTypeIndexWithContract) {
    auto instance = std::make_shared<TestServiceImpl>();
    const std::string contract = "contract";
    EXPECT_CALL(*mockServices, getService(std::type_index(typeid(TestService)), contract))
        .WillOnce(testing::Return(std::make_any<std::shared_ptr<TestService>>(instance)));

    auto service = PureIOC::getService(std::type_index(typeid(TestService)), contract);
    ASSERT_TRUE(service.has_value());
    auto casted = std::any_cast<std::shared_ptr<TestService>>(*service);
    EXPECT_EQ(instance, casted);
}

TEST_F(LocatorTest, GetServiceByTemplate) {
    auto instance = std::make_shared<TestServiceImpl>();
    EXPECT_CALL(*mockServices, getService(std::type_index(typeid(TestService))))
        .WillOnce(testing::Return(std::make_any<std::shared_ptr<TestService>>(instance)));

    auto service = PureIOC::getService<TestService>();
    ASSERT_NE(service, nullptr);
    EXPECT_EQ(instance, service);
}

TEST_F(LocatorTest, GetServiceByTemplateWithContract) {
    auto instance = std::make_shared<TestServiceImpl>();
    const std::string contract = "contract";
    EXPECT_CALL(*mockServices, getService(std::type_index(typeid(TestService)), contract))
        .WillOnce(testing::Return(std::make_any<std::shared_ptr<TestService>>(instance)));

    auto service = PureIOC::getService<TestService>(contract);
    ASSERT_NE(service, nullptr);
    EXPECT_EQ(instance, service);
}

TEST_F(LocatorTest, GetNonExistentServiceReturnsNull) {
    EXPECT_CALL(*mockServices, getService(std::type_index(typeid(TestService))))
        .WillOnce(testing::Return(std::nullopt));
    
    auto service = PureIOC::getService<TestService>();
    EXPECT_EQ(service, nullptr);
}
