#include "MyDB.h"

#include <fstream>
#include <functional>
#include <iterator>
#include <set>
#include <sstream>

const int kFromIndex = 1;
const int kDeleteNameTableIndex = 2;
const int kFirstElement = 0;
const int kColumnsIndex = 1;
const int kDropTableNameIndex = 1;
const int kTableNameIndex = 3;
const int kValuesIndex = 3;
const int kConditionIndex = 1;
const int kFirstCondition = 0;
const int kSecondCondition = 2;
const int kOnIndex = 2;
const int kJoinType = 0;
const int kJoinConditionsIndex = 3;
const int kUpdateTableName = 1;
const int kReferenceSize = 11;
const int kValuePosition = 2;

std::set<std::string> key_words = {"SELECT", "FROM", "WHERE", "LEFT", "RIGHT", "INNER", "CREATE", "DROP", "AND", "OR",
                                   "IS", "NOT", "NULL", "ON", "UPDATE", "INSERT", "VALUES", "DELETE", "SET", "<", ">",
                                   "!=", "=", ">=", "<="};

Table MyDB::Request(const std::string& str) {
    std::string one_str;
    std::vector<std::vector<std::string>> result;
    for (int j = 0; j < str.size() + 1; ++j) {
        if (str[j] == '\n' | str[j] == '\0') {
            std::vector<std::string> request;
            std::istringstream iss(one_str);
            std::string arg;
            std::vector<std::string> parse_str = {std::istream_iterator<std::string>(iss),
                                                  std::istream_iterator<std::string>()};
            for (int i = 0; i < parse_str.size(); ++i) {
                if (key_words.contains(parse_str[i])) {
                    if (!arg.empty()) {
                        request.emplace_back(arg.substr(0, arg.size() - 1));
                    }
                    arg = "";
                    if (parse_str[i] == "INSERT") {
                        if (parse_str[i + 1] != "INTO") {
                            throw std::invalid_argument("No INTO after INSERT");
                        }
                        request.emplace_back("INSERT INTO");
                        ++i;
                    } else if (parse_str[i] == "CREATE") {
                        if (parse_str[i + 1] != "TABLE") {
                            throw std::invalid_argument("No TABLE after CREATE");
                        }
                        request.emplace_back("CREATE TABLE");
                        ++i;
                    } else if (parse_str[i] == "DROP") {
                        if (parse_str[i + 1] != "TABLE") {
                            throw std::invalid_argument("No TABLE after DROP");
                        }
                        request.emplace_back("DROP TABLE");
                        ++i;
                    } else if (parse_str[i] == "INNER") {
                        if (parse_str[i + 1] != "JOIN") {
                            throw std::invalid_argument("No JOIN after INNER");
                        }
                        request.emplace_back("INNER JOIN");
                        ++i;
                    } else if (parse_str[i] == "RIGHT") {
                        if (parse_str[i + 1] != "JOIN") {
                            throw std::invalid_argument("No JOIN after RIGHT");
                        }
                        request.emplace_back("RIGHT JOIN");
                        ++i;
                    } else if (parse_str[i] == "LEFT") {
                        if (parse_str[i + 1] != "JOIN") {
                            throw std::invalid_argument("No JOIN after LEFT");
                        }
                        request.emplace_back("LEFT JOIN");
                        ++i;
                    } else {
                        request.emplace_back(parse_str[i]);
                    }
                } else {
                    arg += parse_str[i] + ' ';
                }
            }
            if (arg[arg.size() - 1] == ' ') {
                request.emplace_back(arg.substr(0, arg.size() - 1));
            } else {
                if (!arg.empty()) {
                    request.emplace_back(arg);
                }
            }
            one_str = "";
            result.emplace_back(request);
            continue;
        }
        one_str += str[j];
    }
    ParseRequest(result);

    return current_db[selected_table];
}

