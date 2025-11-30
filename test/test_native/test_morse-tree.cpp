#include <gtest/gtest.h>

#include "morse-tree.h"

char runTest(const char* input) {
    MorseTree underTest;
    return underTest.getLetterForMorseSymbols(input);
}

TEST(MorseTree, ShouldReturnErrorIfInvalidSymbolProvided) {
    EXPECT_EQ(MorseTree::Error, runTest("a"));
    EXPECT_EQ(MorseTree::Error, runTest(" "));
    EXPECT_EQ(MorseTree::Error, runTest("!"));
}

TEST(MorseTree, ShouldReturnNoneIfEmptyStringProvided) { EXPECT_EQ(MorseTree::Incomplete, runTest("")); }
TEST(MorseTree, ShouldReturnNoneIfIncompleteSequenceProvided) { EXPECT_EQ(MorseTree::Incomplete, runTest("-.-.-")); }

TEST(MorseTree, ShouldDecodeToCorrectLetter) {
    EXPECT_EQ('a', runTest(".-"));
    EXPECT_EQ('b', runTest("-..."));
    EXPECT_EQ('c', runTest("-.-."));
    EXPECT_EQ('d', runTest("-.."));
    EXPECT_EQ('e', runTest("."));
    EXPECT_EQ('f', runTest("..-."));
    EXPECT_EQ('g', runTest("--."));
    EXPECT_EQ('h', runTest("...."));
    EXPECT_EQ('i', runTest(".."));
    EXPECT_EQ('j', runTest(".---"));
    EXPECT_EQ('k', runTest("-.-"));
    EXPECT_EQ('l', runTest(".-.."));
    EXPECT_EQ('m', runTest("--"));
    EXPECT_EQ('n', runTest("-."));
    EXPECT_EQ('o', runTest("---"));
    EXPECT_EQ('p', runTest(".--."));
    EXPECT_EQ('q', runTest("--.-"));
    EXPECT_EQ('r', runTest(".-."));
    EXPECT_EQ('s', runTest("..."));
    EXPECT_EQ('t', runTest("-"));
    EXPECT_EQ('u', runTest("..-"));
    EXPECT_EQ('v', runTest("...-"));
    EXPECT_EQ('w', runTest(".--"));
    EXPECT_EQ('x', runTest("-..-"));
    EXPECT_EQ('y', runTest("-.--"));
    EXPECT_EQ('z', runTest("--.."));
    EXPECT_EQ('1', runTest(".----"));
    EXPECT_EQ('2', runTest("..---"));
    EXPECT_EQ('3', runTest("...--"));
    EXPECT_EQ('4', runTest("....-"));
    EXPECT_EQ('5', runTest("....."));
    EXPECT_EQ('6', runTest("-...."));
    EXPECT_EQ('7', runTest("--..."));
    EXPECT_EQ('8', runTest("---.."));
    EXPECT_EQ('9', runTest("----."));
    EXPECT_EQ('0', runTest("-----"));
    EXPECT_EQ('.', runTest(".-.-.-"));
    EXPECT_EQ('?', runTest("..--.."));
    EXPECT_EQ('\'', runTest(".----."));
    EXPECT_EQ('!', runTest("-.-.--"));
}
