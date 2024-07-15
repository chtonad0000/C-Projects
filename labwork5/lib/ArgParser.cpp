#include "ArgParser.h"


using namespace ArgumentParser;

bool ArgParser::Parse(std::vector<std::string> argv) {
    arguments.assign(argv.begin(), argv.end());
    for (int i = 1; i < arguments.size(); i++) {
        if (arguments[i][0] == '-') {
            int j = i;
            while (arguments[i][j] != '=') {
                if (j == arguments[i].size()) {
                    break;
                }
                j++;
            }
            std::string command;
            std::vector<char> flag_names;
            if (arguments[i][1] == '-') {
                command = arguments[i].substr(2, j - 2);
            } else {
                command = arguments[i].substr(1, j - 1);
                for (int n = 0; n < command.size(); n++) {
                    flag_names.push_back(command[n]);
                }
            }

            std::string result;
            if (j != arguments[i].size()) {
                result = arguments[i].substr(j + 1, arguments[i].size() - j - 1);
            }
            if (command == help.name_str || command == std::string(1, help.name_char)) {
                help.exist = true;

                return true;
            }
            for (int k = 0; k < params_str.size(); k++) {
                if (params_str[k].name_str1 == command || params_str[k].name_str2 == command || std::string(1, params_str[k].name_str_char) == command) {
                    params_str[k].value = result;
                    if (params_str[k].ptr != nullptr) {
                        *params_str[k].ptr = result;
                    }
                    break;
                }
            }
            for (int k = 0; k < params_int.size(); k++) {
                if (params_int[k].name_str1 == command || params_int[k].name_str2 == command || std::string(1, params_int[k].name_str_char) == command) {
                    params_int[k].values.push_back(std::stoi(result));
                }
            }
            for (int k = 0; k < flags.size(); k++) {
                if (flag_names.empty()) {
                    if (flags[k].name_str1 == command || flags[k].name_str2 == command || std::string(1, flags[k].name_str_char) == command) {
                        flags[k].exist = true;
                        if (flags[k].ptr != nullptr) {
                            *flags[k].ptr = true;
                        }
                    }
                } else {
                    for (int m = 0; m < flag_names.size(); m++) {
                        if (flags[k].name_str_char == flag_names[m]) {
                            flags[k].exist = true;
                            if (flags[k].ptr != nullptr) {
                                *flags[k].ptr = true;
                            }
                        }
                    }
                }
            }
        } else {
            params_int.back().values.push_back(stoi(arguments[i]));
        }
    }
    for (int i = 0; i < params_int.size(); i++) {
        if (!params_int[i].values.empty()) {
            if (params_int[i].position) {
                *params_int[i].store_value = params_int[i].values;
            }
            if (params_int[i].store_values != nullptr) {
                std::cout << params_int[i].store_values << ' ';
                *params_int[i].store_values = params_int[i].values;

            }
        }
    }
    for (int i = 0; i < params_int.size(); i++) {
        if (params_int[i].values.empty() || params_int[i].values.size() < params_int[i].multi_value) {
            return false;
        }
    }
    for (int i = 0; i < params_str.size(); i++) {
        if (params_str[i].value == "\0") {
            return false;
        }
    }

    return true;
}

ArgParser::AddString ArgParser::AddStringArgument(const std::string& str) {
    AddString* p1 = new AddString;
    p1->name_str1 = str;
    params_str.push_back(*p1);

    return params_str.back();
}

ArgParser::AddString ArgParser::AddStringArgument(char p, const std::string& str) {
    AddString* p1 = new AddString;
    p1->name_str1 = str;
    p1->name_str_char = p;
    params_str.push_back(*p1);

    return params_str.back();
}

ArgParser::AddString ArgParser::AddStringArgument(char p, const std::string& str1, const std::string& str2) {
    AddString* p1 = new AddString;
    p1->name_str1 = str1;
    p1->name_str2 = str2;
    p1->name_str_char = p;
    params_str.push_back(*p1);

    return params_str.back();
}

void ArgParser::AddString::Default(const std::string& str) {
    params_str.back().value = str;

}

void ArgParser::AddString::MultiValue(size_t count) {
    params_str.back().multi_value = count;
}

void ArgParser::AddString::StoreValue(std::string& str) {
    params_str.back().ptr = &str;
}

ArgParser::flag ArgParser::AddFlag(char chr, const std::string& str1) {
    flag* f1 = new flag;
    f1->name_str1 = str1;
    f1->name_str_char = chr;
    flags.push_back(*f1);

    return flags.back();
}

