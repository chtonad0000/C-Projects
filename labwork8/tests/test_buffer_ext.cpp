#include "CCirtucalBufferExt.h"
#include <gtest/gtest.h>

TEST(CircularBufferExtConstructorTests, ConstructorFullCapacity) {
    CCirtucalBufferExt<int> buff1(5, 1);
    CCirtucalBufferExt<std::string> buff2(5, "hey");
    CCirtucalBufferExt<bool> buff3(5, false);
    std::string result1;
    std::string result2;
    for (int i = 0; i < 5; ++i) {
        result1 += std::to_string(buff1[i]);
        result2 += buff2[i];
    }
    ASSERT_EQ(result1, "11111");
    ASSERT_EQ(result2, "heyheyheyheyhey");
    ASSERT_FALSE(buff3[0]);
}

TEST(CircularBufferExtConstructorTests, InitializerListTest) {
    CCirtucalBufferExt<int> buff1 = {1, 2, 3, 4, 5, 6};
    CCirtucalBufferExt<std::string> buff2 = {"hello", "world"};
    std::string result1;
    std::string result2;
    for (int i = 0; i < buff1.size(); ++i) {
        result1 += std::to_string(buff1[i]);
    }
    result2 = buff2[0] + ' ' + buff2[1];
    ASSERT_EQ(result1, "123456");
    ASSERT_EQ(result2, "hello world");
}

TEST(CircularBufferExtOperatorTests, BracketsTest) {
    CCirtucalBufferExt<int> buff = {1, 2, 3, 4, 5, 6, 7, 8};
    ASSERT_EQ(1, buff[0]);
    ASSERT_EQ(4, buff[3]);
    ASSERT_EQ(8, buff[7]);
    ASSERT_EQ(1, buff[8]);
}

TEST(CircularBufferExtOperatorTests, AssignmentTest) {
    CCirtucalBufferExt<std::string> buff = {"hello", "," "world"};
    CCirtucalBufferExt<std::string> buff_copy;
    buff_copy = buff;
    ASSERT_EQ(buff[0], buff_copy[0]);
    ASSERT_EQ(buff[1], buff_copy[1]);
    ASSERT_EQ(buff[2], buff_copy[2]);
}

TEST(CircularBufferExtOperatorTests, EqualityTest) {
    CCirtucalBufferExt<std::string> buff1 = {"hello", "," "world"};
    CCirtucalBufferExt<std::string> buff2 = {"hello", "," "world"};
    CCirtucalBufferExt<int> buff3 = {1, 2, 3};
    CCirtucalBufferExt<int> buff4 = {1, 2, 4};
    ASSERT_TRUE(buff1 == buff2);
    ASSERT_TRUE(buff4 != buff3);
}

TEST(CircularBufferExtMethods, BeginIteratorTest) {
    CCirtucalBufferExt<int> buff(10, 1);
    int i = *(buff.begin());
    ASSERT_EQ(1, i);
}

TEST(CircularBufferExtMethods, EndIteratorTest) {
    CCirtucalBufferExt<int> buff = {1, 2, 3};
    int i = *(buff.end() - 1);
    ASSERT_EQ(3, i);
}

TEST(CircularBufferExtMethods, SwapTest) {
    CCirtucalBufferExt<int> buff1 = {1, 0, 3};
    CCirtucalBufferExt<int> buff2 = {1, 2, 3, 4};
    buff1.swap(buff2);
    ASSERT_EQ(4, buff1[3]);
    ASSERT_EQ(1, buff2[0]);
    ASSERT_EQ(0, buff2[1]);
    ASSERT_EQ(3, buff2[2]);
}

TEST(CircularBufferExtMethods, SizeTest) {
    CCirtucalBufferExt<int> buff1(3, 5);
    CCirtucalBufferExt<int> buff2 = {1, 2, 3, 4};
    CCirtucalBufferExt<int> buff3;
    ASSERT_EQ(3, buff1.size());
    ASSERT_EQ(4, buff2.size());
    ASSERT_EQ(0, buff3.size());
}

TEST(CircularBufferExtMethods, EmptyTest) {
    CCirtucalBufferExt<int> buff1;
    CCirtucalBufferExt<int> buff2 = {1, 2, 3, 4};
    CCirtucalBufferExt<int> buff3(10);
    ASSERT_TRUE(buff1.empty());
    ASSERT_FALSE(buff2.empty());
    ASSERT_TRUE(buff3.empty());
}

