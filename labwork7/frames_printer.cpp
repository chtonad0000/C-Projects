#include "parser_ID3v2.h"
#include <bitset>

const int kSignBits = 7;
const int kBitsForByte = 8;
const int kEventBytes = 5;
const int kEquBytes = 2;
const int kAbsoluteTimeSize = 4;

void TextFrame::PrintFrame() {
    std::cout << "ID: " << std::string(id) << '\n';
    int frame_size = CountFrameSize(size);
    std::cout << "size: " << frame_size << '\n';
    std::bitset<8> flag;
    std::cout << "flags: ";
    flag = flags[0];
    for (int i = 0; i < kBitsForByte; ++i) {
        std::cout << flag[i];
    }
    std::cout << ' ';
    flag = flags[1];
    for (int i = 0; i < kBitsForByte; ++i) {
        std::cout << flag[i];
    }
    std::cout << '\n';
    std::cout << "encoding: " << static_cast<int>(encoding[0]) << '\n';
    std::cout << "text: ";
    PrintChars(info, frame_size - sizeof(encoding));
    std::cout << '\n' << '\n';
}

void UFIFrame::PrintFrame() {
    std::cout << "ID: " << std::string(id) << '\n';
    int frame_size = CountFrameSize(size);
    std::cout << "size: " << frame_size << '\n';
    std::bitset<8> flag;
    std::cout << "flags: ";
    flag = flags[0];
    for (int i = 0; i < kBitsForByte; ++i) {
        std::cout << flag[i];
    }
    std::cout << ' ';
    flag = flags[1];
    for (int i = 0; i < kBitsForByte; ++i) {
        std::cout << flag[i];
    }
    std::cout << '\n';
    std::cout << "Owner ID: " << owner_id << '\n';
    std::cout << "Identification: ";
    PrintChars(id_data, frame_size - owner_id.size());
    std::cout << '\n' << '\n';
}

void URLFrame::PrintFrame() {
    std::cout << "ID: " << std::string(id) << '\n';
    int frame_size = CountFrameSize(size);
    std::cout << "size: " << frame_size << '\n';
    std::bitset<8> flag;
    std::cout << "flags: ";
    flag = flags[0];
    for (int i = 0; i < kBitsForByte; ++i) {
        std::cout << flag[i];
    }
    std::cout << ' ';
    flag = flags[1];
    for (int i = 0; i < kBitsForByte; ++i) {
        std::cout << flag[i];
    }
    std::cout << '\n';
    std::cout << "text: ";
    PrintChars(url, frame_size);
    std::cout << '\n' << '\n';
}

void EventFrame::PrintFrame() {
    std::cout << "ID: " << std::string(id) << '\n';
    int frame_size = CountFrameSize(size);
    std::cout << "size: " << frame_size << '\n';
    std::bitset<8> flag;
    std::cout << "flags: ";
    flag = flags[0];
    for (int i = 0; i < kBitsForByte; ++i) {
        std::cout << flag[i];
    }
    std::cout << ' ';
    flag = flags[1];
    for (int i = 0; i < kBitsForByte; ++i) {
        std::cout << flag[i];
    }
    std::cout << '\n';
    std::cout << "time format: " << int(time_format[0]) << '\n';
    for (int i = 0; i < (frame_size - sizeof(time_format)) / kEventBytes; ++i) {
        std::cout << "Event type: " << static_cast<int>(data[i * kEventBytes]) << '\n';
        int time_stamp = 0;
        for (int j = 1; j < kEventBytes; ++j) {
            time_stamp = (time_stamp << kBitsForByte) | data[j + i * kEventBytes];
        }
        std::cout << "Tamp stamp: " << time_stamp << '\n';
    }
    std::cout << '\n' << '\n';
}

