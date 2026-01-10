#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <string>
#include <memory>
#include <default-services.h>

struct TestService {
    virtual ~TestService() = default;
    virtual std::string value() = 0;
};

struct TestServiceImpl : public TestService {
    std::string value() override { return "test"; }
};

struct AnotherTestService {
    virtual ~AnotherTestService() = default;
};

struct AnotherTestServiceImpl : public AnotherTestService {};

class DefaultServicesTest : public ::testing::Test {
protected:
    PureIOC::DefaultServices services;
};

TEST_F(DefaultServicesTest, RegisterAndGetService) {
    services.registerService(typeid(TestService), [] {
        return std::make_any<std::shared_ptr<TestService>>(std::make_shared<TestServiceImpl>());
    });

    auto service = services.getService(typeid(TestService));
    ASSERT_TRUE(service.has_value());

    auto casted = std::any_cast<std::shared_ptr<TestService>>(*service);
    EXPECT_EQ("test", casted->value());
}

TEST_F(DefaultServicesTest, GetNonExistentService) {
    auto service = services.getService(typeid(TestService));
    ASSERT_FALSE(service.has_value());
}

TEST_F(DefaultServicesTest, RegisterAndGetServiceWithContract) {
    services.registerService(typeid(TestService), "contract", [] {
        return std::make_any<std::shared_ptr<TestService>>(std::make_shared<TestServiceImpl>());
    });

    auto service = services.getService(typeid(TestService), "contract");
    ASSERT_TRUE(service.has_value());
    auto casted = std::any_cast<std::shared_ptr<TestService>>(*service);
    EXPECT_EQ("test", casted->value());

    auto serviceWithoutContract = services.getService(typeid(TestService));
    ASSERT_FALSE(serviceWithoutContract.has_value());
}

TEST_F(DefaultServicesTest, RegisterServiceTwice) {
    bool firstRegistered = services.registerService(typeid(TestService), [] {
        return std::make_any<std::shared_ptr<TestService>>(std::make_shared<TestServiceImpl>());
    });
    ASSERT_TRUE(firstRegistered);

    bool secondRegistered = services.registerService(typeid(TestService), [] {
        return std::make_any<std::shared_ptr<TestService>>(std::make_shared<TestServiceImpl>());
    });
    ASSERT_FALSE(secondRegistered);
}

TEST_F(DefaultServicesTest, RegisterConstant) {
    auto instance = std::make_shared<TestServiceImpl>();
    services.registerConstant(typeid(TestService), std::make_any<std::shared_ptr<TestService>>(instance));
    
    auto service = services.getService(typeid(TestService));
    ASSERT_TRUE(service.has_value());
    auto casted = std::any_cast<std::shared_ptr<TestService>>(*service);
    EXPECT_EQ(instance, casted);
}

TEST_F(DefaultServicesTest, RegisterLazySingleton) {
    int factory_call_count = 0;
    services.registerLazySingleton(typeid(TestService), [&] {
        factory_call_count++;
        return std::make_any<std::shared_ptr<TestService>>(std::make_shared<TestServiceImpl>());
    });

    EXPECT_EQ(0, factory_call_count);
    
    auto service1 = services.getService(typeid(TestService));
    EXPECT_EQ(1, factory_call_count);

    auto service2 = services.getService(typeid(TestService));
    EXPECT_EQ(1, factory_call_count);

    auto casted1 = std::any_cast<std::shared_ptr<TestService>>(*service1);
    auto casted2 = std::any_cast<std::shared_ptr<TestService>>(*service2);
    EXPECT_EQ(casted1, casted2);
}

TEST_F(DefaultServicesTest, FactoryIsCalledOnEachGet) {
    int factory_call_count = 0;
    services.registerService(typeid(TestService), [&] {
        factory_call_count++;
        return std::make_any<std::shared_ptr<TestService>>(std::make_shared<TestServiceImpl>());
    });

    EXPECT_EQ(0, factory_call_count);
    
    services.getService(typeid(TestService));
    EXPECT_EQ(1, factory_call_count);

    services.getService(typeid(TestService));
    EXPECT_EQ(2, factory_call_count);
}

TEST_F(DefaultServicesTest, UnregisterService) {
    services.registerService(typeid(TestService), [] {
        return std::make_any<std::shared_ptr<TestService>>(std::make_shared<TestServiceImpl>());
    });

    auto service = services.getService(typeid(TestService));
    ASSERT_TRUE(service.has_value());

    services.unregisterService(typeid(TestService));
    auto serviceAfterUnregister = services.getService(typeid(TestService));
    ASSERT_FALSE(serviceAfterUnregister.has_value());
}

TEST_F(DefaultServicesTest, UnregisterServiceWithContract) {
    services.registerService(typeid(TestService), "contract", [] {
        return std::make_any<std::shared_ptr<TestService>>(std::make_shared<TestServiceImpl>());
    });

    auto service = services.getService(typeid(TestService), "contract");
    ASSERT_TRUE(service.has_value());

    services.unregisterService(typeid(TestService), "contract");
    auto serviceAfterUnregister = services.getService(typeid(TestService), "contract");
    ASSERT_FALSE(serviceAfterUnregister.has_value());
}
