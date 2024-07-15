#pragma once

#include <iostream>
#include <vector>

namespace ArgumentParser {

    class ArgParser {
    public:
        inline static std::string parser_name;
        std::vector<std::string> arguments;
        ArgParser(const std::string& name);
        struct AddString {
            std::string name_str1 = "\0";
            std::string name_str2 = "\0";
            char name_str_char = '\0';
            std::string value = "\0";
            std::string* ptr = nullptr;
            size_t multi_value = 0;

            void MultiValue(size_t count);

            void Default(const std::string& str);

            void StoreValue(std::string& str);
        };

        inline static std::vector<AddString> params_str;

        AddString AddStringArgument(const std::string& str);

        AddString AddStringArgument(char p, const std::string& str);

        AddString AddStringArgument(char p, const std::string& str1, const std::string& str2);

        bool Parse(std::vector<std::string> argv);

        std::string GetStringValue(const std::string& param_name);

        struct AddInt {
            std::string name_str1 = "\0";
            std::string name_str2 = "\0";
            char name_str_char = '\0';
            std::vector<int> values;
            int multi_value = 0;
            std::vector<int>* store_values = nullptr;
            std::vector<int>* store_value = nullptr;

            void StoreValue(std::vector<int>& int_values);

            AddInt Positional();

            AddInt MultiValue(size_t x);

            AddInt MultiValue();

            void StoreValues(std::vector<int>& int_values);

            bool position = false;
        };

        inline static std::vector<AddInt> params_int;

        AddInt AddIntArgument(const std::string& str);

        AddInt AddIntArgument(char p, const std::string& str);

        AddInt AddIntArgument(const std::string& str1, const std::string& str2);

        int GetIntValue(const std::string& str);

        int GetIntValue(const std::string& str, int position);

        struct flag {
            std::string name_str1;
            std::string name_str2;
            char name_str_char;
            bool exist = false;
            bool* ptr = nullptr;

            void Default(bool fl);

            void StoreValue(bool& fl);
        };

        inline static std::vector<flag> flags;

        flag AddFlag(char chr, const std::string& str1);

        flag AddFlag(char chr, const std::string& str1, const std::string& str2);

        bool GetFlag(const std::string& str);

        struct help {
            std::string name_str;
            char name_char;
            std::string description;
            bool exist = false;

        } help;

        void AddHelp(char chr, const std::string& str1, const std::string& str2);

        bool Help();

        std::string HelpDescription();
    };

} // namespace ArgumentParser