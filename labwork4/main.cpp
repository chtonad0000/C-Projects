#include "arch.h"
#include <iostream>

int main(int argc, char* argv[]) {
    std::vector<std::string> files;
    std::string name = "";
    std::string path = "C:/Users/d_chu/CLionProjects/labwork-4-chtonad0000/archiv/";
    int i = 1;
    bool flag = false;
    while (i < argc) {
        std::string command = argv[i];
        if ((command == "-c") || (command == "--create")) {
            int j = 0;
            flag = true;
            std::string argument = argv[i + 1];
            while (argument[j] != '=') {
                j++;

            }
            j++;

            for (int k = j; k < argument.size(); k++) {
                name += argument[k];
            }
            i += 2;
        } else if (command == "-l" || command == "--list") {
            Arch ham(files, path, name);
            std::vector<std::string> file_names = ham.PrintFiles(path + argv[i + 2] + ".haf", path);
            for (int i = 0; i < file_names.size(); i++) {
                std::cout << file_names[i] << ' ';
            }
            std::cout << std::endl;
            i += 3;
        } else if (command == "-x" || command == "--extract") {
            Arch ham(files, path, name);

            if (i + 3 < argc) {
                int j = i + 3;
                while (j < argc) {
                    files.emplace_back(std::string(argv[j]));
                    j++;
                }
            }
            std::cout << std::string(argv[i + 2]);
            files.clear();
            ham.DeCompress(path, files, std::string(argv[i + 2]) + ".haf");
            remove((path+std::string(argv[i+2])+".haf").c_str());
            break;
        } else if (command == "-a" || command == "--append") {
            Arch ham1(files, path, name);
            int j = i;
            while (std::string(argv[j]) != "-f") {
                j++;
            }
            std::vector<std::string> file_names = ham1.PrintFiles(path + std::string(argv[j + 1]) + ".haf", path);
            i++;
            while (std::string(argv[i]) != "-f") {
                file_names.emplace_back(std::string(argv[i]));
                i++;

            }
            for (int i = 0; i < file_names.size(); i++) {
                std::cout << file_names[i] << ' ';
            }
            files.clear();
            ham1.DeCompress(path, files, std::string(argv[j + 1]) + ".haf");
            Arch ham2(file_names, path, std::string(argv[j + 1]));
            ham2.Compress();
            for (int i = 0; i < file_names.size(); i++) {
                remove(file_names[i].c_str());
            }
            i += 3;
        } else if (command == "-d" || command == "--delete") {
            Arch ham1(files, path, name);
            int j = i;

            while (std::string(argv[j]) != "-f") {
                j++;
            }
            std::vector<std::string> file_names = ham1.PrintFiles(path + argv[j + 1] + ".haf", path);
            i++;

            while (std::string(argv[i]) != "-f") {
                files.emplace_back(argv[i]);
                i++;
            }

            int m = 0;
            while (m < file_names.size()) {
                std::string name_file;
                for (int n = file_names[m].size() - 1; n >= 0; n--) {
                    if (file_names[m][n] == '/') {
                        break;
                    }
                    name_file += file_names[m][n];

                }
                std::string new_file_name;
                for (int n = name_file.size() - 1; n >= 0; n--) {
                    new_file_name += name_file[n];
                }
                for (int n = 0; n < files.size(); n++) {
                    if (files[n] == new_file_name) {
                        file_names.erase(file_names.begin() + m);
                        m--;
                        break;
                    }
                }
                m++;
            }
            Arch ham2(file_names, path, argv[j + 1]);
            ham2.Compress();
            i += 2;
        } else if (command == "-A" || command == "--concatenate") {
            i++;
            while (std::string(argv[i]) != "-f") {
                if (std::string(argv[i]) != "-f") {
                    files.push_back(argv[i]);
                }
                i++;
            }
            Arch new_arch(files, path, argv[i + 1]);
            std::vector<std::string> file_names_one;
            std::vector<std::string> file_names_two;
            file_names_one = new_arch.PrintFiles(path + files[0] + ".haf", path);
            file_names_two = new_arch.PrintFiles(path + files[1] + ".haf", path);
            std::vector<std::string> common_files;
            common_files.reserve(file_names_two.size() + file_names_one.size());
            common_files.insert(common_files.end(), file_names_one.begin(), file_names_one.end());
            common_files.insert(common_files.end(), file_names_two.begin(), file_names_two.end());
            file_names_one.clear();
            file_names_two.clear();
            new_arch.DeCompress(path, file_names_one, files[0] + ".haf");
            new_arch.DeCompress(path, file_names_two, files[1] + ".haf");
            Arch a_arch(common_files, path, argv[i + 1]);
            a_arch.Compress();
            for (int i = 0; i < common_files.size(); i++) {
                remove(common_files[i].c_str());
            }
            i += 3;
        } else {
            files.emplace_back(std::string(argv[i]));
            i++;
        }
    }

    Arch zip(files, path, name);
    if (flag) {
        for (int i = 0; i < files.size(); i++) {
            std::cout << files[i] << ' ';
        }
        zip.Compress();
    }
}