TEST(CircularBufferExtMethods, ClearTest) {
    CCirtucalBufferExt<int> buff = {1, 2, 3, 4};
    buff.clear();
    ASSERT_TRUE(buff.empty());
}

TEST(CircularBufferExtMethods, AssignTest) {
    CCirtucalBufferExt<std::string> buff1 = {"a", "b", "c"};
    CCirtucalBufferExt<std::string> buff2 = {"hello, ", "world"};
    buff1.assign(buff2.begin(), buff2.end());
    ASSERT_EQ("hello, ", buff1[0]);
    ASSERT_EQ("world", buff1[1]);
    buff1.assign({"proga"});
    ASSERT_EQ("proga", buff1[0]);
    ASSERT_EQ(1, buff1.size());
    buff2.assign(5, "wow");
    ASSERT_EQ("wow", buff2[0]);
    ASSERT_EQ(5, buff2.size());
}

TEST(CircularBufferExtMethods, InsertTest1) {
    CCirtucalBufferExt<int> res1 = {1};
    CCirtucalBufferExt<int> res2 = {2, 1};
    CCirtucalBufferExt<int> res3 = {3, 2, 1};
    CCirtucalBufferExt<int> res4 = {3, 2, 1, 5};
    CCirtucalBufferExt<int> res5 = {3, 2, 1, 5, 6};
    CCirtucalBufferExt<int> res6 = {3, 2, 7, 1, 5, 6};
    CCirtucalBufferExt<int> res7 = {2, 7, 1, 5, 8, 6};
    CCirtucalBufferExt<int> res8 = {7, 1, 5, 10, 8, 6};
    CCirtucalBufferExt<int> buffer;
    buffer.insert(buffer.cbegin(), 1);
    ASSERT_TRUE(buffer == res1);
    buffer.insert(buffer.cbegin(), 2);
    ASSERT_TRUE(buffer == res2);
    buffer.insert(buffer.cbegin(), 3);
    ASSERT_TRUE(buffer == res3);
    buffer.insert(buffer.cend(), 5);
    ASSERT_TRUE(buffer == res4);
    buffer.insert(buffer.cend(), 6);
    ASSERT_TRUE(buffer == res5);
    buffer.insert(buffer.cbegin() + 2, 7);
    ASSERT_TRUE(buffer == res6);
}

TEST(CircularBufferExtMethods, InsertTest2) {
    CCirtucalBufferExt<std::string> res1{"1", "1", "1"};
    CCirtucalBufferExt<std::string> res2{"1", "1", "1", "2", "2", "2"};
    CCirtucalBufferExt<std::string> buffer1;
    buffer1.insert(buffer1.cbegin(), 3, "1");
    ASSERT_TRUE(buffer1 == res1);
    buffer1.insert(buffer1.cend(), 3, "2");
    ASSERT_TRUE(buffer1 == res2);
}

TEST(CircularBufferExtMethods, InsertTest3) {
    CCirtucalBufferExt<int> buffer1;
    buffer1.insert(buffer1.cbegin(), {1, 2, 3, 4, 5});
    buffer1.insert(buffer1.cend(), {1, 2, 3, 4, 5});
    CCirtucalBufferExt<int> res = {1, 2, 3, 4, 5, 1, 2, 3, 4, 5};
    ASSERT_TRUE(buffer1 == res);
}

TEST(CircularBufferExtMethods, EraseTest) {
    CCirtucalBufferExt<int> buffer1 = {1, 2, 3, 4, 5, 6};
    buffer1.erase(buffer1.cbegin() + 2);
    CCirtucalBufferExt<int> res = {1, 2, 4, 5, 6};
    ASSERT_TRUE(buffer1 == res);
    buffer1.erase(buffer1.cbegin() + 1, buffer1.cend() - 1);
    CCirtucalBufferExt<int> res2 = {1, 6};
    ASSERT_TRUE(buffer1 == res2);
}

TEST(CircularBufferExtMethods, FrontBackTest) {
    CCirtucalBufferExt<int> buffer1 = {1, 2, 3, 4, 5, 6};
    ASSERT_EQ(1, buffer1.front());
    ASSERT_EQ(6, buffer1.back());
    buffer1.push_back(7);
    buffer1.push_back(8);
    CCirtucalBufferExt<int> res1 = {1, 2, 3, 4, 5, 6, 7, 8};
    ASSERT_TRUE(buffer1 == res1);
    buffer1.pop_back();
    CCirtucalBufferExt<int> res2 = {1, 2, 3, 4, 5, 6, 7};
    ASSERT_TRUE(buffer1 == res2);
}
