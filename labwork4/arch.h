#include <cstring>
#include <iostream>
#include <string>
#include <vector>

struct Arch {
    std::vector<std::string> files;
    std::string path;
    std::string real_bin_file;

    Arch(std::vector<std::string>& vec, std::string p, std::string name) {
        if (vec.size() > 0) files.assign(vec.begin(), vec.end());
        path = p;
        real_bin_file = path + name + ".haf";
    }

    void FileInfo();

    void Compress();

    void DeCompress(std::string binary, std::vector<std::string> file, std::string name_file);

    std::vector<std::string> PrintFiles(std::string binary, std::string path);

    static std::string get_file_name(std::string fn) {
        return fn.substr(fn.find_last_of("/") + 1, fn.size());
    }
};

