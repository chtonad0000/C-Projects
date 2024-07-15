#include <gtest/gtest.h>
#include "MyDB.h"

Table customers({{{"Name", "Daniil"}, {"Age", "18"}, {"City", "Saint-Petersburg"}, {"ID", "12"}},
                 {{"Name", "Sofia"},  {"Age", "18"}, {"City", "Rostov-on-Don"},    {"ID", "13"}},
                 {{"Name", "Ivan"},   {"Age", "20"}, {"City", "Moscow"},           {"ID", "14"}},
                 {{"Name", "Egor"},   {"Age", "10"}, {"City", "Samara"},           {"ID", "15"}},
                 {{"Name", "Vito"},   {"Age", "28"}, {"City", "Saint-Petersburg"}, {"ID", "17"}}});
Table purchases({{{"Item", "Apple"},  {"Price", "0.99"}, {"Quantity", "5"}, {"ID", "12"}},
                 {{"Item", "Banana"}, {"Price", "0.50"}, {"Quantity", "3"}, {"ID", "12"}},
                 {{"Item", "Orange"}, {"Price", "0.75"}, {"Quantity", "2"}, {"ID", "15"}},
                 {{"Item", "Mango"},  {"Price", "10"},   {"Quantity", "5"}, {"ID", "16"}}});

MyDB database_test({{"customers", customers},
                    {"purchases", purchases}});

TEST(SelectTests, SelectAll) {
    auto database = database_test;
    Table result = database.Request(R"(SELECT * FROM customers)");
    Table expected({{{"Name", "Daniil"}, {"Age", "18"}, {"City", "Saint-Petersburg"}, {"ID", "12"}},
                    {{"Name", "Sofia"},  {"Age", "18"}, {"City", "Rostov-on-Don"},    {"ID", "13"}},
                    {{"Name", "Ivan"},   {"Age", "20"}, {"City", "Moscow"},           {"ID", "14"}},
                    {{"Name", "Egor"},   {"Age", "10"}, {"City", "Samara"},           {"ID", "15"}},
                    {{"Name", "Vito"},   {"Age", "28"}, {"City", "Saint-Petersburg"}, {"ID", "17"}}});
    EXPECT_EQ(result, expected);
}

TEST(SelectTests, SelectOne) {
    auto database = database_test;
    Table result = database.Request(R"(SELECT Name FROM customers)");
    Table expected({{{"Name", "Daniil"}},
                    {{"Name", "Sofia"}},
                    {{"Name", "Ivan"}},
                    {{"Name", "Egor"}},
                    {{"Name", "Vito"}}});
    EXPECT_EQ(result, expected);
}

TEST(SelectTests, SelectTwo) {
    auto database = database_test;
    Table result = database.Request(R"(SELECT Age, City FROM customers)");
    Table expected({{{"Age", "18"}, {"City", "Saint-Petersburg"}},
                    {{"Age", "18"}, {"City", "Rostov-on-Don"}},
                    {{"Age", "20"}, {"City", "Moscow"}},
                    {{"Age", "10"}, {"City", "Samara"}},
                    {{"Age", "28"}, {"City", "Saint-Petersburg"}}});
    EXPECT_EQ(result, expected);
}

TEST(SelectTests, SelectTwo2) {
    auto database = database_test;
    Table result = database.Request(R"(SELECT Age, Name FROM customers)");
    Table expected({{{"Age", "18"}, {"Name", "Daniil"}},
                    {{"Age", "18"}, {"Name", "Sofia"}},
                    {{"Age", "20"}, {"Name", "Ivan"}},
                    {{"Age", "10"}, {"Name", "Egor"}},
                    {{"Age", "28"}, {"Name", "Vito"}}});
    EXPECT_EQ(result, expected);
}