ArgParser::flag ArgParser::AddFlag(char chr, const std::string& str1, const std::string& str2) {
    flag* f1 = new flag;
    f1->name_str1 = str1;
    f1->name_str2 = str2;
    f1->name_str_char = chr;
    flags.push_back(*f1);

    return flags.back();
}

bool ArgParser::GetFlag(const std::string& str) {
    for (int i = 0; i < flags.size(); i++) {
        if (flags[i].name_str1 == str || flags[i].name_str2 == str) {
            return flags[i].exist;
        }
    }
}

void ArgParser::flag::Default(bool fl) {
    flags.back().exist = fl;
}

void ArgParser::flag::StoreValue(bool& fl) {
    flags.back().ptr = &fl;
}

ArgParser::AddInt ArgParser::AddIntArgument(const std::string& str) {
    AddInt* int_struct = new AddInt;
    int_struct->name_str1 = str;
    params_int.push_back(*int_struct);

    return params_int.back();
}

ArgParser::AddInt ArgParser::AddIntArgument(char p, const std::string& str) {
    AddInt* int_struct = new AddInt;
    int_struct->name_str1 = str;
    int_struct->name_str_char = p;
    params_int.push_back(*int_struct);

    return params_int.back();
}

ArgParser::AddInt ArgParser::AddIntArgument(const std::string& str1, const std::string& str2) {
    AddInt* int_struct = new AddInt;
    int_struct->name_str1 = str1;
    int_struct->name_str2 = str2;
    params_int.push_back(*int_struct);

    return params_int.back();
}

ArgParser::ArgParser(const std::string& name) {
    parser_name = name;
}

std::string ArgParser::GetStringValue(const std::string& param_name) {
    size_t size = params_str.size();
    std::cout << params_str[0].value;
    for (int i = 0; i < size; i++) {
        if (params_str[i].name_str1 == param_name || params_str[i].name_str2 == param_name) {
            return params_str[i].value;
        }
    }
}

void ArgParser::AddInt::StoreValue(std::vector<int>& int_values) {
    params_int.back().store_value = &int_values;
}

ArgParser::AddInt ArgParser::AddInt::Positional() {
    params_int.back().position = true;

    return params_int.back();
}

ArgParser::AddInt ArgParser::AddInt::MultiValue(size_t x) {
    params_int.back().multi_value = x;

    return params_int.back();
}

ArgParser::AddInt ArgParser::AddInt::MultiValue() {
    params_int.back().multi_value = 0;

    return params_int.back();
}

void ArgParser::AddInt::StoreValues(std::vector<int>& int_values) {
    params_int.back().store_values = &int_values;
}

int ArgParser::GetIntValue(const std::string& str) {
    for (int i = 0; i < params_int.size(); i++) {
        if (params_int[i].name_str1 == str) {
            return params_int[i].values[0];
        }
    }
}

int ArgParser::GetIntValue(const std::string& str, int position) {
    for (int i = 0; i < params_int.size(); i++) {
        if (params_int[i].name_str1 == str) {
            return params_int[i].values[position];
        }
    }
}

void ArgParser::AddHelp(char chr, const std::string& str1, const std::string& str2) {
    help.name_str = str1;
    help.name_char = chr;
    help.description = str2;
}

bool ArgParser::Help() {
    return help.exist;
}

std::string ArgParser::HelpDescription() {
    std::string description_return;
    description_return += parser_name + "\n" + help.description + "\n" + "\n";
    for (int i = 0; i < params_str.size(); i++) {
        description_return += "-" + std::string(1, params_str.back().name_str_char) + ",  --" + params_str.back().name_str1 + "=<string>" + ",  " + params_str.back().name_str2;
        std::string min_arguments = std::to_string(params_str.back().multi_value);
        if (min_arguments != "0") {
            description_return += " [repeated, min args = " + min_arguments + "]";
        }
        description_return += "\n";
    }
    for (int i = 0; i < flags.size(); i++) {
        description_return += "-" + std::string(1, flags[i].name_str_char) + ",  --" + flags[i].name_str1 + ",  " + flags[i].name_str2;
        if (flags[i].exist) {
            description_return += " [default = " + std::string(flags[i].exist ? "true" : "false") + "]";
        }
        description_return += "\n";
    }
    for (int i = 0; i < params_int.size(); i++) {
        if (params_int[i].name_str_char == '\0') {
            description_return += "  ";
        } else {
            description_return += "-" + std::string(1, params_int[i].name_str_char) + ",";
        }
        description_return += "  --" + params_int[i].name_str1 + "=<int>,  " + params_int[i].name_str2;
        description_return += "\n";


    }
    description_return += "\n";
    description_return += "-" + std::string(1, help.name_char) + ", --" + help.name_str + " Display this help and exit\n";

    return description_return;
}
