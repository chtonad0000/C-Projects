#include "parser_ID3v2.h"

const int kSignBits = 7;
const int kBitsForByte = 8;
const int kEventBytes = 5;
const int kEquBytes = 2;
const int kAbsoluteTimeSize = 4;

TextFrame* TextFrame::ReadFrame(std::string& s, FILE* file) {
    for (int i = 0; i < sizeof(size); ++i) {
        id[i] = s[i];
    }
    fread(size, 1, 4, file);
    fread(flags, 1, 2, file);
    fread(encoding, 1, 1, file);
    int frame_size = CountFrameSize(size);
    info = new char[frame_size - sizeof(encoding)];
    fread(info, 1, frame_size - sizeof(encoding), file);

    return this;
}

UFIFrame* UFIFrame::ReadFrame(std::string& s, FILE* file) {
    for (int i = 0; i < sizeof(size); ++i) {
        id[i] = s[i];
    }
    fread(size, 1, 4, file);
    fread(flags, 1, 2, file);
    int frame_size = CountFrameSize(size);
    char buffer[1];
    fread(buffer, 1, 1, file);
    frame_size -= 1;
    while (buffer[0] != '\0') {
        owner_id += buffer[0];
        fread(buffer, 1, 1, file);
        frame_size -= 1;
    }
    id_data = new char[frame_size];
    fread(id_data, 1, frame_size, file);

    return this;
}

URLFrame* URLFrame::ReadFrame(std::string& s, FILE* file) {
    for (int i = 0; i < sizeof(size); ++i) {
        id[i] = s[i];
    }
    fread(size, 1, 4, file);
    fread(flags, 1, 2, file);
    int frame_size = CountFrameSize(size);
    url = new char[frame_size];
    fread(url, 1, frame_size, file);

    return this;
}

EventFrame* EventFrame::ReadFrame(std::string& s, FILE* file) {
    for (int i = 0; i < sizeof(size); ++i) {
        id[i] = s[i];
    }
    fread(size, 1, 4, file);
    fread(flags, 1, 2, file);
    fread(time_format, 1, 1, file);
    int frame_size = CountFrameSize(size);
    data = new unsigned char[frame_size - sizeof(time_format)];
    fread(data, 1, frame_size - sizeof(time_format), file);

    return this;
}

UnsyncLyricsFrame* UnsyncLyricsFrame::ReadFrame(std::string& s, FILE* file) {
    for (int i = 0; i < sizeof(size); ++i) {
        id[i] = s[i];
    }
    fread(size, 1, 4, file);
    fread(flags, 1, 2, file);
    fread(encoding, 1, 1, file);
    fread(language, 1, 3, file);
    int frame_size = CountFrameSize(size);
    char buffer[1];
    fread(buffer, 1, 1, file);
    frame_size -= 1;
    while (buffer[0] != '\0') {
        content_descriptor += buffer[0];
        fread(buffer, 1, 1, file);
        frame_size -= 1;
    }
    lyrics = new char[frame_size - sizeof(encoding) - sizeof(language)];
    fread(lyrics, 1, frame_size - sizeof(encoding) - sizeof(language), file);

    return this;
}

SyncLyricsFrame* SyncLyricsFrame::ReadFrame(std::string& s, FILE* file) {
    for (int i = 0; i < sizeof(size); ++i) {
        id[i] = s[i];
    }
    fread(size, 1, 4, file);
    fread(flags, 1, 2, file);
    fread(encoding, 1, 1, file);
    fread(language, 1, 3, file);
    fread(stamp_format, 1, 1, file);
    fread(content_type, 1, 1, file);
    int frame_size = CountFrameSize(size);
    char buffer[1];
    fread(buffer, 1, 1, file);
    frame_size -= 1;
    while (buffer[0] != '\0') {
        descriptor += buffer[0];
        fread(buffer, 1, 1, file);
        frame_size -= 1;
    }
    text = new char[frame_size - sizeof(encoding) - sizeof(language) - sizeof(stamp_format) - sizeof(content_type)];
    fread(text, 1, frame_size - sizeof(encoding) - sizeof(language) - sizeof(stamp_format) - sizeof(content_type),
          file);

    return this;
}

