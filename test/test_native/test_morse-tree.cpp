#include <gtest/gtest.h>

#include "morse-tree.h"

// TODO: Add tests for some invalid sequences
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