std::string MyDB::PrintDB() {
    std::string result;
    std::vector<size_t> positions(current_db[selected_table].data[kFirstElement].size(), 0);
    for (int i = 0; i < current_db[selected_table].data.size(); ++i) {
        int k = 0;
        for (auto it = current_db[selected_table].data[i].begin();
             it != current_db[selected_table].data[i].end(); ++it) {
            size_t max = ((it->first).size() > (it->second).size()) ? (it->first).size() : (it->second).size();
            if (max > positions[k]) {
                positions[k] = max;
            }
            ++k;
        }
    }
    int pos = 0;
    for (auto& k: current_db[selected_table].data[kFirstElement]) {
        result += k.first;
        for (size_t i = 0; i < positions[pos] + 1 - (k.first).size(); ++i) {
            result += ' ';
        }
        ++pos;
        result += "| ";
    }
    result += '\n';
    pos = 0;
    for (int i = 0; i < current_db[selected_table].data.size(); ++i) {
        for (auto& k: current_db[selected_table].data[i]) {
            result += k.second;
            for (int j = 0; j < positions[pos] + 1 - (k.second).size(); ++j) {
                result += ' ';
            }
            ++pos;
            result += "| ";
        }
        pos = 0;
        result += '\n';
    }

    return result;
}

void MyDB::SaveIntoFile(const std::string& file_name) {
    std::ofstream file(file_name);
    if (!file.is_open()) {
        throw std::invalid_argument("Wrong file name");
    }
    file << PrintDB();

    file.close();
}

bool IsInteger(const std::string& str) {
    std::istringstream iss(str);
    int num;
    iss >> num;

    return iss.eof() && !iss.fail();
}

bool IsFloat(const std::string& str) {
    std::istringstream iss(str);
    float num;
    iss >> num;

    return iss.eof() && !iss.fail();
}

bool IsDouble(const std::string& str) {
    std::istringstream iss(str);
    double num;
    iss >> num;

    return iss.eof() && !iss.fail();
}

bool IsBoolean(const std::string& str) {
    return str == "true" || str == "false";
}

std::vector<std::string> GetNames(const std::string& add_names) {
    std::string value_name;
    std::vector<std::string> values_name;
    for (int i = 0; i < add_names.size() + 1; ++i) {
        if (add_names[i] == ',' || add_names[i] == '\0') {
            values_name.emplace_back(value_name);
            value_name = "";
            i = i + 1;
            continue;
        }
        value_name += add_names[i];
    }

    return values_name;
}

int MyDB::StringCompare(const std::string& str1, const std::string& str2, int index) {
    if (IsInteger(tables[selected_table].data[index][str1]) && IsInteger(str2)) {
        if (std::stoi(tables[selected_table].data[index][str1]) - std::stoi(str2) < 0) {
            return -1;
        } else if (std::stoi(tables[selected_table].data[index][str1]) - std::stoi(str2) == 0) {
            return 0;
        } else {
            return 1;
        }
    } else if (IsDouble(tables[selected_table].data[index][str1]) && IsDouble(str2)) {
        if (std::stod(tables[selected_table].data[index][str1]) - std::stod(str2) < 0) {
            return -1;
        } else if (std::stod(tables[selected_table].data[index][str1]) - std::stod(str2) == 0) {
            return 0;
        } else {
            return 1;
        }
    } else if (IsFloat(tables[selected_table].data[index][str1]) && IsFloat(str2)) {
        if (std::stof(tables[selected_table].data[index][str1]) - std::stof(str2) < 0) {
            return -1;
        } else if (std::stof(tables[selected_table].data[index][str1]) - std::stof(str2) == 0) {
            return 0;
        } else {
            return 1;
        }
    } else if (IsBoolean(tables[selected_table].data[index][str1]) && IsBoolean(str2)) {
        if (tables[selected_table].data[index][str1] == "false" && str2 == "true") {
            return -1;
        } else if (tables[selected_table].data[index][str1] == str2) {
            return 0;
        } else {
            return 1;
        }
    } else {
        int result = tables[selected_table].data[index][str1].compare(str2);
        if (result < 0) {
            return -1;
        } else if (result == 0) {
            return 0;
        } else {
            return 1;
        }
    }
}