void UnsyncLyricsFrame::PrintFrame() {
    std::cout << "ID: " << std::string(id) << '\n';
    int frame_size = CountFrameSize(size);
    std::cout << "size: " << frame_size << '\n';
    std::bitset<8> flag;
    std::cout << "flags: ";
    flag = flags[0];
    for (int i = 0; i < kBitsForByte; ++i) {
        std::cout << flag[i];
    }
    std::cout << ' ';
    flag = flags[1];
    for (int i = 0; i < kBitsForByte; ++i) {
        std::cout << flag[i];
    }
    std::cout << '\n';
    std::cout << "encoding: " << int(encoding[0]) << '\n';
    std::cout << "language: ";
    PrintChars(language, sizeof(language));
    std::cout << '\n';
    std::cout << "content descriptor: " << content_descriptor << '\n';
    std::cout << "text: ";
    PrintChars(lyrics, frame_size - sizeof(encoding) - sizeof(language) - content_descriptor.size() - 1);
    std::cout << '\n' << '\n';
}

void SyncLyricsFrame::PrintFrame() {
    std::cout << "ID: " << std::string(id) << '\n';
    int frame_size = CountFrameSize(size);
    std::cout << "size: " << frame_size << '\n';
    std::bitset<8> flag;
    std::cout << "flags: ";
    flag = flags[0];
    for (int i = 0; i < kBitsForByte; ++i) {
        std::cout << flag[i];
    }
    std::cout << ' ';
    flag = flags[1];
    for (int i = 0; i < kBitsForByte; ++i) {
        std::cout << flag[i];
    }
    std::cout << '\n';
    std::cout << "encoding: " << static_cast<int>(encoding[0]) << '\n';
    std::cout << "language: ";
    PrintChars(language, sizeof(language));
    std::cout << '\n';
    std::cout << "time stamp format: " << static_cast<int>(stamp_format[0]) << '\n';
    std::cout << "content type: " << static_cast<int>(content_type[0]) << '\n';
    std::cout << "content descriptor: " << descriptor << '\n';
    frame_size = frame_size - sizeof(encoding) - sizeof(language) - sizeof(stamp_format) - sizeof(content_type) -
                 descriptor.size() - 2;
    int k = 1;
    while (frame_size > 0) {
        std::string lyrics;
        while (text[k] != '\0') {
            lyrics += text[k];
            k += 1;
            frame_size -= 1;
        }
        std::cout << lyrics << ' ';
        k += 1;
        frame_size -= 1;
        int time_stamp;
        for (int i = 0; i < kAbsoluteTimeSize; ++i) {
            time_stamp = (time_stamp << kBitsForByte) | static_cast<unsigned char>(text[k]);
            k += 1;
            frame_size -= 1;
        }
        std::cout << time_stamp << '\n';
        k += 1;
        frame_size -= 1;
    }
    std::cout << '\n' << '\n';
}

void CommentFrame::PrintFrame() {
    std::cout << "ID: " << std::string(id) << '\n';
    int frame_size = CountFrameSize(size);
    std::cout << "size: " << frame_size << '\n';
    std::bitset<8> flag;
    std::cout << "flags: ";
    flag = flags[0];
    for (int i = 0; i < kBitsForByte; ++i) {
        std::cout << flag[i];
    }
    std::cout << ' ';
    flag = flags[1];
    for (int i = 0; i < kBitsForByte; ++i) {
        std::cout << flag[i];
    }
    std::cout << '\n';
    std::cout << "encoding: " << int(encoding[0]) << '\n';
    std::cout << "language: ";
    PrintChars(language, sizeof(language));
    std::cout << '\n';
    std::cout << "short content descriptor: " << short_descriptor << '\n';
    std::cout << "text: ";
    PrintChars(text, frame_size - sizeof(encoding) - sizeof(language) - short_descriptor.size() - 1);
    std::cout << '\n' << '\n';
}

void RelativeVolumeAdjFrame::PrintFrame() {
    std::cout << "ID: " << std::string(id) << '\n';
    int frame_size = CountFrameSize(size);
    std::cout << "size: " << frame_size << '\n';
    std::bitset<8> flag;
    std::cout << "flags: ";
    flag = flags[0];
    for (int i = 0; i < kBitsForByte; ++i) {
        std::cout << flag[i];
    }
    std::cout << ' ';
    flag = flags[1];
    for (int i = 0; i < kBitsForByte; ++i) {
        std::cout << flag[i];
    }
    std::cout << '\n';
    std::cout << "text: ";
    PrintChars(data, frame_size);
    std::cout << '\n' << '\n';
}

