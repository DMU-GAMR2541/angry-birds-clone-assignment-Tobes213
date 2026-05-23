#include <gtest/gtest.h>
#include "Enemy.h"
#include "slingshot.h"
#include <SFML/Graphics.hpp>
#include <filesystem>
#include <list>

TEST(TextureTest, PrintWorkingDir) {
    std::cout << "CWD: " << std::filesystem::current_path() << std::endl;
}

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

TEST_F(EnemyTest, DynamicObject_MovementTest) {
    enemy->takeDamage(10);
    EXPECT_LT(enemy->getHealth(), 50);
    EXPECT_GE(enemy->getHealth(), 0);
}

TEST(StaticObjectTest, Placement_WithinWindowBounds) {
    float windowWidth = 800.0f;
    float windowHeight = 600.0f;
    float posX = 450.0f;
    float posY = 510.0f;

    EXPECT_GT(posX, 0.0f);
    EXPECT_LT(posX, windowWidth);
    EXPECT_GT(posY, 0.0f);
    EXPECT_LT(posY, windowHeight);
}

TEST(GameObjectPositionTest, RelativePositions) {
    Enemy pig1(50, 200.0f, 300.0f);
    Enemy pig2(100, 400.0f, 200.0f);
    Enemy pig3(150, 600.0f, 100.0f);

    EXPECT_GT(pig2.getPosition().first, pig1.getPosition().first);
    EXPECT_GT(pig3.getPosition().first, pig2.getPosition().first);
    EXPECT_LT(pig3.getPosition().second, pig1.getPosition().second);
}

TEST(TextureTest, Texture_LoadsSuccessfully) {
    sf::Texture texture;
    bool loaded = texture.loadFromFile("../assets/Ang_Birds/Angry_Birds.png");
    EXPECT_TRUE(loaded);
}

TEST(TextureTest, Texture_FailsWithBadPath) {
    sf::Texture texture;
    bool loaded = texture.loadFromFile("../assets/nonexistent.png");
    EXPECT_FALSE(loaded);
}

class DestructorTest : public testing::Test {
protected:
    std::vector<std::string> destructorLog;
};

class TestGameObject {
public:
    std::vector<std::string>& log;
    TestGameObject(std::vector<std::string>& l) : log(l) {}
    virtual ~TestGameObject() { log.push_back("GameObject"); }
};

class TestDynamicObject : public TestGameObject {
public:
    TestDynamicObject(std::vector<std::string>& l) : TestGameObject(l) {}
    virtual ~TestDynamicObject() { log.push_back("DynamicObject"); }
};

class TestPig : public TestDynamicObject {
public:
    TestPig(std::vector<std::string>& l) : TestDynamicObject(l) {}
    virtual ~TestPig() { log.push_back("Pig"); }
};