std::vector<int> FindIntersection(const std::vector<int>& vec1, const std::vector<int>& vec2) {
    std::vector<int> intersection;
    std::set_intersection(vec1.begin(), vec1.end(), vec2.begin(), vec2.end(), std::back_inserter(intersection));

    return intersection;
}

std::vector<int> FindUnion(const std::vector<int>& vec1, const std::vector<int>& vec2) {
    std::vector<int> union_vec;
    std::set_union(vec1.begin(), vec1.end(), vec2.begin(), vec2.end(), std::back_inserter(union_vec));

    return union_vec;
}


void MyDB::ParseRequest(std::vector<std::vector<std::string>>& request) {
    for (int i = 0; i < request.size(); ++i) {
        if (request[i][kFirstElement] == "WHERE") {
            FindLines(WHEREParse(request[i]));
        } else if (request[i][kFirstElement] == "SELECT") {
            SELECTParse(request[i]);
        } else if (request[i][kFirstElement] == "CREATE TABLE") {
            CreateTableParse(request[i]);
        } else if (request[i][kFirstElement] == "DROP TABLE") {
            DropTableParse(request[i]);
        } else if (request[i][kFirstElement] == "INSERT INTO") {
            InsertParse(request[i]);
        } else if (request[i][kFirstElement] == "DELETE") {
            DELETEParse(request[i], request[i + 1]);
            ++i;
        } else if (request[i][kFirstElement].find("JOIN", 0) != std::string::npos) {
            JOINParse(request[i]);
        } else if (request[i][kFirstElement] == "UPDATE") {
            current_db = tables;
            if (request[i + 1][kFirstElement] != "SET") {
                throw std::invalid_argument("Wrong request(must be SET): " + request[i + 1][kFirstElement]);
            }
            if (request[i + 2][kFirstElement] != "WHERE") {
                throw std::invalid_argument("Wrong request(must be WHERE): " + request[i + 2][kFirstElement]);
            }
            UPDATEPArse(request[i], SETParse(request[i + 1]), request[i + 2]);
            i += 2;
        } else {
            throw std::invalid_argument("Wrong request " + request[i][kFirstElement]);
        }
    }

}

std::vector<int> MyDB::WHEREParse(const std::vector<std::string>& where_request) {
    std::vector<std::vector<int>> table_strings;
    std::vector<std::string> operands;
    std::vector<int> strings;
    for (int i = 1; i < where_request.size(); ++i) {
        if (!key_words.contains(where_request[i])) {
            std::vector<std::string> condition;
            condition.emplace_back(where_request[i]);
            if (where_request[i + 2] == "NOT") {
                condition.emplace_back("!=");
                if (where_request[i + 3] == "NULL") {
                    condition.emplace_back("NULL");
                    strings = find_strings(condition);
                    i += 3;
                } else {
                    condition.emplace_back(where_request[i + 3]);
                    strings = find_strings(condition);
                    i += 3;
                }
            } else {
                if (where_request[i + 1] == "IS") {
                    condition.emplace_back("=");
                } else {
                    condition.emplace_back(where_request[i + 1]);
                }
                if (where_request[i + 2] == "NULL") {
                    condition.emplace_back("NULL");
                    strings = find_strings(condition);
                    i += 2;
                } else {
                    condition.emplace_back(where_request[i + 2]);
                    strings = find_strings(condition);
                    i += 2;
                }
            }
            table_strings.emplace_back(strings);
        } else {
            if (where_request[i] == "AND") {
                operands.emplace_back("AND");
            } else if (where_request[i] == "OR") {
                operands.emplace_back("OR");
            } else {
                throw std::invalid_argument("Can't use this keyword with WHERE: " + where_request[i]);
            }
        }
    }

    int k = 0;
    std::vector<int> result = table_strings[kFirstElement];
    for (int i = 1; i < table_strings.size(); ++i) {
        if (operands[k] == "AND") {
            result = FindIntersection(result, table_strings[i]);
            ++k;
        } else if (operands[k] == "OR") {
            result = FindUnion(result, table_strings[i]);
            ++k;
        }
    }

    return result;
}