TEST(SelectTests, SelectThree) {
    auto database = database_test;
    Table result = database.Request(R"(SELECT Name, Age, City FROM customers)");
    Table expected({{{"Name", "Daniil"}, {"Age", "18"}, {"City", "Saint-Petersburg"}},
                    {{"Name", "Sofia"},  {"Age", "18"}, {"City", "Rostov-on-Don"}},
                    {{"Name", "Ivan"},   {"Age", "20"}, {"City", "Moscow"}},
                    {{"Name", "Egor"},   {"Age", "10"}, {"City", "Samara"}},
                    {{"Name", "Vito"},   {"Age", "28"}, {"City", "Saint-Petersburg"}}});
    EXPECT_EQ(result, expected);
}

TEST(SelectTests, SelectThree2) {
    auto database = database_test;
    Table result = database.Request(R"(SELECT Name, ID, City FROM customers)");
    Table expected({{{"Name", "Daniil"}, {"ID", "12"}, {"City", "Saint-Petersburg"}},
                    {{"Name", "Sofia"},  {"ID", "13"}, {"City", "Rostov-on-Don"}},
                    {{"Name", "Ivan"},   {"ID", "14"}, {"City", "Moscow"}},
                    {{"Name", "Egor"},   {"ID", "15"}, {"City", "Samara"}},
                    {{"Name", "Vito"},   {"ID", "17"}, {"City", "Saint-Petersburg"}}});
    EXPECT_EQ(result, expected);
}

TEST(SelectTests, SelectExceptionTest) {
    auto database = database_test;
    EXPECT_THROW(database.Request(R"(SELECT * FROM custom)"), std::invalid_argument);
}

TEST(SelectTests, SelectExceptionTest2) {
    auto database = database_test;
    EXPECT_THROW(database.Request(R"(SELECT Age, Weight FROM customers)"), std::invalid_argument);
}

TEST(WhereTests, WhereTestGreater) {
    auto database = database_test;
    Table result = database.Request(R"(SELECT * FROM customers
WHERE Age > 18)");
    Table expected({{{"Name", "Ivan"}, {"Age", "20"}, {"City", "Moscow"},           {"ID", "14"}},
                    {{"Name", "Vito"}, {"Age", "28"}, {"City", "Saint-Petersburg"}, {"ID", "17"}}});
    EXPECT_EQ(result, expected);
}

TEST(WhereTests, WhereTestLess) {
    auto database = database_test;
    Table result = database.Request(R"(SELECT * FROM customers
WHERE Age < 19)");
    Table expected({{{"Name", "Daniil"}, {"Age", "18"}, {"City", "Saint-Petersburg"}, {"ID", "12"}},
                    {{"Name", "Sofia"},  {"Age", "18"}, {"City", "Rostov-on-Don"},    {"ID", "13"}},
                    {{"Name", "Egor"},   {"Age", "10"}, {"City", "Samara"},           {"ID", "15"}}});
    EXPECT_EQ(result, expected);
}

TEST(WhereTests, WhereTestEq) {
    auto database = database_test;
    Table result = database.Request(R"(SELECT Name, Age FROM customers
WHERE Age = 18)");
    Table expected({{{"Name", "Daniil"}, {"Age", "18"}},
                    {{"Name", "Sofia"},  {"Age", "18"}}});
    EXPECT_EQ(result, expected);
}

TEST(WhereTests, WhereTestMultiConditions) {
    auto database = database_test;
    Table result = database.Request(R"(SELECT * FROM customers
WHERE Age < 19 AND City = Saint-Petersburg)");
    Table expected(std::vector<std::unordered_map<std::string, std::string>>{
            {{"Name", "Daniil"}, {"Age", "18"}, {"City", "Saint-Petersburg"}, {"ID", "12"}}});
    EXPECT_EQ(result, expected);
}

