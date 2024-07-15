#include "parser.h"
#include <cstring>
#include <fstream>

using namespace omfl;


bool CorrectName(std::string name) {
    if (name.empty()) {
        return false;
    }
    for (int ptr_line = 0; ptr_line < name.size(); ptr_line++) {
        if (name[ptr_line] != '-' && !std::isalpha(name[ptr_line]) && name[ptr_line] != '_' &&
            !std::isdigit(name[ptr_line])) {
            return false;
        }
    }
    return true;
}

bool IsInt(std::string value) {
    int start = 0;
    if (value.empty()) return false;
    if (value[0] == '-' || value[0] == '+') {
        start = 1;
        if (value.size() == 1) {
            return false;
        }
    }
    for (int i = start; i < value.size(); i++) {
        if (!std::isdigit(value[i])) {
            return false;
        }
    }
    return true;
}

bool IsFloat(std::string value) {
    int start = 0;
    int count_point = 0;
    if (value.empty()) {
        return false;
    }
    if (value[0] == '+' || value[0] == '-') {
        start = 1;
        if (value.size() == 1) {
            return false;
        }
    }
    if (value.size() < 3 || (value[0] == '.' && start == 0) || (value[1] == '.' && start == 1) ||
        value[value.size() - 1] == '.') {
        return false;
    }
    for (int i = start; i < value.size(); i++) {
        if (!std::isdigit(value[i])) {
            if (value[i] != '.') {
                return false;
            } else {
                count_point++;
            }
        }
    }
    if (count_point != 1) {
        return false;
    }
    return true;
}

bool IsString(std::string value) {
    if (value[0] == '"' && value[value.size() - 1] == '"') {
        for (int i = 1; i < value.size() - 1; i++) {
            if (value[i] == '"') {
                return false;
            }
        }
        return true;
    } else {
        return false;
    }
}

bool IsBool(std::string value) {
    if (value == "true" || value == "false") {
        return true;
    }
    return false;
}

bool IsArray(const std::string& value) {
    if (value.size() < 2 || value[0] != '[' || value[value.size() - 1] != ']') {
        return false;
    }
    std::string str = value.substr(1, value.size() - 2);
    std::vector<std::string> array_elements;
    int i = 0;
    while (std::isblank(str[0])) {
        str.erase(0, 1);
    }
    while (std::isblank(str[str.size() - 1])) {
        str.erase(str.size() - 1, 1);
    }
    std::string elem = "";
    while (i < str.size()) {
        while (i < str.size() && str[i] != ',') {
            elem += str[i];
            if (str[i] == '[') {
                int count_brackets = 1;
                i++;
                while (i < str.size() && count_brackets != 0) {
                    if (str[i] == '[') {
                        count_brackets++;
                    } else if (str[i] == ']') {
                        count_brackets--;
                    }
                    elem += str[i];
                    i++;
                }
            } else {
                i++;
            }
        }
        while (std::isblank(elem[0])) {
            elem.erase(0, 1);
        }
        while (std::isblank(elem[elem.size() - 1])) {
            elem.erase(elem.size() - 1, 1);
        }
        array_elements.push_back(elem);
        elem = "";
        i++;
    }
    for (int i = 0; i < array_elements.size(); i++) {
        if (!IsString(array_elements[i]) && !IsInt(array_elements[i]) && !IsFloat(array_elements[i]) &&
            !IsBool(array_elements[i])) {
            if (!IsArray(array_elements[i])) {
                return false;
            }
        }
    }
    return true;
}

bool CorrectValue(std::string value) {
    if (value.empty()) {
        return false;
    }
    if (!IsString(value) && !IsInt(value) && !IsBool(value) && !IsFloat(value) && !IsArray(value)) {
        return false;
    }
    return true;
}

bool OMFLParser::Keys::IsArray() {
    return ::IsArray(value);
}

std::vector<OMFLParser::Keys> AsArray(OMFLParser::Keys key) {
    std::string str = key.value.substr(1, key.value.size() - 2);
    std::vector<OMFLParser::Keys> elements;
    int i = 0;
    while (std::isblank(str[0])) {
        str.erase(0, 1);
    }
    while (std::isblank(str[str.size() - 1])) {
        str.erase(str.size() - 1, 1);
    }
    std::string elem = "";
    while (i < str.size()) {
        while (i < str.size() && str[i] != ',') {
            elem += str[i];
            if (str[i] == '[') {
                int count_brackets = 1;
                i++;
                while (i < str.size() && count_brackets != 0) {
                    if (str[i] == '[') {
                        count_brackets++;
                    } else if (str[i] == ']') {
                        count_brackets--;
                    }
                    elem += str[i];
                    i++;
                }
            } else {
                i++;
            }
        }
        while (std::isblank(elem[0])) {
            elem.erase(0, 1);
        }
        while (std::isblank(elem[elem.size() - 1])) {
            elem.erase(elem.size() - 1, 1);
        }
        OMFLParser::Keys element;
        element.value = elem;
        if (element.IsArray()) {
            element.values = AsArray(element);
        }
        elements.push_back(element);
        elem = "";
        i++;
    }
    return elements;
}