CommentFrame* CommentFrame::ReadFrame(std::string& s, FILE* file) {
    for (int i = 0; i < sizeof(size); ++i) {
        id[i] = s[i];
    }
    fread(size, 1, 4, file);
    fread(flags, 1, 2, file);
    fread(encoding, 1, 1, file);
    fread(language, 1, 3, file);
    int frame_size = CountFrameSize(size);
    char buffer[1];
    fread(buffer, 1, 1, file);
    frame_size -= 1;
    while (buffer[0] != '\0') {
        short_descriptor += buffer[0];
        fread(buffer, 1, 1, file);
        frame_size -= 1;
    }
    text = new char[frame_size - sizeof(encoding) - sizeof(language)];
    fread(text, 1, frame_size - sizeof(encoding) - sizeof(language), file);

    return this;
}

RelativeVolumeAdjFrame* RelativeVolumeAdjFrame::ReadFrame(std::string& s, FILE* file) {
    for (int i = 0; i < sizeof(size); ++i) {
        id[i] = s[i];
    }
    fread(size, 1, 4, file);
    fread(flags, 1, 2, file);
    int frame_size = CountFrameSize(size);
    data = new char[frame_size];
    fread(data, 1, frame_size, file);

    return this;
}

EquFrame* EquFrame::ReadFrame(std::string& s, FILE* file) {
    for (int i = 0; i < sizeof(size); ++i) {
        id[i] = s[i];
    }
    fread(size, 1, 4, file);
    fread(flags, 1, 2, file);
    fread(interpolation, 1, 1, file);
    int frame_size = CountFrameSize(size);
    char buffer[1];
    fread(buffer, 1, 1, file);
    frame_size -= 1;
    while (buffer[0] != '\0') {
        identification += buffer[0];
        fread(buffer, 1, 1, file);
        frame_size -= 1;
    }
    data = new char[frame_size - sizeof(interpolation)];
    fread(data, 1, frame_size - sizeof(interpolation), file);

    return this;
}

PlayCounterFrame* PlayCounterFrame::ReadFrame(std::string& s, FILE* file) {
    for (int i = 0; i < sizeof(size); ++i) {
        id[i] = s[i];
    }
    fread(size, 1, 4, file);
    fread(flags, 1, 2, file);
    int frame_size = CountFrameSize(size);
    counter = new char[frame_size];
    fread(counter, 1, frame_size, file);

    return this;
}

PopularimeterFrame* PopularimeterFrame::ReadFrame(std::string& s, FILE* file) {
    for (int i = 0; i < sizeof(size); ++i) {
        id[i] = s[i];
    }
    fread(size, 1, 4, file);
    fread(flags, 1, 2, file);
    int frame_size = CountFrameSize(size);
    char buffer[1];
    fread(buffer, 1, 1, file);
    frame_size -= 1;
    while (buffer[0] != '\0') {
        email += buffer[0];
        fread(buffer, 1, 1, file);
        frame_size -= 1;
    }
    fread(rating, 1, 1, file);
    counter = new char[frame_size - 1];
    fread(counter, 1, frame_size - 1, file);

    return this;
}

BuffSizeFrame* BuffSizeFrame::ReadFrame(std::string& s, FILE* file) {
    for (int i = 0; i < sizeof(size); ++i) {
        id[i] = s[i];
    }
    fread(size, 1, 4, file);
    fread(flags, 1, 2, file);
    fread(buff_size, 1, 3, file);
    fread(flag, 1, 1, file);
    fread(offset, 1, 4, file);

    return this;
}

LinkInfoFrame* LinkInfoFrame::ReadFrame(std::string& s, FILE* file) {
    for (int i = 0; i < sizeof(size); ++i) {
        id[i] = s[i];
    }
    fread(size, 1, 4, file);
    fread(flags, 1, 2, file);
    fread(frame_id, 1, 4, file);
    int frame_size = CountFrameSize(size);
    char buffer[1];
    fread(buffer, 1, 1, file);
    frame_size -= 1;
    while (buffer[0] != '\0') {
        url += buffer[0];
        fread(buffer, 1, 1, file);
        frame_size -= 1;
    }
    data = new char[frame_size - sizeof(frame_id)];
    fread(data, 1, frame_size - sizeof(frame_id), file);

    return this;
}

PosSyncFrame* PosSyncFrame::ReadFrame(std::string& s, FILE* file) {
    for (int i = 0; i < sizeof(size); ++i) {
        id[i] = s[i];
    }
    fread(size, 1, 4, file);
    fread(flags, 1, 2, file);
    fread(stamp_format, 1, 1, file);
    int frame_size = CountFrameSize(size);
    position = new char[frame_size - sizeof(stamp_format)];
    fread(position, 1, frame_size - sizeof(stamp_format), file);

    return this;
}