void MyDB::SELECTParse(const std::vector<std::string>& select_request) {
    std::string columns_str = select_request[kColumnsIndex];
    if (select_request.size() <= kTableNameIndex) {
        throw std::invalid_argument("No table name after FROM");
    }
    std::string table_name = select_request[kTableNameIndex];
    if (tables.find(table_name) == tables.end()) {
        throw std::invalid_argument("No table with that name");
    }
    std::vector<std::string> columns;
    selected_table = table_name;
    if (columns_str == "*") {
        for (auto& it: tables[table_name].data[kFirstElement]) {
            columns.emplace_back(it.first);
        }
    } else {
        columns = GetNames(columns_str);
    }
    current_db[table_name].data.resize(tables[table_name].data.size());
    for (int i = 0; i < tables[table_name].data.size(); ++i) {
        for (int j = columns.size() - 1; j >= 0; --j) {
            if (tables[table_name].data[i][columns[j]].empty()) {
                throw std::invalid_argument("No column with this name: " + columns[j]);
            }
            current_db[table_name].data[i][columns[j]] = tables[table_name].data[i][columns[j]];
        }
    }
}

std::vector<int> MyDB::find_strings(const std::vector<std::string>& condition) {
    std::vector<int> strings_number;
    std::string symb = condition[kConditionIndex];
    std::string column = condition[kFirstCondition];
    if (tables[selected_table].data[kFirstElement][column].empty()) {
        throw std::invalid_argument("No column with that name: " + column);
    }
    if (symb == "<") {
        for (int i = 0; i < tables[selected_table].data.size(); ++i) {
            if (StringCompare(condition[kFirstCondition], condition[kSecondCondition], i) == -1) {
                strings_number.emplace_back(i);
            }
        }
    }
    if (symb == "<=") {
        for (int i = 0; i < tables[selected_table].data.size(); ++i) {
            if (StringCompare(condition[kFirstCondition], condition[kSecondCondition], i) == -1 ||
                StringCompare(condition[kFirstCondition], condition[kSecondCondition], i) == 0) {
                strings_number.emplace_back(i);
            }
        }
    }
    if (symb == ">") {
        for (int i = 0; i < tables[selected_table].data.size(); ++i) {
            if (StringCompare(condition[kFirstCondition], condition[kSecondCondition], i) == 1) {
                strings_number.emplace_back(i);
            }
        }
    }
    if (symb == ">=") {
        for (int i = 0; i < tables[selected_table].data.size(); ++i) {
            if (StringCompare(condition[kFirstCondition], condition[kSecondCondition], i) == 1 ||
                StringCompare(condition[kFirstCondition], condition[kSecondCondition], i) == 0) {
                strings_number.emplace_back(i);
            }
        }
    }
    if (symb == "=") {
        for (int i = 0; i < tables[selected_table].data.size(); ++i) {
            if (StringCompare(condition[kFirstCondition], condition[kSecondCondition], i) == 0) {
                strings_number.emplace_back(i);
            }
        }
    }
    if (symb == "!=") {
        for (int i = 0; i < tables[selected_table].data.size(); ++i) {
            if (StringCompare(condition[kFirstCondition], condition[kSecondCondition], i) != 0) {
                strings_number.emplace_back(i);
            }
        }
    }

    return strings_number;
}


