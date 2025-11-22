#include <gtest/gtest.h>

#include "morse-tree.h"

TEST(DummyTest, ShouldPass) { EXPECT_EQ(1, 1); }

TEST(MorseTree, ShouldDecodeToCorrectLetter) {
    EXPECT_EQ('a', getLetterForMorseSymbols(".-"));
    EXPECT_EQ('b', getLetterForMorseSymbols("-..."));
    EXPECT_EQ('c', getLetterForMorseSymbols("-.-."));
    EXPECT_EQ('d', getLetterForMorseSymbols("-.."));
    EXPECT_EQ('e', getLetterForMorseSymbols("."));
    EXPECT_EQ('f', getLetterForMorseSymbols("..-."));
    EXPECT_EQ('g', getLetterForMorseSymbols("--."));
    EXPECT_EQ('h', getLetterForMorseSymbols("...."));
    EXPECT_EQ('i', getLetterForMorseSymbols(".."));
    EXPECT_EQ('j', getLetterForMorseSymbols(".---"));
    EXPECT_EQ('k', getLetterForMorseSymbols("-.-"));
    EXPECT_EQ('l', getLetterForMorseSymbols(".-.."));
    EXPECT_EQ('m', getLetterForMorseSymbols("--"));
    EXPECT_EQ('n', getLetterForMorseSymbols("-."));
    EXPECT_EQ('o', getLetterForMorseSymbols("---"));
    EXPECT_EQ('p', getLetterForMorseSymbols(".--."));
    EXPECT_EQ('q', getLetterForMorseSymbols("--.-"));
    EXPECT_EQ('r', getLetterForMorseSymbols(".-."));
    EXPECT_EQ('s', getLetterForMorseSymbols("..."));
    EXPECT_EQ('t', getLetterForMorseSymbols("-"));
    EXPECT_EQ('u', getLetterForMorseSymbols("..-"));
    EXPECT_EQ('v', getLetterForMorseSymbols("...-"));
    EXPECT_EQ('w', getLetterForMorseSymbols(".--"));
    EXPECT_EQ('x', getLetterForMorseSymbols("-..-"));
    EXPECT_EQ('y', getLetterForMorseSymbols("-.--"));
    EXPECT_EQ('z', getLetterForMorseSymbols("--.."));
    EXPECT_EQ('1', getLetterForMorseSymbols(".----"));
    EXPECT_EQ('2', getLetterForMorseSymbols("..---"));
    EXPECT_EQ('3', getLetterForMorseSymbols("...--"));
    EXPECT_EQ('4', getLetterForMorseSymbols("....-"));
    EXPECT_EQ('5', getLetterForMorseSymbols("....."));
    EXPECT_EQ('6', getLetterForMorseSymbols("-...."));
    EXPECT_EQ('7', getLetterForMorseSymbols("--..."));
    EXPECT_EQ('8', getLetterForMorseSymbols("---.."));
    EXPECT_EQ('9', getLetterForMorseSymbols("----."));
    EXPECT_EQ('0', getLetterForMorseSymbols("-----"));
    EXPECT_EQ('.', getLetterForMorseSymbols(".-.-.-"));
    EXPECT_EQ('?', getLetterForMorseSymbols("..--.."));
    EXPECT_EQ('\'', getLetterForMorseSymbols(".----."));
    EXPECT_EQ('!', getLetterForMorseSymbols("-.-.--"));
}

void Sub1(int n) { EXPECT_EQ(n, 1); }

#if defined(ARDUINO)
#include <Arduino.h>

void setup() {
    // should be the same value as for the `test_speed` option in "platformio.ini"
    // default value is test_speed=115200
    Serial.begin(115200);

    ::testing::InitGoogleTest();
    // if you plan to use GMock, replace the line above with
    // ::testing::InitGoogleMock();
}

void loop() {
    // Run tests
    if (RUN_ALL_TESTS())
        ;

    // sleep for 1 sec
    delay(1000);
}

#else
int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    // if you plan to use GMock, replace the line above with
    // ::testing::InitGoogleMock(&argc, argv);

    if (RUN_ALL_TESTS())
        ;

    // Always return zero-code and allow PlatformIO to parse results
    return 0;
}
#endif
// #include "unity.h"

// void test_tree_is_created() {
//     Node* tree = createMorseTree();

//     TEST_ASSERT_NOT_NULL(tree);
//     TEST_ASSERT_EQUAL_CHAR(0, tree->letter);

//     TEST_ASSERT_NOT_NULL(tree->dot);
// }

// void setUp(void) {
//     // set stuff up here
// }

// void tearDown(void) {
//     // clean stuff up here
// }

// int main() {
//     UNITY_BEGIN();

//     RUN_TEST(test_tree_is_created);

//     return UNITY_END();
// }