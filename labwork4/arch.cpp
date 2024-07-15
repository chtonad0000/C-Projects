#include "arch.h"
#include <bitset>
#include <cstring>
#include <cmath>

std::string HammingCode(std::vector<uint64_t> bits) {
    std::string hamming_str;
    std::vector<uint64_t> hamming(bits.size() + floor(log2(bits.size())));
    int r = floor(log2(bits.size()));
    for (int i = 0; i < r; i++) {
        hamming[pow(2, i) - 1] = 2;
    }
    int j = 0;
    for (int i = 0; i < r + bits.size(); i++) {
        if (hamming[i] != 2) {
            hamming[i] = bits[j];
            j++;
        }
    }
    for (int i = 0; i < hamming.size(); i++) {
        if (hamming[i] == 2) {
            int x = log2(i + 1);
            int count = 0;
            for (int j = i + 2; j <= hamming.size(); j++) {
                if (j & (1 << x)) {
                    if (hamming[j] == 1) {
                        count++;
                    }
                }
            }
            if (count % 2 == 0) {
                hamming_str += "0";
            } else {
                hamming_str += "1";
            }
        }
    }

    return hamming_str;
}

std::vector<uint64_t> HammingFixed(std::vector<uint64_t> bits, std::string str1, std::string str2) {
    int a[str1.size()];
    std::memset(a, 0, str1.size());
    for (int i = 0; i < str1.size(); i++) {
        if (str1[i] != str2[i]) {
            a[i] = 1;
        }
    }
    uint64_t sum = 0;
    for (int i = 0; i < str1.size(); i++) {
        sum += a[i] * pow(2, i);
    }
    std::vector<uint8_t> hamming(bits.size() + floor(log2(bits.size())));
    int r = floor(log2(bits.size()));
    for (int i = 0; i < r; i++) {
        hamming[pow(2, i) - 1] = 2;
    }
    int j = 0;
    for (int i = 0; i < r + bits.size(); i++) {
        if (hamming[i] != 2) {
            hamming[i] = bits[j];
            j++;
        }
    }
    if (hamming[sum - 1] == 1) {
        hamming[sum - 1] = 0;
    } else {
        hamming[sum - 1] = 1;
    }
    std::vector<uint64_t> BitsFixed;
    for (int i = 0; i < hamming.size(); i++) {
        if (hamming[i] != 2) {
            BitsFixed.push_back(hamming[i]);
        }
    }

    return BitsFixed;
}

int CountDigits(size_t x) {
    if (x == 0) {
        return 1;
    } else {
        int res = 0;
        int tmp = x;
        while (tmp > 0) {
            res++;
            tmp = tmp / 10;
        }
        return res;
    }
}

void Arch::FileInfo() {
    std::basic_string<char> s_info = "";
    remove((this->path + "info.txt").c_str());
    FILE* info = fopen((this->path + "info.txt").c_str(), "a+");
    int bytes_size = 0;
    for (int i = 0; i < files.size(); i++) {
        FILE* f = fopen((files[i]).c_str(), "rb");
        if (!f) {
            break;
        }
        fseek(f, 0, SEEK_END);
        int size = ftell(f);
        std::string name = Arch::get_file_name(files[i]);
        char* m_size = new char[CountDigits(size)];
        itoa(size, m_size, 10);
        fclose(f);
        bytes_size += CountDigits(size);
        bytes_size += strlen(name.c_str());
        s_info.append(m_size);
        s_info.append("||");
        s_info.append(name);
        s_info.append("||");

    }
    bytes_size = s_info.size() + 2;
    char* b_buff = new char[CountDigits(bytes_size)];
    itoa(bytes_size, b_buff, 10);
    if (CountDigits(bytes_size) < 5) {
        fputs(std::string(5 - CountDigits(bytes_size), '0').c_str(), info);
    }
    fputs(b_buff, info);
    fputs("||", info);
    fputs(s_info.c_str(), info);
    fclose(info);
}

void Arch::Compress() {
    std::vector<uint64_t> bits;
    char byte[1];
    FileInfo();
    FILE* f;
    FILE* main = fopen((this->real_bin_file).c_str(), "wb");
    FILE* info = fopen((this->path + "info.txt").c_str(), "rb");
    while (!feof(info)) {
        if (fread(byte, 1, 1, info) == 1) {
            fwrite(byte, 1, 1, main);
            for (int i = 0; i < 8; i++) {
                bits.push_back((byte[0] >> i) & 1);
            }
        }
    }
    fclose(info);
    remove((this->path + "info.txt").c_str());
    for (int i = 0; i < files.size(); i++) {
        f = fopen((files[i]).c_str(), "rb");
        while (!feof(f)) {
            if (fread(byte, 1, 1, f) == 1) {
                fwrite(byte, 1, 1, main);
                for (int i = 0; i < 8; i++) {
                    bits.push_back((byte[0] >> i) & 1);
                }
            }
        }
        fclose(f);
    }
    fwrite(HammingCode(bits).c_str(), HammingCode(bits).size(), 1, main);
    fclose(main);
}