TEST_F(DestructorTest, DestructorSequence_PigToGameObject) {
    {
        TestPig pig(destructorLog);
    }
    ASSERT_EQ(destructorLog.size(), 3);
    EXPECT_EQ(destructorLog[0], "Pig");
    EXPECT_EQ(destructorLog[1], "DynamicObject");
    EXPECT_EQ(destructorLog[2], "GameObject");
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

class CatapultTest : public testing::Test {
public:
    std::unique_ptr<Slingshot> slingshot;
protected:
    void SetUp() override {
        slingshot = std::make_unique<Slingshot>();
    }

    void TearDown() override {}
};

TEST_F(CatapultTest, Expect_TensionZeroAtStart) {
    EXPECT_EQ(slingshot->getTension(), 0);
}

TEST_F(CatapultTest, Expect_PullBackIncreasesTension) {
    slingshot->pullBack(30);
    EXPECT_GT(slingshot->getTension(), 0);
}

TEST_F(CatapultTest, Expect_LargerPullGivesGreaterTension) {
    slingshot->pullBack(20);
    int tensionSmall = slingshot->getTension();
    slingshot->release();
    slingshot->pullBack(80);
    int tensionLarge = slingshot->getTension();
    EXPECT_GT(tensionLarge, tensionSmall);
}

TEST_F(CatapultTest, Assert_TensionResetsAfterRelease) {
    slingshot->pullBack(50);
    slingshot->release();
    ASSERT_EQ(slingshot->getTension(), 0);
}

TEST_F(CatapultTest, Expect_NegativePullReturnsFalse) {
    EXPECT_FALSE(slingshot->pullBack(-10));
}

TEST_F(CatapultTest, Expect_ValidPullReturnsTrue) {
    EXPECT_TRUE(slingshot->pullBack(50));
}

TEST_F(CatapultTest, Expect_TensionDoesNotExceedMaxLimit) {
    slingshot->pullBack(9999);
    EXPECT_LE(slingshot->getTension(), 100);
}

TEST_F(CatapultTest, Assert_MaxTensionCappedAt100) {
    slingshot->pullBack(9999);
    ASSERT_EQ(slingshot->getTension(), 100);
}

TEST_F(CatapultTest, Expect_PullBeyondLimitClamped) {
    slingshot->pullBack(100);
    int tensionAtMax = slingshot->getTension();
    slingshot->release();
    slingshot->pullBack(600);
    int tensionBeyond = slingshot->getTension();
    EXPECT_EQ(tensionAtMax, tensionBeyond);
}

TEST_F(CatapultTest, Expect_TensionLessThanOrEqualToMax) {
    slingshot->pullBack(60);
    EXPECT_LE(slingshot->getTension(), 100);
}


class BirdLoadTest : public testing::Test {
public:
    std::unique_ptr<Slingshot> slingshot;
protected:
    void SetUp() override {
        slingshot = std::make_unique<Slingshot>();
    }

    void TearDown() override {}
};

TEST_F(BirdLoadTest, Expect_DefaultBirdIsRed) {
    EXPECT_EQ(slingshot->getBirdType(), "Red");
}

TEST_F(BirdLoadTest, Expect_BirdTypeNotEmpty) {
    EXPECT_NE(slingshot->getBirdType(), "");
}

TEST_F(BirdLoadTest, Assert_LoadBirdChangesType) {
    slingshot->loadBird("Chuck");
    ASSERT_EQ(slingshot->getBirdType(), "Chuck");
}

TEST_F(BirdLoadTest, Expect_LoadBirdNotEqualToPrevious) {
    std::string firstBird = slingshot->getBirdType();
    slingshot->loadBird("Bomb");
    EXPECT_NE(slingshot->getBirdType(), firstBird);
}

TEST_F(BirdLoadTest, Expect_BirdTypeCaseInsensitiveMatch) {
    EXPECT_STRCASEEQ(slingshot->getBirdType().c_str(), "red");
}

TEST_F(BirdLoadTest, Expect_BirdTypeStrictCasingCorrect) {
    EXPECT_STREQ(slingshot->getBirdType().c_str(), "Red");
}

TEST_F(BirdLoadTest, Expect_BirdTypeStrictNonEqualLowercase) {
    EXPECT_STRNE(slingshot->getBirdType().c_str(), "red");
}

TEST_F(BirdLoadTest, Assert_SetBirdTypeUpdatesCorrectly) {
    slingshot->setBirdType("Matilda");
    ASSERT_EQ(slingshot->getBirdType(), "Matilda");
}

TEST_F(BirdLoadTest, Expect_BirdTypeNotEqualAfterChange) {
    slingshot->setBirdType("Chuck");
    EXPECT_NE(slingshot->getBirdType(), "Red");
}


class BirdLoadParamTest : public testing::TestWithParam<std::string> {};

TEST_P(BirdLoadParamTest, AllBirdTypesLoadCorrectly) {
    Slingshot s;
    s.loadBird(GetParam());
    EXPECT_EQ(s.getBirdType(), GetParam());
    EXPECT_NE(s.getBirdType(), "");
    EXPECT_STRNE(s.getBirdType().c_str(), "Ball");
    EXPECT_STRNE(s.getBirdType().c_str(), "Circle");
}

INSTANTIATE_TEST_SUITE_P(
    BirdTypes,
    BirdLoadParamTest,
    testing::Values("Red", "Chuck", "Bomb", "Matilda")
);


class PigCountTest : public testing::Test {
public:
    int pigCount;
protected:
    void SetUp() override {
        pigCount = 3;
    }

    void TearDown() override {}
};

TEST_F(PigCountTest, Expect_InitialPigCountIsThree) {
    EXPECT_EQ(pigCount, 3);
}

TEST_F(PigCountTest, Expect_KKeyDecrementsPigCount) {
    pigCount--;
    EXPECT_EQ(pigCount, 2);
}

TEST_F(PigCountTest, Assert_PigCountNeverBelowZero) {
    pigCount = 0;
    if (pigCount > 0) pigCount--;
    ASSERT_GE(pigCount, 0);
}

TEST_F(PigCountTest, Expect_PigCountLessThanInitialAfterDecrement) {
    int initial = pigCount;
    pigCount--;
    EXPECT_LT(pigCount, initial);
}

TEST_F(PigCountTest, Expect_PigCountGreaterThanZeroWhenPigsRemain) {
    EXPECT_GT(pigCount, 0);
}

TEST_F(PigCountTest, Assert_AllPigsPoppedCountIsZero) {
    pigCount = 0;
    ASSERT_EQ(pigCount, 0);
}

class PigCountParamTest : public testing::TestWithParam<int> {};

TEST_P(PigCountParamTest, DecrementReducesPigCount) {
    int count = GetParam();
    int original = count;
    if (count > 0) count--;
    EXPECT_LT(count, original);
    EXPECT_GE(count, 0);
}

INSTANTIATE_TEST_SUITE_P(
    PigCounts,
    PigCountParamTest,
    testing::Values(1, 2, 3, 5, 10)
);

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

class EnemyMovementTest : public testing::TestWithParam<int> {};

TEST_P(EnemyMovementTest, DamageReducesHealthCorrectly) {
    Enemy e(100);
    int damage = GetParam();
    e.takeDamage(damage);
    EXPECT_LT(e.getHealth(), 100);
    EXPECT_GE(e.getHealth(), 0);
    EXPECT_EQ(e.getHealth(), 100 - damage);
}

INSTANTIATE_TEST_SUITE_P(
    DamageValues,
    EnemyMovementTest,
    testing::Values(10, 20, 30, 40, 50)
);

class BlockHealthTest : public testing::Test {
public:
    int i_health;
    bool b_isDestroyed;
protected:
    void SetUp() override {
        i_health = 1;
        b_isDestroyed = false;
    }
    void TearDown() override {}
};

TEST_F(BlockHealthTest, Expect_WoodenBlockDiesInOneHit) {
    i_health--;
    if (i_health <= 0) b_isDestroyed = true;
    EXPECT_TRUE(b_isDestroyed);
}

TEST_F(BlockHealthTest, Expect_BlockHealthZeroAfterHit) {
    i_health--;
    EXPECT_LE(i_health, 0);
}

TEST_F(BlockHealthTest, Assert_BlockNotDestroyedBeforeHit) {
    ASSERT_FALSE(b_isDestroyed);
}

class StoneBlockTest : public testing::Test {
public:
    int i_health;
    bool b_isDestroyed;
protected:
    void SetUp() override {
        i_health = 2;
        b_isDestroyed = false;
    }
    void TearDown() override {}
};

TEST_F(StoneBlockTest, Expect_StoneBlockSurvivesFirstHit) {
    i_health--;
    if (i_health <= 0) b_isDestroyed = true;
    EXPECT_FALSE(b_isDestroyed);
    EXPECT_GT(i_health, 0);
}

TEST_F(StoneBlockTest, Expect_StoneBlockDiesOnSecondHit) {
    i_health--;
    i_health--;
    if (i_health <= 0) b_isDestroyed = true;
    EXPECT_TRUE(b_isDestroyed);
}

TEST_F(StoneBlockTest, Assert_StoneHealthStartsAtTwo) {
    ASSERT_EQ(i_health, 2);
}

class PigHealthTest : public testing::Test {
public:
    int i_health;
    bool b_isDestroyed;
protected:
    void SetUp() override {
        i_health = 100;
        b_isDestroyed = false;
    }
    void TearDown() override {}
};

TEST_F(PigHealthTest, Expect_PigSurvivesPartialDamage) {
    i_health -= 50;
    if (i_health <= 0) b_isDestroyed = true;
    EXPECT_FALSE(b_isDestroyed);
    EXPECT_GT(i_health, 0);
}

TEST_F(PigHealthTest, Expect_PigDiesOnLethalDamage) {
    i_health -= 100;
    if (i_health <= 0) b_isDestroyed = true;
    EXPECT_TRUE(b_isDestroyed);
}

TEST_F(PigHealthTest, Assert_PigHealthStartsAtHundred) {
    ASSERT_EQ(i_health, 100);
}

TEST_F(PigHealthTest, Expect_PigHealthNeverBelowZero) {
    i_health -= 200;
    if (i_health < 0) i_health = 0;
    EXPECT_GE(i_health, 0);
}

class WinLoseTest : public testing::Test {
public:
    int pigCount;
    int birdCount;
protected:
    void SetUp() override {
        pigCount = 3;
        birdCount = 4;
    }
    void TearDown() override {}
};

TEST_F(WinLoseTest, Expect_WinWhenAllPigsDestroyed) {
    pigCount = 0;
    EXPECT_EQ(pigCount, 0);
}

TEST_F(WinLoseTest, Expect_LoseWhenNoBirdsLeft) {
    birdCount = 0;
    EXPECT_EQ(birdCount, 0);
    EXPECT_GT(pigCount, 0);
}

TEST_F(WinLoseTest, Expect_GameOngoingWhenBothRemain) {
    EXPECT_GT(pigCount, 0);
    EXPECT_GT(birdCount, 0);
}

TEST_F(WinLoseTest, Assert_InitialPigCountIsThree) {
    ASSERT_EQ(pigCount, 3);
}

TEST_F(WinLoseTest, Assert_InitialBirdCountIsFour) {
    ASSERT_EQ(birdCount, 4);
}

class BirdPowerupTest : public testing::Test {
public:
    bool b_activated;
    float f_speed;
    std::string str_birdType;
protected:
    void SetUp() override {
        b_activated = false;
        f_speed = 10.0f;
        str_birdType = "Chuck";
    }
    void TearDown() override {}
};

TEST_F(BirdPowerupTest, Expect_PowerupNotActivatedByDefault) {
    EXPECT_FALSE(b_activated);
}

TEST_F(BirdPowerupTest, Expect_ChuckSpeedBoostWorks) {
    if (str_birdType == "Chuck" && !b_activated) {
        b_activated = true;
        f_speed *= 2.5f;
    }
    EXPECT_GT(f_speed, 10.0f);
    EXPECT_TRUE(b_activated);
}

TEST_F(BirdPowerupTest, Expect_PowerupOnlyActivatesOnce) {
    b_activated = true;
    float originalSpeed = f_speed;
    if (!b_activated) f_speed *= 2.5f;
    EXPECT_EQ(f_speed, originalSpeed);
}

TEST_F(BirdPowerupTest, Assert_BirdTypeIsChuck) {
    ASSERT_EQ(str_birdType, "Chuck");
}

class BirdQueueTest : public testing::Test {
public:
    std::list<std::string> birdQueue;
protected:
    void SetUp() override {
        birdQueue.push_back("Red");
        birdQueue.push_back("Chuck");
        birdQueue.push_back("Bomb");
        birdQueue.push_back("Matilda");
    }
    void TearDown() override {}
};

TEST_F(BirdQueueTest, Assert_FirstBirdIsRed) {
    ASSERT_EQ(birdQueue.front(), "Red");
}

TEST_F(BirdQueueTest, Expect_QueueHasFourBirds) {
    EXPECT_EQ(birdQueue.size(), 4);
}

TEST_F(BirdQueueTest, Expect_SecondBirdIsChuckAfterPop) {
    birdQueue.pop_front();
    EXPECT_EQ(birdQueue.front(), "Chuck");
}

TEST_F(BirdQueueTest, Expect_QueueSizeReducesAfterPop) {
    size_t original = birdQueue.size();
    birdQueue.pop_front();
    EXPECT_LT(birdQueue.size(), original);
}

TEST_F(BirdQueueTest, Assert_LastBirdIsMatilda) {
    ASSERT_EQ(birdQueue.back(), "Matilda");
}

TEST_F(BirdQueueTest, Expect_QueueEmptyAfterAllPopped) {
    birdQueue.clear();
    EXPECT_TRUE(birdQueue.empty());
}

int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}