OMFLParser::Keys OMFLParser::Get(const std::string& name) const {
    for (int i = 0; i < keys.size(); i++) {
        if (keys[i].name == name) {
            return keys[i];
        }
    }
    OMFLParser::Keys new_key;
    new_key.name = name + '.';
    return new_key;
}

OMFLParser::Keys OMFLParser::Keys::Get(const std::string& name) {
    this->name += name + '.';
    for (int i = 0; i < keys.size(); i++) {
        if (this->name.substr(0, this->name.size() - 1) == keys[i].name) {
            return keys[i];
        }
    }

    return *this;
}

bool OMFLParser::valid() const {
    return validation;
}

bool OMFLParser::Keys::IsInt() {
    return ::IsInt(value);
}

int OMFLParser::Keys::AsInt() {
    return std::stoi(value);
}

int OMFLParser::Keys::AsIntOrDefault(int x) {
    if (IsInt()) {
        return std::stoi(value);
    } else {
        return x;
    }
}

bool OMFLParser::Keys::IsFloat() {
    return ::IsFloat(value);
}

float OMFLParser::Keys::AsFloat() {
    return std::stof(value);
}

float OMFLParser::Keys::AsFloatOrDefault(float x) {
    if (IsFloat()) {
        return std::stof(value);
    } else {
        return x;
    }
}

bool OMFLParser::Keys::IsString() {
    return ::IsString(value);
}

std::string OMFLParser::Keys::AsString() {
    return value.substr(1, value.size() - 2);
}

std::string OMFLParser::Keys::AsStringOrDefault(const std::string& str) {
    if (IsString()) {
        return value.substr(1, value.size() - 2);
    } else {
        return str;
    }
}

bool OMFLParser::Keys::IsBool() {
    return ::IsBool(value);
}

bool OMFLParser::Keys::AsBool() {
    if (value == "true") {
        return true;
    }
    if (value == "false") {
        return false;
    }
}

OMFLParser omfl::parse(const std::string& str) {
    OMFLParser data;
    const char* divide = "\n";
    char* line = std::strtok(const_cast<char*>(str.c_str()), divide);
    std::string current_section = "";
    while (line != nullptr) {
        std::string line_str = std::string(line);
        line = std::strtok(nullptr, divide);
        size_t ptr = 0;
        size_t count_spaces = 1;
        while (line_str[ptr] == ' ') {
            line_str = line_str.substr(1, line_str.size() - count_spaces);
        }
        if (line_str[0] != '#') {
            if (line_str[0] != '[') {
                std::string key = "";
                std::string value = "";
                size_t ptr_line = 0;
                while (line_str[ptr_line] != ' ' && line_str[ptr_line] != '=' && ptr_line != line_str.size()) {
                    key += line_str[ptr_line];
                    ptr_line++;

                }

                if (!CorrectName(key)) {
                    data.validation = false;
                }
                while (line_str[ptr_line] == ' ' && ptr_line != line_str.size()) {
                    ptr_line++;
                }
                ptr_line++;
                while (line_str[ptr_line] == ' ' && ptr_line != line_str.size()) {
                    ptr_line++;
                }
                if (ptr_line == line_str.size()) { // есть ли значение у key
                    data.validation = false;
                    break;
                }
                while (line_str[ptr_line] != '#' && ptr_line != line_str.size()) {
                    value += line_str[ptr_line];
                    ptr_line++;
                }
                ptr_line--;
                while (line_str[ptr_line] == ' ') {
                    value = value.substr(0, value.size() - 1);
                    ptr_line--;
                }
                OMFLParser::Keys new_key;
                if (!CorrectValue(value)) {
                    data.validation = false;
                }
                new_key.value = value;
                if (::IsArray(value)) {
                    new_key.values = AsArray(new_key);
                }
                new_key.name = current_section + key;
                for (int i = 0; i < data.keys.size(); i++) {
                    if (new_key.name == data.keys[i].name) {
                        data.validation = false;
                    }
                }
                data.keys.push_back(new_key);
            } else {
                std::string section;
                size_t start = 1;
                while (line_str[start] != ']') {
                    section += line_str[start];
                    start++;
                }
                if (section.empty() || section[0] == '.' || section[section.size() - 1] == '.') {
                    data.validation = false;
                }
                current_section = section + '.';
            }
        }
    }
    return data;
}

OMFLParser omfl::parse(const std::filesystem::path& path) {
    std::ifstream f(path);
    std::string data;
    char byte;
    while (f.get(byte)) {
        data += byte;
    }
    return parse(data);
}