void Arch::DeCompress(std::string binary, std::vector<std::string> file, std::string name_file) {
    std::vector<uint64_t> bits;
    FILE* bin = fopen((binary + name_file).c_str(), "rb");
    char info_block_size[5];
    fread(info_block_size, 1, 5, bin);
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 8; j++) {
            bits.push_back((info_block_size[i] >> j) & 1);
        }
    }

    int sz = atoi(info_block_size);
    char* info_block = new char[sz];
    fread(info_block, 1, sz, bin);
    for (int i = 0; i < sz; i++) {
        for (int j = 0; j < 8; j++) {
            bits.push_back((info_block[i] >> j) & 1);
        }
    }
    std::vector<std::string> tokens;
    char* tok = strtok(info_block, "||");
    int toks = 0;
    while (tok) {
        tokens.push_back(tok);
        tok = strtok(nullptr, "||");
        toks++;
    }
    if (toks % 2 == 1) {
        toks--;
    }

    int files = toks / 2;
    char byte[1];
    for (int i = 0; i < files; i++) {
        const char* size = tokens[i * 2].c_str();
        const char* name = tokens[i * 2 + 1].c_str();
        char full_path[255];
        strcpy(full_path, (binary + name).c_str());
        int sz = atoi(size);
        for (int r = 1; r <= sz; r++) {
            if (fread(byte, 1, 1, bin) == 1) {
                for (int i = 0; i < 8; i++) {
                    bits.push_back((byte[0] >> i) & 1);
                }
            }
        }
    }

    std::string HammingStrGet;
    while (!feof(bin)) {
        if (fread(byte, 1, 1, bin) == 1) {
            HammingStrGet += byte[0];
        }
    }

    std::string hamming_str_decode = HammingCode(bits);
    fclose(bin);
    if (HammingStrGet != hamming_str_decode) {
        std::vector<uint64_t> fixBytes = HammingFixed(bits, HammingStrGet, hamming_str_decode);
        int j = 0;
        char ByteWrite[1];
        std::bitset<8> bytes;
        FILE* bin_ham = fopen((binary + name_file).c_str(), "wb");
        while (j < fixBytes.size()) {
            bytes[0] = fixBytes[j];
            bytes[1] = fixBytes[j + 1];
            bytes[2] = fixBytes[j + 2];
            bytes[3] = fixBytes[j + 3];
            bytes[4] = fixBytes[j + 4];
            bytes[5] = fixBytes[j + 5];
            bytes[6] = fixBytes[j + 6];
            bytes[7] = fixBytes[j + 7];
            unsigned long i = bytes.to_ulong();
            ByteWrite[0] = static_cast<unsigned char>( i );
            fwrite(ByteWrite, 1, 1, bin_ham);
            j += 8;
        }
        fclose(bin_ham);
    }

    bin = fopen((binary + name_file).c_str(), "rb");
    fread(info_block_size, 1, 5, bin);
    fread(info_block, 1, sz, bin);
    uint64_t iterator = 0;
    for (int i = 0; i < files; i++) {
        const char* size = tokens[i * 2].c_str();
        const char* name = tokens[i * 2 + 1].c_str();
        char full_path[255];
        char byte2[1];
        strcpy(full_path, (binary + name).c_str());
        int sz_f = atoi(size);
        if (file.empty()) {

            FILE* curr = fopen(full_path, "wb");
            for (int r = 1; r <= sz_f; r++) {

                if (fread(byte2, 1, 1, bin) == 1) {
                    fwrite(byte2, 1, 1, curr);
                }
            }
            fclose(curr);
        } else {
            if (name == file[iterator]) {
                FILE* curr = fopen(full_path, "wb");
                for (int r = 1; r <= sz_f; r++) {
                    if (fread(byte, 1, 1, bin) == 1) {
                        fwrite(byte, 1, 1, curr);
                    }
                }
                fclose(curr);
                iterator++;
            } else {
                for (int r = 1; r <= sz_f; r++) {
                    fread(byte, 1, 1, bin);
                }
            }
        }
    }
    fclose(bin);
}

std::vector<std::string> Arch::PrintFiles(std::string binary, std::string path) {
    std::vector<uint8_t> bits;
    FILE* bin = fopen(binary.c_str(), "rb");
    char info_block_size[5];
    fread(info_block_size, 1, 5, bin);
    int sz = atoi(info_block_size);
    char* info_block = new char[sz];
    fread(info_block, 1, sz, bin);
    std::vector<std::string> tokens;
    char* tok = strtok(info_block, "||");
    int toks = 0;
    while (tok) {
        tokens.push_back(tok);
        tok = strtok(nullptr, "||");
        toks++;
    }
    if (toks % 2 == 1) {
        toks--;
    }
    int files = toks / 2;
    char byte[1];
    std::vector<std::string> file_names;
    for (int i = 0; i < files; i++) {
        const char* size = tokens[i * 2].c_str();
        const char* name = tokens[i * 2 + 1].c_str();
        char full_path[255];
        strcpy(full_path, (binary + name).c_str());
        int sz_f = atoi(size);
        file_names.emplace_back(name);
        for (int r = 1; r <= sz_f; r++) {
            fread(byte, 1, 1, bin);
        }
    }

    return file_names;
}