void MyDB::CreateTableParse(const std::vector<std::string>& create_request) {
    std::string foreign_key_column;
    std::string foreign_key_reference;
    std::string primary_key_column;
    size_t pos = create_request[kColumnsIndex].find(' ', 0);
    std::string table_name(create_request[kColumnsIndex].begin(), create_request[kColumnsIndex].begin() + pos);
    std::string add_columns = create_request[kColumnsIndex].substr(pos + 2,
                                                                   create_request[kColumnsIndex].size() - 3 - pos);
    std::string column_name;
    std::vector<std::string> columns_name;
    for (int i = 0; i < add_columns.size() + 1; ++i) {
        if (add_columns[i] == ',' || add_columns[i] == '\0') {
            columns_name.emplace_back(column_name);
            column_name = "";
            i = i + 1;
            continue;
        }
        column_name += add_columns[i];
        if (column_name == "PRIMARY KEY") {
            i = i + 3;
            if (i >= add_columns.size()) {
                throw std::invalid_argument("Must be value after KEY");
            }
            while (i < add_columns.size() && add_columns[i] != ')') {
                primary_key_column += add_columns[i];
                ++i;
            }
            ++i;
            if (add_columns[i] == ',') {
                column_name = "";
            } else {
                break;
            }
        } else if (column_name == "FOREIGN KEY") {
            i = i + 3;
            if (i >= add_columns.size()) {
                throw std::invalid_argument("Must be value after KEY");
            }
            while (i < add_columns.size() && add_columns[i] != ')') {
                foreign_key_column += add_columns[i];
                ++i;
            }
            if (add_columns.find("REFERENCE", i) == std::string::npos) {
                throw std::invalid_argument("Must be REFERENCE with FOREIGN KEY");
            }
            i = add_columns.find("REFERENCE", i) + kReferenceSize;
            while (i < add_columns.size() && add_columns[i] != ')') {
                foreign_key_reference += add_columns[i];
                ++i;
            }
            if (add_columns[i] == ',') {
                column_name = "";
            } else {
                break;
            }
        }
    }
    tables[table_name].data.emplace_back();
    for (int i = 0; i < columns_name.size(); ++i) {
        tables[table_name].data[kFirstElement].emplace(columns_name[i], "NULL");
    }
    tables[table_name].empty = true;
    tables[table_name].foreign_key = foreign_key_column;
    tables[table_name].reference = foreign_key_reference;
    tables[table_name].primary_key = primary_key_column;
}

void MyDB::DropTableParse(const std::vector<std::string>& drop_request) {
    std::string table_name = drop_request[kDropTableNameIndex];
    if (tables.count(table_name) == 0) {
        throw std::invalid_argument("Can't DROP this table, no table with that name");
    }
    tables.erase(table_name);
}

void MyDB::InsertParse(const std::vector<std::string>& insert_request) {
    size_t pos = insert_request[kColumnsIndex].find(' ', 0);
    std::string table_name(insert_request[kColumnsIndex].begin(), insert_request[kColumnsIndex].begin() + pos);
    std::string add_columns = insert_request[kColumnsIndex].substr(pos + 2,
                                                                   insert_request[kColumnsIndex].size() - 3 - pos);
    std::vector<std::string> columns_name = GetNames(add_columns);
    if (insert_request[kValuePosition] == "VALUES") {
        std::string add_values = insert_request[kValuesIndex].substr(1, insert_request[kValuesIndex].size() - 2);
        std::vector<std::string> values_name = GetNames(add_values);
        std::unordered_map<std::string, std::string> line_data;
        for (auto it = tables[table_name].data[kFirstElement].begin();
             it != tables[table_name].data[kFirstElement].end(); ++it) {
            line_data[it->first] = "NULL";
        }
        for (int i = 0; i < columns_name.size(); ++i) {
            if (tables[table_name].data[kFirstElement][columns_name[i]].empty()) {
                throw std::invalid_argument("No column with this name: " + columns_name[i]);
            }
            if (tables[table_name].primary_key == columns_name[i]) {
                for (int j = 0; j < tables[table_name].data.size(); ++j) {
                    if (tables[table_name].data[j][columns_name[i]] == values_name[i]) {
                        throw std::invalid_argument(
                                "Primary key values must be uniq, database has already have this " + columns_name[i] +
                                ": " + values_name[i]);
                    }
                }
            }
            line_data[columns_name[i]] = values_name[i];
        }
        if (tables[table_name].empty) {
            tables[table_name].data[kFirstElement] = line_data;
            tables[table_name].empty = false;
        } else {
            tables[table_name].data.emplace_back(line_data);
        }
    } else {
        throw std::invalid_argument("Must be VALUES, but get this: " + insert_request[kValuePosition]);
    }
}

