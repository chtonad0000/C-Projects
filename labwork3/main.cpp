#include "bitmapfile.cpp"
#include <iostream>
#include <filesystem>

const int kTabsCount = 2;

int main(int argc, char *argv[]) {
    std::string file_name;
    int16_t width;
    int16_t height;
    int image_number = 0;
    std::string path;
    std::string ex_path;
    int x;
    int y;
    int piles;
    int iterations = -1;
    int freq = 0;
    for (int i = 1; i < argc - 1; i++) {
        std::string argument = argv[i];
        std::string argument_next = argv[i + 1];
        if (argument == "-l" || argument == "--length") {
            width = stoi(argument_next);
        }
        if (argument == "-w" || argument == "--width") {
            height = stoi(argument_next);
        }
        if (argument == "-i" || argument == "--input") {
            file_name = argument_next;
        }
        if (argument == "-m" || argument == "--max_iter") {
            iterations = stoi(argument_next);
        }
        if (argument == "-f" || argument == "--freq") {
            freq = stoi(argument_next);
        }
        if (argument == "-o" || argument == "--output") {
            path = argument_next;
        }
    }
    std::filesystem::remove_all(path);
    std::filesystem::create_directory(path);
    std::ifstream file_txt(file_name);
    Image image(width, height);
    for (int j = 0; j < height; j++) {
        for (int i = 0; i < width; i++) {
            image.SetColor(Color(1, 1, 1), i, j);
        }
    }
    std::vector<int> sand(height * width);
    sand.assign(width*height, 0);
    std::string line;
    while (std::getline(file_txt, line)) {
        int indexes[kTabsCount];
        int k = 0;
        for (int i = 0; i < line.length(); i++) {
            if (line[i] == '\t') {
                indexes[k] = i;
                k++;
            }
        }
        x = stoi(line.substr(0, indexes[0]));
        y = stoi(line.substr(indexes[0] + 1, indexes[1] - indexes[0] - 1));
        piles = stoi(line.substr(indexes[1] + 1, line.length() - 1 - indexes[1]));
        sand[x * width + y] = piles;
        if (piles == 1) {
            image.SetColor(Color(0, 128, 0), y, x);
        }
        if (piles == 2) {
            image.SetColor(Color(180, 0, 180), y, x);
        }
        if (piles == 3) {
            image.SetColor(Color(2, 1, 0), y, x);
        }
        if (piles > 3) {
            image.SetColor(Color(255, 255, 255), y, x);
        }
    }
    bool is_stable = false;
    int count_iter= 0;
    std::vector<int> new_sand(height * width);
    while (!is_stable && (count_iter < iterations || iterations == -1)) {
        is_stable = true;
        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                new_sand[i * width + j] = sand[i * width + j];
            }
        }
        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                if (sand[i * width + j] >= 4) {
                    new_sand[i * width + j] -= 4;
                    if (i >= 1) {
                        new_sand[(i - 1) * width + j]++;
                    }
                    if (j >= 1) {
                        new_sand[i * width + j - 1]++;
                    }
                    if (i + 1 < width) {
                        new_sand[(i + 1) * width + j]++;
                    }
                    if (j + 1 < height) {
                        new_sand[i * width + j + 1]++;
                    }
                }
            }
        }
        for (int j = 0; j < width; j++) {
            for (int i = 0; i < height; i++) {
                sand[i * width + j] = new_sand[i * width + j];
                if (sand[i * width + j] == 0) {
                    image.SetColor(Color(1, 1, 1), j, i);
                }
                if (sand[i * width + j] == 1) {
                    image.SetColor(Color(0, 128, 0), j, i);
                }
                if (sand[i * width + j] == 2) {
                    image.SetColor(Color(180, 0, 180), j, i);
                }
                if (sand[i * width + j] == 3) {
                    image.SetColor(Color(2, 1, 0), j, i);
                }
                if (sand[i * width + j] > 3) {
                    image.SetColor(Color(255, 255, 255), j, i);
                }
                if (sand[i * width + j] > 3) {
                    is_stable = false;
                }
            }
        }
        count_iter++;
        if (freq != 0) {
            if (count_iter % freq == 0) {
                ex_path = path + std::to_string(image_number) + ".bmp";
                image_number++;
                image.Export(ex_path);
            }
        }
    }
    ex_path = path + std::to_string(image_number) + ".bmp";
    image.Export(ex_path);
    std::cout << "All files are were created";
}