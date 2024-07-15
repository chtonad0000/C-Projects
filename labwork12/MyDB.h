#pragma once

#include <string>
#include <unordered_map>
#include <vector>

class Table {
public:
    friend bool operator==(const Table& lhs, const Table& rhs);

    friend bool operator!=(const Table& lhs, const Table& rhs);

    friend class MyDB;

    explicit Table(const std::vector<std::unordered_map<std::string, std::string>>& data) : data(data) {
    };

    Table() = default;

private:
    std::vector<std::unordered_map<std::string, std::string>> data;
    std::string foreign_key;
    std::string reference;
    std::string primary_key;
    bool empty = false;
};

bool operator==(const Table& lhs, const Table& rhs);

bool operator!=(const Table& lhs, const Table& rhs);

class MyDB {
public:
    MyDB(const std::unordered_map<std::string, Table>& tables) : tables(tables) {
    }

    MyDB();

    Table Request(const std::string& str);

    std::string PrintDB();

    void SaveIntoFile(const std::string& file_name);

private:
    void ParseRequest(std::vector<std::vector<std::string>>& request);

    int StringCompare(const std::string& str1, const std::string& str2, int index);

    void InsertParse(const std::vector<std::string>& insert_request);

    void CreateTableParse(const std::vector<std::string>& create_request);

    void DropTableParse(const std::vector<std::string>& drop_request);

    std::vector<int> WHEREParse(const std::vector<std::string>& where_request);

    void DELETEParse(const std::vector<std::string>& delete_request, const std::vector<std::string>& where_request);

    std::vector<int> find_strings(const std::vector<std::string>& condition);

    void FindLines(const std::vector<int>& lines);

    void SELECTParse(const std::vector<std::string>& select_request);

    void JOINParse(const std::vector<std::string>& join_request);

    std::vector<std::pair<std::string, std::string>> SETParse(const std::vector<std::string>& set_request);

    void UPDATEPArse(const std::vector<std::string>& update_request,
                     const std::vector<std::pair<std::string, std::string>>& changes,
                     const std::vector<std::string>& where_request);

    std::unordered_map<std::string, Table> tables;
    std::unordered_map<std::string, Table> current_db;
    std::string selected_table;
    std::string output_data;
};