void MyDB::DELETEParse(const std::vector<std::string>& delete_request, const std::vector<std::string>& where_request) {
    if (delete_request[kFromIndex] == "FROM") {
        selected_table = delete_request[kDeleteNameTableIndex];
    } else {
        throw std::invalid_argument("Must be FROM after DELETE");
    }
    current_db[selected_table] = tables[selected_table];
    std::vector<int> lines = WHEREParse(where_request);
    for (int i = lines.size() - 1; i >= 0; --i) {
        tables[selected_table].data.erase(tables[selected_table].data.begin() + lines[i]);
    }
    current_db[selected_table] = tables[selected_table];
}

void MyDB::FindLines(const std::vector<int>& lines) {
    Table new_table;
    for (int i = 0; i < lines.size(); ++i) {
        new_table.data.emplace_back(current_db[selected_table].data[lines[i]]);
    }
    current_db[selected_table] = new_table;
}

void MyDB::JOINParse(const std::vector<std::string>& join_request) {
    size_t pos = join_request[kColumnsIndex].find(' ', 0);
    std::string right_table_name(join_request[kColumnsIndex].begin(), join_request[kColumnsIndex].begin() + pos);
    std::string right_add_columns = join_request[kColumnsIndex].substr(pos + 2,
                                                                       join_request[kColumnsIndex].size() - 2 - pos -
                                                                       1);
    std::vector<std::string> rights_columns_name;
    rights_columns_name = GetNames(right_add_columns);
    Table result;
    if (join_request[kOnIndex] != "ON") {
        throw std::invalid_argument("Must be ON after table name");
    }
    size_t pos_eq = join_request[kJoinConditionsIndex].find('=', 0);
    std::string left_column = join_request[kJoinConditionsIndex].substr(0, pos_eq - 1);
    std::string right_column = join_request[kJoinConditionsIndex].substr(pos_eq + 1, join_request.size() - pos_eq - 2);
    if (join_request[kJoinType].find("INNER", 0) != std::string::npos) {
        for (int i = 0; i < tables[selected_table].data.size(); ++i) {
            for (int j = 0; j < tables[right_table_name].data.size(); ++j) {
                if (tables[selected_table].data[i][left_column] != tables[right_table_name].data[j][right_column]) {
                    continue;
                }
                std::unordered_map<std::string, std::string> curr_res;
                for (auto iter = current_db[selected_table].data[i].begin();
                     iter != current_db[selected_table].data[i].end(); ++iter) {
                    curr_res.emplace(iter->first, iter->second);
                }
                for (int k = 0; k < rights_columns_name.size(); ++k) {
                    curr_res.emplace(rights_columns_name[k], tables[right_table_name].data[j][rights_columns_name[k]]);
                }
                result.data.emplace_back(curr_res);
            }
        }
    } else if (join_request[kJoinType].find("LEFT", 0) != std::string::npos) {
        for (int i = 0; i < tables[selected_table].data.size(); ++i) {
            bool has_NULL = true;
            for (int j = 0; j < tables[right_table_name].data.size(); ++j) {
                if (tables[selected_table].data[i][left_column] != tables[right_table_name].data[j][right_column]) {
                    continue;
                }
                has_NULL = false;
                std::unordered_map<std::string, std::string> curr_res;
                for (auto iter = current_db[selected_table].data[i].begin();
                     iter != current_db[selected_table].data[i].end(); ++iter) {
                    curr_res.emplace(iter->first, iter->second);
                }
                for (int k = 0; k < rights_columns_name.size(); ++k) {
                    curr_res.emplace(rights_columns_name[k], tables[right_table_name].data[j][rights_columns_name[k]]);
                }
                result.data.emplace_back(curr_res);
            }
            if (has_NULL) {
                std::unordered_map<std::string, std::string> curr_res;
                for (auto iter = current_db[selected_table].data[i].begin();
                     iter != current_db[selected_table].data[i].end(); ++iter) {
                    curr_res.emplace(iter->first, iter->second);
                }
                for (int k = 0; k < rights_columns_name.size(); ++k) {
                    curr_res.emplace(rights_columns_name[k], "NULL");
                }
                result.data.emplace_back(curr_res);
            }
        }
    } else if (join_request[kJoinType].find("RIGHT", 0) != std::string::npos) {
        for (int i = 0; i < tables[right_table_name].data.size(); ++i) {
            bool has_NULL = true;
            for (int j = 0; j < tables[selected_table].data.size(); ++j) {
                if (tables[selected_table].data[j][left_column] != tables[right_table_name].data[i][right_column]) {
                    continue;
                }
                has_NULL = false;
                std::unordered_map<std::string, std::string> curr_res;
                for (auto iter = current_db[selected_table].data[j].begin();
                     iter != current_db[selected_table].data[j].end(); ++iter) {
                    curr_res[iter->first] = iter->second;
                }
                for (int k = 0; k < rights_columns_name.size(); ++k) {
                    curr_res.emplace(rights_columns_name[k], tables[right_table_name].data[i][rights_columns_name[k]]);
                }
                result.data.emplace_back(curr_res);
            }
            if (has_NULL) {
                std::unordered_map<std::string, std::string> curr_res;
                for (int k = 0; k < rights_columns_name.size(); ++k) {
                    curr_res.emplace(rights_columns_name[k], tables[right_table_name].data[i][rights_columns_name[k]]);
                }
                for (auto iter = current_db[selected_table].data[0].begin();
                     iter != current_db[selected_table].data[0].end(); ++iter) {
                    curr_res[iter->first] = "NULL";
                }
                result.data.emplace_back(curr_res);
            }
        }
    } else {
        throw std::invalid_argument("You must set JOIN's type (INNER, LEFT or RIGHT)");
    }
    current_db[selected_table] = result;
}

