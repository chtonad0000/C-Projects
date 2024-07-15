#include "parser_ID3v2.h"
#include <iostream>

int main(int argc, char* argv[]) {
    for (int i = 0; i < argc; ++i) {
        if ((std::string(argv[i]) == "-f") || (std::string(argv[i]) == "--file")) {
            if (i + 1 < argc) {
                FILE* file = fopen(argv[i + 1], "rb");
                if (file == nullptr) {
                    std::cout << "No file";
                } else {
                    ID3v2Header header;
                    int tag_size = header.ReadHeader(file);
                    if (tag_size == -1) {
                        std::cout << "There is no ID3";
                        fclose(file);
                    } else {
                        std::vector<ID3v2Frame*> frames = ParseFrames(file, tag_size);
                        fclose(file);
                        for (int i = 0; i < frames.size(); ++i) {
                            frames[i]->PrintFrame();
                        }
                        for (int i = 0; i < frames.size(); ++i) {
                            delete frames[i];
                        }
                    }
                }
            }
        }
    }
}