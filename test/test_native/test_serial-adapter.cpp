#include <gmock/gmock.h>

#include "serial-adapter.h"

using ::testing::InSequence;
using ::testing::Return;

namespace {
class MockSerialAdapter : public SerialAdapter {
   public:
    MockSerialAdapter() : SerialAdapter(NULL) {}
    MOCK_METHOD(int, available, (), (override));
    MOCK_METHOD(int, read, (), (override));
};

// TODO: Add tests for some invalid sequences
TEST(IsLineReady, ShouldReturnFalseIfNothingAvailable) {
    MockSerialAdapter underTest;

    EXPECT_CALL(underTest, available()).Times(1);

    EXPECT_EQ(false, underTest.isLineReady());
}

TEST(IsLineReady, ShouldReturnFalseIfTextButNoNewlineAvailable) {
    MockSerialAdapter underTest;

    EXPECT_CALL(underTest, available()).Times(4).WillOnce(Return(3)).WillOnce(Return(2)).WillOnce(Return(1));

    EXPECT_CALL(underTest, read()).Times(3).WillRepeatedly(Return('a'));

    EXPECT_EQ(false, underTest.isLineReady());
}

TEST(IsLineReady, ShouldReturnTrueIfNewlineAvailableNow) {
    MockSerialAdapter underTest;

    EXPECT_CALL(underTest, available()).Times(1).WillOnce(Return(1));
    EXPECT_CALL(underTest, read()).Times(1).WillOnce(Return('\n'));

    EXPECT_EQ(true, underTest.isLineReady());
}

TEST(IsLineReady, ShouldReturnTrueIfNewlineAvailableSoon) {
    MockSerialAdapter underTest;

    EXPECT_CALL(underTest, available()).Times(3).WillRepeatedly(Return(1));
    EXPECT_CALL(underTest, read()).Times(3).WillOnce(Return('a')).WillOnce(Return('b')).WillOnce(Return('\n'));

    EXPECT_EQ(true, underTest.isLineReady());
}

TEST(ReadWord, ShouldReturnNullIfLineIsEmpty) {
    MockSerialAdapter underTest;

    EXPECT_CALL(underTest, available()).Times(1).WillRepeatedly(Return(1));
    EXPECT_CALL(underTest, read()).Times(1).WillOnce(Return('\n'));

    EXPECT_EQ(true, underTest.isLineReady());
    EXPECT_STREQ(nullptr, underTest.readWord());
}

TEST(ReadWord, ShouldReturnCharsIfLineHasSingleWord) {
    MockSerialAdapter underTest;

    EXPECT_CALL(underTest, available()).Times(5).WillRepeatedly(Return(1));
    EXPECT_CALL(underTest, read())
        .Times(5)
        .WillOnce(Return('a'))
        .WillOnce(Return('b'))
        .WillOnce(Return('-'))
        .WillOnce(Return('c'))
        .WillOnce(Return('\n'));

    EXPECT_EQ(true, underTest.isLineReady());
    EXPECT_STREQ("ab-c", underTest.readWord());
}

TEST(ReadWord, ShouldReturnNullIfNoFurtherWords) {
    MockSerialAdapter underTest;

    EXPECT_CALL(underTest, available()).Times(5).WillRepeatedly(Return(1));
    EXPECT_CALL(underTest, read())
        .Times(5)
        .WillOnce(Return('a'))
        .WillOnce(Return('b'))
        .WillOnce(Return('-'))
        .WillOnce(Return('c'))
        .WillOnce(Return('\n'));

    EXPECT_EQ(true, underTest.isLineReady());
    EXPECT_STREQ("ab-c", underTest.readWord());
    EXPECT_STREQ(nullptr, underTest.readWord());
    EXPECT_STREQ(nullptr, underTest.readWord());
}

TEST(ReadWord, ShouldReturnDifferentWordsWhenSpaceSeparated) {
    MockSerialAdapter underTest;

    EXPECT_CALL(underTest, available()).Times(8).WillRepeatedly(Return(1));
    EXPECT_CALL(underTest, read())
        .Times(8)
        .WillOnce(Return('a'))
        .WillOnce(Return('b'))
        .WillOnce(Return('c'))
        .WillOnce(Return(' '))
        .WillOnce(Return('d'))
        .WillOnce(Return('e'))
        .WillOnce(Return('f'))
        .WillOnce(Return('\n'));

    EXPECT_EQ(true, underTest.isLineReady());
    EXPECT_STREQ("abc", underTest.readWord());
    EXPECT_STREQ("def", underTest.readWord());
    EXPECT_STREQ(nullptr, underTest.readWord());
    EXPECT_STREQ(nullptr, underTest.readWord());
}

TEST(ReadWord, ShouldReturnNullWhenLastCharIsSpace) {
    MockSerialAdapter underTest;

    EXPECT_CALL(underTest, available()).Times(4).WillRepeatedly(Return(1));
    EXPECT_CALL(underTest, read())
        .Times(4)
        .WillOnce(Return('a'))
        .WillOnce(Return('b'))
        .WillOnce(Return(' '))
        .WillOnce(Return('\n'));

    EXPECT_EQ(true, underTest.isLineReady());
    EXPECT_STREQ("ab", underTest.readWord());
    EXPECT_STREQ(nullptr, underTest.readWord());
    EXPECT_STREQ(nullptr, underTest.readWord());
    EXPECT_STREQ(nullptr, underTest.readWord());
}

TEST(skipRestOfLine, ShouldResetBuffer) {
    MockSerialAdapter underTest;

    EXPECT_CALL(underTest, available()).Times(4).WillRepeatedly(Return(1));
    EXPECT_CALL(underTest, read())
        .Times(4)
        .WillOnce(Return('a'))
        .WillOnce(Return('b'))
        .WillOnce(Return('c'))
        .WillOnce(Return('\n'));

    EXPECT_EQ(true, underTest.isLineReady());
    EXPECT_STREQ("abc", underTest.readWord());
    EXPECT_STREQ(nullptr, underTest.readWord());

    EXPECT_EQ(false, underTest.skipRestOfLine());

    EXPECT_CALL(underTest, available()).Times(4).WillRepeatedly(Return(1));
    EXPECT_CALL(underTest, read())
        .Times(4)
        .WillOnce(Return('d'))
        .WillOnce(Return('e'))
        .WillOnce(Return('f'))
        .WillOnce(Return('\n'));

    EXPECT_EQ(true, underTest.isLineReady());
    EXPECT_STREQ("def", underTest.readWord());
    EXPECT_STREQ(nullptr, underTest.readWord());
}

TEST(skipRestOfLine, ShouldReturnTrueWhenCharsAreSkipped) {
    MockSerialAdapter underTest;

    EXPECT_CALL(underTest, available()).Times(6).WillRepeatedly(Return(1));
    EXPECT_CALL(underTest, read())
        .Times(6)
        .WillOnce(Return('a'))
        .WillOnce(Return('b'))
        .WillOnce(Return(' '))
        .WillOnce(Return('c'))
        .WillOnce(Return('d'))
        .WillOnce(Return('\n'));

    EXPECT_EQ(true, underTest.isLineReady());
    EXPECT_STREQ("ab", underTest.readWord());

    EXPECT_EQ(true, underTest.skipRestOfLine());
}

TEST(ReadBool, ShouldReturnNullWhenWordIsNull) {
    MockSerialAdapter underTest;

    EXPECT_CALL(underTest, available()).Times(1).WillRepeatedly(Return(1));
    EXPECT_CALL(underTest, read()).Times(1).WillOnce(Return('\n'));

    EXPECT_EQ(true, underTest.isLineReady());
    EXPECT_EQ(nullptr, underTest.readBool());
    EXPECT_EQ(nullptr, underTest.readBool());
}

TEST(ReadBool, ShouldReturnNullWhenWordIsNotABoolean) {
    MockSerialAdapter underTest;

    EXPECT_CALL(underTest, available()).Times(4).WillRepeatedly(Return(1));
    EXPECT_CALL(underTest, read())
        .Times(4)
        .WillOnce(Return('a'))
        .WillOnce(Return('b'))
        .WillOnce(Return('c'))
        .WillOnce(Return('\n'));

    EXPECT_EQ(true, underTest.isLineReady());
    EXPECT_EQ(nullptr, underTest.readBool());
    EXPECT_EQ(nullptr, underTest.readWord());
}

TEST(ReadBool, ShouldReturnFalseWhenWordIsFalse) {
    MockSerialAdapter underTest;

    EXPECT_CALL(underTest, available()).Times(6).WillRepeatedly(Return(1));
    EXPECT_CALL(underTest, read())
        .Times(6)
        .WillOnce(Return('F'))
        .WillOnce(Return('A'))
        .WillOnce(Return('l'))
        .WillOnce(Return('s'))
        .WillOnce(Return('e'))
        .WillOnce(Return('\n'));

    EXPECT_EQ(true, underTest.isLineReady());
    const bool* result = underTest.readBool();
    EXPECT_NE(nullptr, result);
    EXPECT_EQ(false, *result);
    EXPECT_EQ(nullptr, underTest.readBool());
}

TEST(ReadBool, ShouldReturnFalseWhenWordIsTrue) {
    MockSerialAdapter underTest;

    EXPECT_CALL(underTest, available()).Times(5).WillRepeatedly(Return(1));
    EXPECT_CALL(underTest, read())
        .Times(5)
        .WillOnce(Return('T'))
        .WillOnce(Return('R'))
        .WillOnce(Return('u'))
        .WillOnce(Return('e'))
        .WillOnce(Return('\n'));

    EXPECT_EQ(true, underTest.isLineReady());
    const bool* result = underTest.readBool();
    EXPECT_NE(nullptr, result);
    EXPECT_EQ(true, *result);
    EXPECT_EQ(nullptr, underTest.readBool());
}

TEST(ReadFloat, ShouldReturnNullWhenWordIsNull) {
    MockSerialAdapter underTest;

    EXPECT_CALL(underTest, available()).Times(1).WillRepeatedly(Return(1));
    EXPECT_CALL(underTest, read()).Times(1).WillOnce(Return('\n'));

    EXPECT_EQ(true, underTest.isLineReady());
    EXPECT_EQ(nullptr, underTest.readFloat());
    EXPECT_EQ(nullptr, underTest.readFloat());
}

TEST(ReadFloat, ShouldReturnNullWhenWordIsNotAFloat) {
    MockSerialAdapter underTest;

    EXPECT_CALL(underTest, available()).Times(4).WillRepeatedly(Return(1));
    EXPECT_CALL(underTest, read())
        .Times(4)
        .WillOnce(Return('a'))
        .WillOnce(Return('b'))
        .WillOnce(Return('c'))
        .WillOnce(Return('\n'));

    EXPECT_EQ(true, underTest.isLineReady());
    EXPECT_EQ(nullptr, underTest.readFloat());
    EXPECT_EQ(nullptr, underTest.readFloat());
}

TEST(ReadFloat, ShouldReturnValueWhenWordIsInteger) {
    MockSerialAdapter underTest;

    EXPECT_CALL(underTest, available()).Times(4).WillRepeatedly(Return(1));
    EXPECT_CALL(underTest, read())
        .Times(4)
        .WillOnce(Return('1'))
        .WillOnce(Return('2'))
        .WillOnce(Return('3'))
        .WillOnce(Return('\n'));

    EXPECT_EQ(true, underTest.isLineReady());
    const float* result = underTest.readFloat();
    EXPECT_NE(nullptr, result);
    EXPECT_EQ((float)123.0, *result);
    EXPECT_EQ(nullptr, underTest.readFloat());
}

TEST(ReadFloat, ShouldReturnValueWhenWordIsFloat) {
    MockSerialAdapter underTest;

    EXPECT_CALL(underTest, available()).Times(6).WillRepeatedly(Return(1));
    EXPECT_CALL(underTest, read())
        .Times(6)
        .WillOnce(Return('1'))
        .WillOnce(Return('2'))
        .WillOnce(Return('.'))
        .WillOnce(Return('3'))
        .WillOnce(Return('4'))
        .WillOnce(Return('\n'));

    EXPECT_EQ(true, underTest.isLineReady());
    const float* result = underTest.readFloat();
    EXPECT_NE(nullptr, result);
    EXPECT_EQ((float)12.34, *result);
    EXPECT_EQ(nullptr, underTest.readFloat());
}

TEST(ReadUnsignedLong, ShouldReturnNullWhenWordIsNull) {
    MockSerialAdapter underTest;

    EXPECT_CALL(underTest, available()).Times(1).WillRepeatedly(Return(1));
    EXPECT_CALL(underTest, read()).Times(1).WillOnce(Return('\n'));

    EXPECT_EQ(true, underTest.isLineReady());
    EXPECT_EQ(nullptr, underTest.readUnsignedLong());
    EXPECT_EQ(nullptr, underTest.readUnsignedLong());
}

TEST(ReadUnsignedLong, ShouldReturnNullWhenWordIsNotNumeric) {
    MockSerialAdapter underTest;

    EXPECT_CALL(underTest, available()).Times(4).WillRepeatedly(Return(1));
    EXPECT_CALL(underTest, read())
        .Times(4)
        .WillOnce(Return('a'))
        .WillOnce(Return('b'))
        .WillOnce(Return('c'))
        .WillOnce(Return('\n'));

    EXPECT_EQ(true, underTest.isLineReady());
    EXPECT_EQ(nullptr, underTest.readUnsignedLong());
    EXPECT_EQ(nullptr, underTest.readUnsignedLong());
}

TEST(ReadUnsignedLong, ShouldReturnNullWhenWordIsFloat) {
    MockSerialAdapter underTest;

    EXPECT_CALL(underTest, available()).Times(4).WillRepeatedly(Return(1));
    EXPECT_CALL(underTest, read())
        .Times(4)
        .WillOnce(Return('1'))
        .WillOnce(Return('.'))
        .WillOnce(Return('2'))
        .WillOnce(Return('\n'));

    EXPECT_EQ(true, underTest.isLineReady());
    EXPECT_EQ(nullptr, underTest.readUnsignedLong());
    EXPECT_EQ(nullptr, underTest.readUnsignedLong());
}

TEST(ReadUnsignedLong, ShouldReturnValueWhenWordIsInteger) {
    MockSerialAdapter underTest;

    EXPECT_CALL(underTest, available()).Times(4).WillRepeatedly(Return(1));
    EXPECT_CALL(underTest, read())
        .Times(4)
        .WillOnce(Return('1'))
        .WillOnce(Return('2'))
        .WillOnce(Return('3'))
        .WillOnce(Return('\n'));

    EXPECT_EQ(true, underTest.isLineReady());
    const unsigned long* result = underTest.readUnsignedLong();
    EXPECT_NE(nullptr, result);
    EXPECT_EQ((unsigned long)123, *result);
    EXPECT_EQ(nullptr, underTest.readUnsignedLong());
}
// TODO: Continue here with checks for readWord, readBool, readFloat, readUnsignedLong
}  // namespace