void MyDB::UPDATEPArse(const std::vector<std::string>& update_request,
                       const std::vector<std::pair<std::string, std::string>>& changes,
                       const std::vector<std::string>& where_request) {
    selected_table = update_request[kUpdateTableName];
    std::vector<int> lines = WHEREParse(where_request);
    for (int i = 0; i < lines.size(); ++i) {
        for (int j = 0; j < changes.size(); ++j) {
            if (tables[selected_table].data[lines[i]][changes[j].first].empty()) {
                throw std::invalid_argument("No column with this name: " + changes[j].first);
            }
            tables[selected_table].data[lines[i]][changes[j].first] = changes[j].second;
        }
    }
}

std::vector<std::pair<std::string, std::string>> MyDB::SETParse(const std::vector<std::string>& set_request) {
    std::vector<std::pair<std::string, std::string>> result;
    std::string column_name = set_request[kColumnsIndex];
    std::string value;
    for (int i = 2; i < set_request.size(); ++i) {
        if (set_request[i] == "=") {
            if (set_request[i + 1].find(',', 0) != std::string::npos) {
                value = set_request[i + 1].substr(0, set_request[i + 1].find(',', 0));
                result.emplace_back(column_name, value);
                column_name = set_request[i + 1].substr(set_request[i + 1].find(',', 0) + 2,
                                                        set_request[i + 1].size() - set_request[i + 1].find(',', 0) -
                                                        1);
                i += 1;
            } else {
                value = set_request[i + 1];
                result.emplace_back(column_name, value);
                i += 1;
            }

        } else {
            throw std::invalid_argument("Wrong argument, must be =");
        }
    }

    return result;
}


bool operator==(const Table& lhs, const Table& rhs) {
    return lhs.data == rhs.data;
}

bool operator!=(const Table& lhs, const Table& rhs) {
    return lhs.data != rhs.data;
}

std::ostream& operator<<(std::ostream& os, MyDB& database) {
    os << database.PrintDB();

    return os;
}