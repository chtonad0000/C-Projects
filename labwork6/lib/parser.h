#pragma once

#include <filesystem>
#include <istream>
#include <iostream>
#include <vector>

namespace omfl {

    class OMFLParser {
    public:
        bool validation = true;

        struct Keys {
            std::string name;
            std::string value;
            std::vector<Keys> values;

            Keys operator[](size_t ind) {
                if (ind >= values.size()) {
                    OMFLParser::Keys extra;
                    extra.value = "";
                    return extra;
                }
                return values[ind];
            };

            bool IsInt();

            int AsInt();

            int AsIntOrDefault(int x);

            bool IsFloat();

            float AsFloat();

            float AsFloatOrDefault(float x);

            bool IsString();

            std::string AsString();

            std::string AsStringOrDefault(const std::string& str);

            bool IsArray();

            bool IsBool();

            bool AsBool();

            OMFLParser::Keys Get(const std::string& name);
        };

        inline static std::vector<Keys> keys;

        Keys Get(const std::string& name) const;

        bool valid() const;
    };

    OMFLParser parse(const std::filesystem::path& path);

    OMFLParser parse(const std::string& str);


}// namespace