TEST(WhereTests, WhereTestMultiConditions2) {
    auto database = database_test;
    Table result = database.Request(R"(SELECT Name, Age, City FROM customers
WHERE Age < 19 OR City IS NOT Saint-Petersburg)");
    Table expected({{{"Name", "Daniil"}, {"Age", "18"}, {"City", "Saint-Petersburg"}},
                    {{"Name", "Sofia"},  {"Age", "18"}, {"City", "Rostov-on-Don"}},
                    {{"Name", "Ivan"},   {"Age", "20"}, {"City", "Moscow"}},
                    {{"Name", "Egor"},   {"Age", "10"}, {"City", "Samara"}}});
    EXPECT_EQ(result, expected);
}

TEST(WhereTests, WhereTestMultiConditions3) {
    auto database = database_test;
    Table result = database.Request(R"(SELECT * FROM customers
WHERE Age < 19 OR City IS NOT Saint-Petersburg OR Name = Vito)");
    Table expected({{{"Name", "Daniil"}, {"Age", "18"}, {"City", "Saint-Petersburg"}, {"ID", "12"}},
                    {{"Name", "Sofia"},  {"Age", "18"}, {"City", "Rostov-on-Don"},    {"ID", "13"}},
                    {{"Name", "Ivan"},   {"Age", "20"}, {"City", "Moscow"},           {"ID", "14"}},
                    {{"Name", "Egor"},   {"Age", "10"}, {"City", "Samara"},           {"ID", "15"}},
                    {{"Name", "Vito"},   {"Age", "28"}, {"City", "Saint-Petersburg"}, {"ID", "17"}}});
    EXPECT_EQ(result, expected);
}

TEST(WhereTests, WhereExceptionTest) {
    auto database = database_test;
    std::string request = R"(SELECT Age, Name FROM customers
WHERE Ag > 5)";
    EXPECT_THROW(database.Request(request), std::invalid_argument);
}

TEST(WhereTests, WhereExceptionTest2) {
    auto database = database_test;
    std::string request = R"(SELECT Age, Name FROM customers
WHERE Age > 5 SELECT ID = 32)";
    EXPECT_THROW(database.Request(request), std::invalid_argument);
}

TEST (JoinTests, InnerJointest) {
    auto database = database_test;
    Table result = database.Request(R"(SELECT Name, Age, City FROM customers
INNER JOIN purchases (Item, Price) ON ID = ID)");
    Table expected(
            {{{"Name", "Daniil"}, {"Age", "18"}, {"City", "Saint-Petersburg"}, {"Item", "Apple"},  {"Price", "0.99"}},
             {{"Name", "Daniil"}, {"Age", "18"}, {"City", "Saint-Petersburg"}, {"Item", "Banana"}, {"Price", "0.50"}},
             {{"Name", "Egor"},   {"Age", "10"}, {"City", "Samara"},           {"Item", "Orange"}, {"Price", "0.75"}}});
    EXPECT_EQ(result, expected);
}

TEST (JoinTests, LeftJointest) {
    auto database = database_test;
    Table result = database.Request(R"(SELECT Name, Age, City FROM customers
LEFT JOIN purchases (Item, Price) ON ID = ID)");
    Table expected(
            {{{"Name", "Daniil"}, {"Age", "18"}, {"City", "Saint-Petersburg"}, {"Item", "Apple"},  {"Price", "0.99"}},
             {{"Name", "Daniil"}, {"Age", "18"}, {"City", "Saint-Petersburg"}, {"Item", "Banana"}, {"Price", "0.50"}},
             {{"Name", "Sofia"},  {"Age", "18"}, {"City", "Rostov-on-Don"},    {"Item", "NULL"},   {"Price", "NULL"}},
             {{"Name", "Ivan"},   {"Age", "20"}, {"City", "Moscow"},           {"Item", "NULL"},   {"Price", "NULL"}},
             {{"Name", "Egor"},   {"Age", "10"}, {"City", "Samara"},           {"Item", "Orange"}, {"Price", "0.75"}},
             {{"Name", "Vito"},   {"Age", "28"}, {"City", "Saint-Petersburg"}, {"Item", "NULL"},   {"Price", "NULL"}}});
    EXPECT_EQ(result, expected);
}