void EquFrame::PrintFrame() {
    std::cout << "ID: " << std::string(id) << '\n';
    int frame_size = CountFrameSize(size);
    std::cout << "size: " << frame_size << '\n';
    std::bitset<8> flag;
    std::cout << "flags: ";
    flag = flags[0];
    for (int i = 0; i < kBitsForByte; ++i) {
        std::cout << flag[i];
    }
    std::cout << ' ';
    flag = flags[1];
    for (int i = 0; i < kBitsForByte; ++i) {
        std::cout << flag[i];
    }
    std::cout << '\n';
    std::cout << "encoding: " << int(interpolation[0]) << '\n';
    std::cout << "identification: " << identification << '\0';
    frame_size -= (identification.size() + 1);
    for (int i = 0; i < frame_size / (2 * kEquBytes); ++i) {
        int frequency = 0;
        int volume_adj = 0;
        for (int j = 0; j < kEquBytes; ++j) {
            frequency = (frame_size << kSignBits) | static_cast<unsigned char>(data[i * 2 * kEquBytes + j]);
        }
        for (int j = 0; j < kEquBytes; ++j) {
            volume_adj = (frame_size << kSignBits) | data[i * 2 * kEquBytes + j + kEquBytes];
        }
        std::cout << "frequency: " << frequency << '\0';
        std::cout << "volume adjustment: " << volume_adj << '\0';
    }
    std::cout << '\n' << '\n';
}

void PlayCounterFrame::PrintFrame() {
    std::cout << "ID: " << std::string(id) << '\n';
    int frame_size = CountFrameSize(size);
    std::cout << "size: " << frame_size << '\n';
    std::bitset<8> flag;
    std::cout << "flags: ";
    flag = flags[0];
    for (int i = 0; i < kBitsForByte; ++i) {
        std::cout << flag[i];
    }
    std::cout << ' ';
    flag = flags[1];
    for (int i = 0; i < kBitsForByte; ++i) {
        std::cout << flag[i];
    }
    std::cout << '\n';
    std::cout << "text: ";
    PrintChars(counter, frame_size);
    std::cout << '\n' << '\n';
}

void PopularimeterFrame::PrintFrame() {
    std::cout << "ID: " << std::string(id) << '\n';
    int frame_size = CountFrameSize(size);
    std::cout << "size: " << frame_size << '\n';
    std::bitset<8> flag;
    std::cout << "flags: ";
    flag = flags[0];
    for (int i = 0; i < kBitsForByte; ++i) {
        std::cout << flag[i];
    }
    std::cout << ' ';
    flag = flags[1];
    for (int i = 0; i < kBitsForByte; ++i) {
        std::cout << flag[i];
    }
    std::cout << '\n';
    std::cout << "Email for user: " << email << '\n';
    std::cout << "Rating: " << static_cast<int>(rating[0]) << '\n';
    std::cout << "Counter: ";
    PrintChars(counter, frame_size - email.size() - 1 - sizeof(rating));
    std::cout << '\n' << '\n';
}

void BuffSizeFrame::PrintFrame() {
    std::cout << "ID: " << std::string(id) << '\n';
    int frame_size = CountFrameSize(size);
    std::cout << "size: " << frame_size << '\n';
    std::bitset<8> binary_flag;
    std::cout << "flags: ";
    binary_flag = flags[0];
    for (int i = 0; i < kBitsForByte; ++i) {
        std::cout << binary_flag[i];
    }
    std::cout << ' ';
    binary_flag = flags[1];
    for (int i = 0; i < kBitsForByte; ++i) {
        std::cout << binary_flag[i];
    }
    std::cout << '\n';
    std::cout << "buffer size: "
              << ((buff_size[0] << 2 * kBitsForByte) | (buff_size[1] << 1 * kBitsForByte) | (buff_size[2])) << '\n';
    std::cout << "flag: " << int(flag[0]) << '\n';
    std::cout << "offset: "
              << ((offset[0] << 3 * kBitsForByte) | (offset[1] << 2 * kBitsForByte) | (offset[2] << 1 * kBitsForByte) |
                  (offset[3])) << '\n' << '\n';
}