TermsOfUseFrame* TermsOfUseFrame::ReadFrame(std::string& s, FILE* file) {
    for (int i = 0; i < sizeof(size); ++i) {
        id[i] = s[i];
    }
    fread(size, 1, 4, file);
    fread(flags, 1, 2, file);
    fread(encoding, 1, 1, file);
    fread(language, 1, 3, file);
    int frame_size = CountFrameSize(size);
    text = new char[frame_size - sizeof(encoding) - sizeof(language)];
    fread(text, 1, frame_size - sizeof(encoding) - sizeof(language), file);

    return this;
}

OwnershipFrame* OwnershipFrame::ReadFrame(std::string& s, FILE* file) {
    for (int i = 0; i < sizeof(size); ++i) {
        id[i] = s[i];
    }
    fread(size, 1, 4, file);
    fread(flags, 1, 2, file);
    fread(encoding, 1, 1, file);
    int frame_size = CountFrameSize(size);
    char buffer[1];
    fread(buffer, 1, 1, file);
    frame_size -= 1;
    while (buffer[0] != '\0') {
        price_paid += buffer[0];
        fread(buffer, 1, 1, file);
        frame_size -= 1;
    }
    fread(date, 1, 8, file);
    info = new char[frame_size - sizeof(encoding) - sizeof(date)];
    fread(info, 1, frame_size - sizeof(encoding), file);

    return this;
}

CommercialFrame* CommercialFrame::ReadFrame(std::string& s, FILE* file) {
    for (int i = 0; i < sizeof(size); ++i) {
        id[i] = s[i];
    }
    fread(size, 1, 4, file);
    fread(flags, 1, 2, file);
    fread(encoding, 1, 1, file);
    int frame_size = CountFrameSize(size);
    info = new char[frame_size - sizeof(encoding)];
    fread(info, 1, frame_size - sizeof(encoding), file);

    return this;
}

EncryptionMethodRegFrame* EncryptionMethodRegFrame::ReadFrame(std::string& s, FILE* file) {
    for (int i = 0; i < sizeof(size); ++i) {
        id[i] = s[i];
    }
    fread(size, 1, 4, file);
    fread(flags, 1, 2, file);
    int frame_size = CountFrameSize(size);
    char buffer[1];
    fread(buffer, 1, 1, file);
    frame_size -= 1;
    while (buffer[0] != '\0') {
        owner_id += buffer[0];
        fread(buffer, 1, 1, file);
        frame_size -= 1;
    }
    fread(method, 1, 1, file);
    data = new char[frame_size - sizeof(method)];
    fread(data, 1, frame_size, file);

    return this;
}

GroupIdRegFrame* GroupIdRegFrame::ReadFrame(std::string& s, FILE* file) {
    for (int i = 0; i < sizeof(size); ++i) {
        id[i] = s[i];
    }
    fread(size, 1, 4, file);
    fread(flags, 1, 2, file);
    int frame_size = CountFrameSize(size);
    char buffer[1];
    fread(buffer, 1, 1, file);
    frame_size -= 1;
    while (buffer[0] != '\0') {
        owner_id += buffer[0];
        fread(buffer, 1, 1, file);
        frame_size -= 1;
    }
    fread(group, 1, 1, file);
    data = new char[frame_size - sizeof(group)];
    fread(data, 1, frame_size, file);

    return this;
}

PrivateFrame* PrivateFrame::ReadFrame(std::string& s, FILE* file) {
    for (int i = 0; i < sizeof(size); ++i) {
        id[i] = s[i];
    }
    fread(size, 1, 4, file);
    fread(flags, 1, 2, file);
    int frame_size = CountFrameSize(size);
    char buffer[1];
    fread(buffer, 1, 1, file);
    frame_size -= 1;
    while (buffer[0] != '\0') {
        owner_id += buffer[0];
        fread(buffer, 1, 1, file);
        frame_size -= 1;
    }
    private_data = new char[frame_size];
    fread(private_data, 1, frame_size, file);

    return this;
}


SeekFrame* SeekFrame::ReadFrame(std::string& s, FILE* file) {
    for (int i = 0; i < sizeof(size); ++i) {
        id[i] = s[i];
    }
    fread(size, 1, 4, file);
    fread(flags, 1, 2, file);
    fread(min_offset, 1, 4, file);

    return this;
}