TEST (JoinTests, RightJointest) {
    auto database = database_test;
    Table result = database.Request(R"(SELECT Name, Age, City FROM customers
RIGHT JOIN purchases (Item, Price) ON ID = ID)");
    Table expected(
            {{{"Name", "Daniil"}, {"Age", "18"},   {"City", "Saint-Petersburg"}, {"Item", "Apple"},  {"Price", "0.99"}},
             {{"Name", "Daniil"}, {"Age", "18"},   {"City", "Saint-Petersburg"}, {"Item", "Banana"}, {"Price", "0.50"}},
             {{"Name", "Egor"},   {"Age", "10"},   {"City", "Samara"},           {"Item", "Orange"}, {"Price", "0.75"}},
             {{"Name", "NULL"},   {"Age", "NULL"}, {"City", "NULL"},             {"Item", "Mango"},  {"Price", "10"}}});
    EXPECT_EQ(result, expected);
}

TEST (CreateTableTest, CreateTable) {
    auto database = database_test;
    Table result = database.Request(R"(CREATE TABLE Shops (Name, City, Rating, Type)
SELECT * FROM Shops)");
    Table expected(std::vector<std::unordered_map<std::string, std::string>>{
            {{"Name", "NULL"}, {"City", "NULL"}, {"Rating", "NULL"}, {"Type", "NULL"}}});
    EXPECT_EQ(result, expected);
}

TEST (CreateTableTest, CreateTable2) {
    auto database = database_test;
    Table result = database.Request(R"(CREATE TABLE University (Name, City, Date, Minimal price)
SELECT * FROM University)");
    Table expected(std::vector<std::unordered_map<std::string, std::string>>{
            {{"Name", "NULL"}, {"City", "NULL"}, {"Date", "NULL"}, {"Minimal price", "NULL"}}});
    EXPECT_EQ(result, expected);
}

TEST (DropTableTest, DropTable) {
    auto database = database_test;
    std::string request = R"(DROP TABLE customers
SELECT * FROM customers)";
    EXPECT_THROW(database.Request(request), std::invalid_argument);
}

TEST(InsertTest, Insert1) {
    auto database = database_test;
    Table result = database.Request(R"(INSERT INTO customers (Name, Age, City, ID) VALUES (Gleb, 36, Novgorod, 20)
SELECT * FROM customers)");
    Table expected({{{"Name", "Daniil"}, {"Age", "18"}, {"City", "Saint-Petersburg"}, {"ID", "12"}},
                    {{"Name", "Sofia"},  {"Age", "18"}, {"City", "Rostov-on-Don"},    {"ID", "13"}},
                    {{"Name", "Ivan"},   {"Age", "20"}, {"City", "Moscow"},           {"ID", "14"}},
                    {{"Name", "Egor"},   {"Age", "10"}, {"City", "Samara"},           {"ID", "15"}},
                    {{"Name", "Vito"},   {"Age", "28"}, {"City", "Saint-Petersburg"}, {"ID", "17"}},
                    {{"Name", "Gleb"},   {"Age", "36"}, {"City", "Novgorod"},         {"ID", "20"}}});
    EXPECT_EQ(result, expected);
}

TEST(InsertTest, CreateAndInsert) {
    auto database = database_test;
    Table result = database.Request(R"(CREATE TABLE shops (Name, City, Rating, Type)
INSERT INTO shops (Name, City, Rating, Type) VALUES (Dixy, Saint-Petersburg, 9, Grocery)
SELECT * FROM shops)");
    Table expected(std::vector<std::unordered_map<std::string, std::string>>{
            {{"Name", "Dixy"}, {"City", "Saint-Petersburg"}, {"Rating", "9"}, {"Type", "Grocery"}}});
    EXPECT_EQ(result, expected);
}