void LinkInfoFrame::PrintFrame() {
    std::cout << "ID: " << std::string(id) << '\n';
    int frame_size = CountFrameSize(size);
    std::cout << "size: " << frame_size << '\n';
    std::bitset<8> flag;
    std::cout << "flags: ";
    flag = flags[0];
    for (int i = 0; i < kBitsForByte; ++i) {
        std::cout << flag[i];
    }
    std::cout << ' ';
    flag = flags[1];
    for (int i = 0; i < kBitsForByte; ++i) {
        std::cout << flag[i];
    }
    std::cout << '\n';
    std::cout << "frame identifier: " << ((frame_id[0] << 3 * kBitsForByte) | (frame_id[1] << 2 * kBitsForByte) |
                                          (frame_id[2] << 1 * kBitsForByte) | (frame_id[3])) << '\n';
    std::cout << "URL: " << url << '\0';
    std::cout << "data: ";
    PrintChars(data, frame_size - sizeof(frame_id) - url.size() - 1);
    std::cout << '\n' << '\n';
}

void PosSyncFrame::PrintFrame() {
    std::cout << "ID: " << std::string(id) << '\n';
    int frame_size = CountFrameSize(size);
    std::cout << "size: " << frame_size << '\n';
    std::bitset<8> flag;
    std::cout << "flags: ";
    flag = flags[0];
    for (int i = 0; i < kBitsForByte; ++i) {
        std::cout << flag[i];
    }
    std::cout << ' ';
    flag = flags[1];
    for (int i = 0; i < kBitsForByte; ++i) {
        std::cout << flag[i];
    }
    std::cout << '\n';
    std::cout << "stamp format: " << static_cast<int>(stamp_format[0]) << '\n';
    std::cout << "text: ";
    PrintChars(position, frame_size - sizeof(stamp_format));
    std::cout << '\n' << '\n';
}

void TermsOfUseFrame::PrintFrame() {
    std::cout << "ID: " << std::string(id) << '\n';
    int frame_size = CountFrameSize(size);
    std::cout << "size: " << frame_size << '\n';
    std::bitset<8> flag;
    std::cout << "flags: ";
    flag = flags[0];
    for (int i = 0; i < kBitsForByte; ++i) {
        std::cout << flag[i];
    }
    std::cout << ' ';
    flag = flags[1];
    for (int i = 0; i < kBitsForByte; ++i) {
        std::cout << flag[i];
    }
    std::cout << '\n';
    std::cout << "encoding: " << static_cast<int>(encoding[0]) << '\n';
    std::cout << "language: ";
    PrintChars(language, sizeof(language));
    std::cout << '\n';
    std::cout << "text: ";
    PrintChars(text, frame_size - sizeof(encoding) - sizeof(language));
    std::cout << '\n' << '\n';
}

void OwnershipFrame::PrintFrame() {
    std::cout << "ID: " << std::string(id) << '\n';
    int frame_size = CountFrameSize(size);
    std::cout << "size: " << frame_size << '\n';
    std::bitset<8> flag;
    std::cout << "flags: ";
    flag = flags[0];
    for (int i = 0; i < kBitsForByte; ++i) {
        std::cout << flag[i];
    }
    std::cout << ' ';
    flag = flags[1];
    for (int i = 0; i < kBitsForByte; ++i) {
        std::cout << flag[i];
    }
    std::cout << '\n';
    std::cout << "encoding: " << static_cast<int>(encoding[0]) << '\n';
    std::cout << "price paid: " << price_paid << '\n';
    std::cout << "date: ";
    PrintChars(date, sizeof(date));
    std::cout << '\0';
    std::cout << "info: ";
    PrintChars(info, frame_size - sizeof(encoding) - sizeof(date) - price_paid.size() - 1);
    std::cout << '\n' << '\n';
}

