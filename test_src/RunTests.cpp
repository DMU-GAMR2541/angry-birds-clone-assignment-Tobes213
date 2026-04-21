#include <gtest/gtest.h>
#include "Enemy.h"
#include "slingshot.h"

/// <summary>
///Taken from the GoogleTest primer. 
/// </summary>

// The fixture for testing class Foo.
class EnemyTest : public testing::Test {
public:
    std::unique_ptr<Enemy> enemy;
protected:
    // You can remove any or all of the following functions if their bodies would
    // be empty.

    EnemyTest() {
        // You can do set-up work for each test here.
                    
    }

    ~EnemyTest() override {
        // You can do clean-up work that doesn't throw exceptions here.
    }

    // If the constructor and destructor are not enough for setting up
    // and cleaning up each test, you can define the following methods:

    void SetUp() override {
        // Code here will be called immediately after the constructor (right
        // before each test).
        enemy = std::make_unique<Enemy>(50); // All enemnies in this test suite start with 50 HP.
                    
    }

    void TearDown() override {
        // Code here will be called immediately after each test (right
        // before the destructor).
    }


};

//A single test, not a fixture. No setup is called.
TEST(Enemy, First_test) {
    Enemy e(100);
    EXPECT_GT(e.getHealth(), 100);
    SUCCEED() << "Test test passed";
    FAIL() << "Test didn't pass";
}

TEST_F(EnemyTest, LethalDamagePopsPig) {
    enemy->takeDamage(60);
    EXPECT_TRUE(enemy->checkIfPopped());
}

class SlingshotTest : public testing::Test {
public:
    std::unique_ptr<Slingshot> slingshot;
protected:
    void SetUp() override {
        slingshot = std::make_unique<Slingshot>();
    }
};

TEST_F(EnemyTest, Expect_IntEquality) {
    EXPECT_EQ(enemy->getHealth(), 50);
}

TEST_F(EnemyTest, Expect_IntNonEquality) {
    EXPECT_NE(enemy->getHealth(), 100);
}

TEST_F(SlingshotTest, Expect_FloatEquality) {
    float tension = static_cast<float>(slingshot->getTension());
    EXPECT_FLOAT_EQ(tension, 0.0f);
}

TEST_F(SlingshotTest, Expect_FloatNonEquality) {
    slingshot->pullBack(50);
    float tension = static_cast<float>(slingshot->getTension());
    EXPECT_NE(tension, 0.0f);
}

TEST_F(SlingshotTest, Expect_StringEquality) {
    EXPECT_EQ(slingshot->getBirdType(), "Red");
}

TEST_F(SlingshotTest, Expect_StringNonEquality) {
    EXPECT_NE(slingshot->getBirdType(), "Blue");
}

TEST_F(SlingshotTest, Expect_StringCaseEquality) {
    EXPECT_STRCASEEQ(slingshot->getBirdType().c_str(), "red");
}

TEST_F(SlingshotTest, Expect_StringCaseNonEquality) {
    EXPECT_STRCASENE(slingshot->getBirdType().c_str(), "blue");
}

TEST_F(SlingshotTest, Expect_StringStrictEquality) {
    EXPECT_STREQ(slingshot->getBirdType().c_str(), "Red");
}

TEST_F(SlingshotTest, Expect_StringStrictNonEquality) {
    EXPECT_STRNE(slingshot->getBirdType().c_str(), "red");
}

TEST_F(EnemyTest, Expect_GreaterThan) {
    EXPECT_GT(enemy->getHealth(), 0);
}

TEST_F(EnemyTest, Expect_LessThan) {
    EXPECT_LT(enemy->getHealth(), 100);
}

TEST_F(EnemyTest, Expect_GreaterThanOrEqual) {
    EXPECT_GE(enemy->getHealth(), 50);
}

TEST_F(EnemyTest, Expect_LessThanOrEqual) {
    EXPECT_LE(enemy->getHealth(), 50);
}

TEST_F(EnemyTest, Assert_IntEquality) {
    ASSERT_EQ(enemy->getHealth(), 50);
}

TEST_F(EnemyTest, Assert_IntNonEquality) {
    ASSERT_NE(enemy->getHealth(), 100);
}

TEST_F(SlingshotTest, Assert_FloatEquality) {
    float tension = static_cast<float>(slingshot->getTension());
    ASSERT_FLOAT_EQ(tension, 0.0f);
}

TEST_F(SlingshotTest, Assert_FloatNonEquality) {
    slingshot->pullBack(50);
    float tension = static_cast<float>(slingshot->getTension());
    ASSERT_NE(tension, 0.0f);
}

TEST_F(SlingshotTest, Assert_StringEquality) {
    ASSERT_EQ(slingshot->getBirdType(), "Red");
}

TEST_F(SlingshotTest, Assert_StringNonEquality) {
    ASSERT_NE(slingshot->getBirdType(), "Blue");
}

TEST_F(SlingshotTest, Assert_StringCaseEquality) {
    ASSERT_STRCASEEQ(slingshot->getBirdType().c_str(), "red");
}

TEST_F(SlingshotTest, Assert_StringCaseNonEquality) {
    ASSERT_STRCASENE(slingshot->getBirdType().c_str(), "blue");
}

TEST_F(SlingshotTest, Assert_StringStrictEquality) {
    ASSERT_STREQ(slingshot->getBirdType().c_str(), "Red");
}

TEST_F(SlingshotTest, Assert_StringStrictNonEquality) {
    ASSERT_STRNE(slingshot->getBirdType().c_str(), "red");
}

TEST_F(EnemyTest, Assert_GreaterThan) {
    ASSERT_GT(enemy->getHealth(), 0);
}

TEST_F(EnemyTest, Assert_LessThan) {
    ASSERT_LT(enemy->getHealth(), 100);
}

TEST_F(EnemyTest, Assert_GreaterThanOrEqual) {
    ASSERT_GE(enemy->getHealth(), 50);
}

TEST_F(EnemyTest, Assert_LessThanOrEqual) {
    ASSERT_LE(enemy->getHealth(), 50);
}

class EnemyPositionTest : public testing::TestWithParam<std::pair<float, float>> {};

TEST_P(EnemyPositionTest, CompareEnemyPositions) {
    auto [x, y] = GetParam();
    Enemy e(50, x, y);
    auto [posX, posY] = e.getPosition();
    EXPECT_EQ(posX, x);
    EXPECT_EQ(posY, y);
}

INSTANTIATE_TEST_SUITE_P(
    EnemyPositions,
    EnemyPositionTest,
    testing::Values(
        std::make_pair(0.0f, 0.0f),
        std::make_pair(10.0f, 20.0f),
        std::make_pair(100.0f, 200.0f),
        std::make_pair(-50.0f, 75.0f)
    )
);

int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}