TEST(InsertTest, CreateAndMultiInsert) {
    auto database = database_test;
    Table result = database.Request(R"(CREATE TABLE shops (Name, City, Rating, Type, ID)
INSERT INTO shops (Name, City, Rating, Type, ID) VALUES (Dixy, Saint-Petersburg, 9, Grocery, 20)
INSERT INTO shops (Name, City, Rating, Type) VALUES (MVideo, Rostov-on-Don, 7, Electronics)
SELECT * FROM shops)");
    Table expected(
            {{{"Name", "Dixy"},   {"City", "Saint-Petersburg"}, {"Rating", "9"}, {"Type", "Grocery"},     {"ID", "20"}},
             {{"Name", "MVideo"}, {"City", "Rostov-on-Don"},    {"Rating", "7"}, {"Type", "Electronics"}, {"ID", "NULL"}}});
    EXPECT_EQ(result, expected);
}

TEST(DeleteTest, Delete) {
    auto database = database_test;
    Table result = database.Request(R"(DELETE FROM customers
WHERE ID IS 12
SELECT * FROM customers)");
    Table expected({{{"Name", "Sofia"}, {"Age", "18"}, {"City", "Rostov-on-Don"},    {"ID", "13"}},
                    {{"Name", "Ivan"},  {"Age", "20"}, {"City", "Moscow"},           {"ID", "14"}},
                    {{"Name", "Egor"},  {"Age", "10"}, {"City", "Samara"},           {"ID", "15"}},
                    {{"Name", "Vito"},  {"Age", "28"}, {"City", "Saint-Petersburg"}, {"ID", "17"}}});
    EXPECT_EQ(result, expected);
}

TEST(DeleteTest, MultiDelete) {
    auto database = database_test;
    Table result = database.Request(R"(DELETE FROM customers
WHERE ID IS 12
DELETE FROM customers
WHERE Age < 19
SELECT * FROM customers)");
    Table expected({{{"Name", "Ivan"}, {"Age", "20"}, {"City", "Moscow"},           {"ID", "14"}},
                    {{"Name", "Vito"}, {"Age", "28"}, {"City", "Saint-Petersburg"}, {"ID", "17"}}});
    EXPECT_EQ(result, expected);
}

TEST (DeleteTest, DeleteAfterInsert) {
    auto database = database_test;
    Table result = database.Request(R"(CREATE TABLE shops (Name, City, Rating, Type, ID)
INSERT INTO shops (Name, City, Rating, Type, ID) VALUES (Dixy, Saint-Petersburg, 9, Grocery, 20)
INSERT INTO shops (Name, City, Rating, Type) VALUES (MVideo, Rostov-on-Don, 7, Electronics)
DELETE FROM shops
WHERE ID IS NULL
SELECT * FROM shops)");
    Table expected(std::vector<std::unordered_map<std::string, std::string>>{
            {{"Name", "Dixy"}, {"City", "Saint-Petersburg"}, {"Rating", "9"}, {"Type", "Grocery"}, {"ID", "20"}}});
    EXPECT_EQ(result, expected);
}

TEST (UpdateTest, Update) {
    auto database = database_test;
    Table result = database.Request(R"(UPDATE customers
SET Age = 56
WHERE ID = 15
SELECT * FROM customers)");
    Table expected({{{"Name", "Daniil"}, {"Age", "18"}, {"City", "Saint-Petersburg"}, {"ID", "12"}},
                    {{"Name", "Sofia"},  {"Age", "18"}, {"City", "Rostov-on-Don"},    {"ID", "13"}},
                    {{"Name", "Ivan"},   {"Age", "20"}, {"City", "Moscow"},           {"ID", "14"}},
                    {{"Name", "Egor"},   {"Age", "56"}, {"City", "Samara"},           {"ID", "15"}},
                    {{"Name", "Vito"},   {"Age", "28"}, {"City", "Saint-Petersburg"}, {"ID", "17"}}});
    EXPECT_EQ(result, expected);
}