void CommercialFrame::PrintFrame() {
    std::cout << "ID: " << std::string(id) << '\n';
    int frame_size = CountFrameSize(size);
    std::cout << "size: " << frame_size << '\n';
    std::bitset<8> flag;
    std::cout << "flags: ";
    flag = flags[0];
    for (int i = 0; i < kBitsForByte; ++i) {
        std::cout << flag[i];
    }
    std::cout << ' ';
    flag = flags[1];
    for (int i = 0; i < kBitsForByte; ++i) {
        std::cout << flag[i];
    }
    std::cout << '\n';
    std::cout << "encoding: " << static_cast<int>(encoding[0]) << '\n';
    std::cout << "info: ";
    PrintChars(info, frame_size - sizeof(encoding));
    std::cout << '\n' << '\n';
}

void EncryptionMethodRegFrame::PrintFrame() {
    std::cout << "ID: " << std::string(id) << '\n';
    int frame_size = CountFrameSize(size);
    std::cout << "size: " << frame_size << '\n';
    std::bitset<8> flag;
    std::cout << "flags: ";
    flag = flags[0];
    for (int i = 0; i < kBitsForByte; ++i) {
        std::cout << flag[i];
    }
    std::cout << ' ';
    flag = flags[1];
    for (int i = 0; i < kBitsForByte; ++i) {
        std::cout << flag[i];
    }
    std::cout << '\n';
    std::cout << "owner identifier: " << owner_id << '\0';
    std::cout << "method symbol: " << static_cast<int>(method[0]) << '\0';
    std::cout << "data: ";
    PrintChars(data, frame_size - sizeof(method) - owner_id.size() - 1);
    std::cout << '\n' << '\n';
}

void GroupIdRegFrame::PrintFrame() {
    std::cout << "ID: " << std::string(id) << '\n';
    int frame_size = CountFrameSize(size);
    std::cout << "size: " << frame_size << '\n';
    std::bitset<8> flag;
    std::cout << "flags: ";
    flag = flags[0];
    for (int i = 0; i < kBitsForByte; ++i) {
        std::cout << flag[i];
    }
    std::cout << ' ';
    flag = flags[1];
    for (int i = 0; i < kBitsForByte; ++i) {
        std::cout << flag[i];
    }
    std::cout << '\n';
    std::cout << "owner identifier: " << owner_id << '\0';
    std::cout << "group symbol: " << static_cast<int>(group[0]) << '\0';
    std::cout << "data: ";
    PrintChars(data, frame_size - sizeof(group) - owner_id.size() - 1);
    std::cout << '\n' << '\n';
}

void PrivateFrame::PrintFrame() {
    std::cout << "ID: " << std::string(id) << '\n';
    int frame_size = CountFrameSize(size);
    std::cout << "size: " << frame_size << '\n';
    std::bitset<8> flag;
    std::cout << "flags: ";
    flag = flags[0];
    for (int i = 0; i < kBitsForByte; ++i) {
        std::cout << flag[i];
    }
    std::cout << ' ';
    flag = flags[1];
    for (int i = 0; i < kBitsForByte; ++i) {
        std::cout << flag[i];
    }
    std::cout << '\n';
    std::cout << "owner identifier: " << owner_id << '\0';
    std::cout << "private data: ";
    PrintChars(private_data, frame_size - owner_id.size() - 1);
    std::cout << '\n' << '\n';
}

void SeekFrame::PrintFrame() {
    std::cout << "ID: " << std::string(id) << '\n';
    int frame_size = CountFrameSize(size);
    std::cout << "size: " << frame_size << '\n';

    std::bitset<8> flag;
    std::cout << "flags: ";
    flag = flags[0];
    for (int i = 0; i < kBitsForByte; ++i) {
        std::cout << flag[i];
    }
    std::cout << ' ';
    flag = flags[1];
    for (int i = 0; i < kBitsForByte; ++i) {
        std::cout << flag[i];
    }
    std::cout << '\n';
    std::cout << "minimal offset: " << ((min_offset[0] << 3 * kBitsForByte) | (min_offset[1] << 2 * kBitsForByte) |
                                        (min_offset[2] << 1 * kBitsForByte) | (min_offset[3])) << '\n' << '\n';
}