TEST (UpdateTest, UpdateMulti) {
    auto database = database_test;
    Table result = database.Request(R"(UPDATE customers
SET Age = 777, Name = Peter
WHERE City = Saint-Petersburg
SELECT * FROM customers)");
    Table expected({{{"Name", "Peter"}, {"Age", "777"}, {"City", "Saint-Petersburg"}, {"ID", "12"}},
                    {{"Name", "Sofia"}, {"Age", "18"},  {"City", "Rostov-on-Don"},    {"ID", "13"}},
                    {{"Name", "Ivan"},  {"Age", "20"},  {"City", "Moscow"},           {"ID", "14"}},
                    {{"Name", "Egor"},  {"Age", "10"},  {"City", "Samara"},           {"ID", "15"}},
                    {{"Name", "Peter"}, {"Age", "777"}, {"City", "Saint-Petersburg"}, {"ID", "17"}}});
    EXPECT_EQ(result, expected);
}

TEST (UpdateTest, UpdateMulti2) {
    auto database = database_test;
    Table result = database.Request(R"(UPDATE customers
SET Age = 34, Name = Brat
WHERE City = Saint-Petersburg AND Name = Vito
SELECT * FROM customers)");
    Table expected({{{"Name", "Daniil"}, {"Age", "18"}, {"City", "Saint-Petersburg"}, {"ID", "12"}},
                    {{"Name", "Sofia"},  {"Age", "18"}, {"City", "Rostov-on-Don"},    {"ID", "13"}},
                    {{"Name", "Ivan"},   {"Age", "20"}, {"City", "Moscow"},           {"ID", "14"}},
                    {{"Name", "Egor"},   {"Age", "10"}, {"City", "Samara"},           {"ID", "15"}},
                    {{"Name", "Brat"},   {"Age", "34"}, {"City", "Saint-Petersburg"}, {"ID", "17"}}});
    EXPECT_EQ(result, expected);
}

TEST (UpdateTest, UpdateMulti3) {
    auto database = database_test;
    Table result = database.Request(R"(UPDATE customers
SET Age = 12, Name = NONAME
WHERE Age < 19 AND City IS NOT Samara
SELECT * FROM customers)");
    Table expected({{{"Name", "NONAME"}, {"Age", "12"}, {"City", "Saint-Petersburg"}, {"ID", "12"}},
                    {{"Name", "NONAME"}, {"Age", "12"}, {"City", "Rostov-on-Don"},    {"ID", "13"}},
                    {{"Name", "Ivan"},   {"Age", "20"}, {"City", "Moscow"},           {"ID", "14"}},
                    {{"Name", "Egor"},   {"Age", "10"}, {"City", "Samara"},           {"ID", "15"}},
                    {{"Name", "Vito"},   {"Age", "28"}, {"City", "Saint-Petersburg"}, {"ID", "17"}}});
    EXPECT_EQ(result, expected);
}

TEST (PrimaryKeyTest, DiffrentPrimaryKey) {
    auto database = database_test;
    Table result = database.Request(R"(CREATE TABLE Shops (Name, City, ID, PRIMARY KEY (ID))
INSERT INTO Shops (Name, City, ID) VALUES (Dixy, Saint-Petersburg, 1)
INSERT INTO Shops (Name, City, ID) VALUES (MVideo, Moscow, 2)
SELECT * FROM Shops)");
    Table expected({{{"Name", "Dixy"},   {"City", "Saint-Petersburg"}, {"ID", "1"}},
                    {{"Name", "MVideo"}, {"City", "Moscow"},           {"ID", "2"}}});
    EXPECT_EQ(result, expected);
}

TEST (PrimaryKeyTest, SamePrimaryKey) {
    auto database = database_test;
    std::string request = R"(CREATE TABLE Shops (Name, City, ID, PRIMARY KEY (ID))
INSERT INTO Shops (Name, City, ID) VALUES (Dixy, Saint-Petersburg, 1)
INSERT INTO Shops (Name, City, ID) VALUES (MVideo, Moscow, 1)
SELECT * FROM Shops)";

    EXPECT_THROW(database.Request(request), std::invalid_argument);
}

