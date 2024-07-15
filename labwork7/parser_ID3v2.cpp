#include "library.h"
#include "parser_ID3v2.h"

const int kSignBits = 7;
const int kBitsForByte = 8;
const int kEventBytes = 5;
const int kEquBytes = 2;
const int kAbsoluteTimeSize = 4;

void PrintChars(char* s, int size) {
    for (int i = 0; i < size; ++i) {
        std::cout << s[i];
    }
}

int CountFrameSize(const char* size) {
    int frame_size = 0;
    for (int i = 0; i < 4; ++i) {
        frame_size = (frame_size << kSignBits) | size[i];
    }
    return frame_size;
}

int ID3v2Header::ReadHeader(FILE* file) {
    if (fread(id, 1, 3, file) != 3) {
        return -1;
    }
    char str2[3] = {'I', 'D', '3'};
    for (int i = 0; i < 3; ++i) {
        if (id[i] != str2[i]) {
            return -1;
        }
    }
    char size[4];
    fread(version, 1, 2, file);
    fread(flags, 1, 1, file);
    fread(size, 1, 4, file);
    for (int i = 0; i < sizeof(size); ++i) {
        tag_size = (tag_size << kSignBits) | size[i];
    }
    return tag_size;
}


std::vector<ID3v2Frame*> ParseFrames(FILE* file, int tag_size) {
    int frames_size = tag_size;
    std::vector<ID3v2Frame*> frames;
    char id[4];
    while (frames_size > 0) {
        fread(id, 1, 4, file);
        std::string s;
        for (int i = 0; i < 4; ++i) {
            s += id[i];
        }
        if (s == "") {
            break;
        }
        if (text.contains(s)) {
            auto* frame = new TextFrame;
            frame = frame->ReadFrame(s, file);
            int frame_size = CountFrameSize(frame->size);
            frames_size = frames_size - sizeof(frame->id) - sizeof(frame->size) - sizeof(frame->flags) - frame_size;
            frames.emplace_back(frame);
        } else if (uniq_file_id.contains(s)) {
            auto* frame = new UFIFrame;
            frame = frame->ReadFrame(s, file);
            int frame_size = CountFrameSize(frame->size);
            frames_size = frames_size - sizeof(frame->id) - sizeof(frame->size) - sizeof(frame->flags) - frame_size;
            frames.emplace_back(frame);
        } else if (link.contains(s)) {
            auto* frame = new URLFrame;
            frame = frame->ReadFrame(s, file);
            int frame_size = CountFrameSize(frame->size);
            frames_size = frames_size - sizeof(frame->id) - sizeof(frame->size) - sizeof(frame->flags) - frame_size;
            frames.emplace_back(frame);
        } else if (event.contains(s)) {
            auto* frame = new EventFrame;
            frame = frame->ReadFrame(s, file);
            int frame_size = CountFrameSize(frame->size);
            frames_size = frames_size - sizeof(frame->id) - sizeof(frame->size) - sizeof(frame->flags) - frame_size;
            frames.emplace_back(frame);
        } else if (unsync_lyrics.contains(s)) {
            auto* frame = new UnsyncLyricsFrame;
            frame = frame->ReadFrame(s, file);
            int frame_size = CountFrameSize(frame->size);
            frames_size = frames_size - sizeof(frame->id) - sizeof(frame->size) - sizeof(frame->flags) - frame_size;
            frames.emplace_back(frame);
        } else if (sync_lyrics.contains(s)) {
            auto* frame = new SyncLyricsFrame;
            frame = frame->ReadFrame(s, file);
            int frame_size = CountFrameSize(frame->size);
            frames_size = frames_size - sizeof(frame->id) - sizeof(frame->size) - sizeof(frame->flags) - frame_size;
            frames.emplace_back(frame);
        } else if (comments.contains(s)) {
            auto* frame = new CommentFrame;
            frame = frame->ReadFrame(s, file);
            int frame_size = CountFrameSize(frame->size);
            frames_size = frames_size - sizeof(frame->id) - sizeof(frame->size) - sizeof(frame->flags) - frame_size;
            frames.emplace_back(frame);
        } else if (volume.contains(s)) {
            auto* frame = new RelativeVolumeAdjFrame;
            frame = frame->ReadFrame(s, file);
            int frame_size = CountFrameSize(frame->size);
            frames_size = frames_size - sizeof(frame->id) - sizeof(frame->size) - sizeof(frame->flags) - frame_size;
            frames.emplace_back(frame);
        } else if (equ.contains(s)) {
            auto* frame = new EquFrame;
            frame = frame->ReadFrame(s, file);
            int frame_size = CountFrameSize(frame->size);
            frames_size = frames_size - sizeof(frame->id) - sizeof(frame->size) - sizeof(frame->flags) - frame_size;
            frames.emplace_back(frame);
        } else if (play_counter.contains(s)) {
            auto* frame = new PlayCounterFrame;
            frame = frame->ReadFrame(s, file);
            int frame_size = CountFrameSize(frame->size);
            frames_size = frames_size - sizeof(frame->id) - sizeof(frame->size) - sizeof(frame->flags) - frame_size;
            frames.emplace_back(frame);
        } else if (poplarimeter.contains(s)) {
            auto* frame = new PopularimeterFrame;
            frame = frame->ReadFrame(s, file);
            int frame_size = CountFrameSize(frame->size);
            frames_size = frames_size - sizeof(frame->id) - sizeof(frame->size) - sizeof(frame->flags) - frame_size;
            frames.emplace_back(frame);
        } else if (buff_size.contains(s)) {
            auto* frame = new BuffSizeFrame;
            frame = frame->ReadFrame(s, file);
            int frame_size = CountFrameSize(frame->size);
            frames_size = frames_size - sizeof(frame->id) - sizeof(frame->size) - sizeof(frame->flags) - frame_size;
            frames.emplace_back(frame);
        } else if (link_info.contains(s)) {
            auto* frame = new LinkInfoFrame;
            frame = frame->ReadFrame(s, file);
            int frame_size = CountFrameSize(frame->size);
            frames_size = frames_size - sizeof(frame->id) - sizeof(frame->size) - sizeof(frame->flags) - frame_size;
            frames.emplace_back(frame);
        } else if (pos_sync.contains(s)) {
            auto* frame = new PosSyncFrame;
            frame = frame->ReadFrame(s, file);
            int frame_size = CountFrameSize(frame->size);
            frames_size = frames_size - sizeof(frame->id) - sizeof(frame->size) - sizeof(frame->flags) - frame_size;
            frames.emplace_back(frame);
        } else if (terms.contains(s)) {
            auto* frame = new TermsOfUseFrame;
            frame = frame->ReadFrame(s, file);
            int frame_size = CountFrameSize(frame->size);
            frames_size = frames_size - sizeof(frame->id) - sizeof(frame->size) - sizeof(frame->flags) - frame_size;
            frames.emplace_back(frame);
        } else if (ownership.contains(s)) {
            auto* frame = new OwnershipFrame;
            frame = frame->ReadFrame(s, file);
            int frame_size = CountFrameSize(frame->size);
            frames_size = frames_size - sizeof(frame->id) - sizeof(frame->size) - sizeof(frame->flags) - frame_size;
            frames.emplace_back(frame);
        } else if (commercial.contains(s)) {
            auto* frame = new CommercialFrame;
            frame = frame->ReadFrame(s, file);
            int frame_size = CountFrameSize(frame->size);
            frames_size = frames_size - sizeof(frame->id) - sizeof(frame->size) - sizeof(frame->flags) - frame_size;
            frames.emplace_back(frame);
        } else if (encrypt_method.contains(s)) {
            auto* frame = new EncryptionMethodRegFrame;
            frame = frame->ReadFrame(s, file);
            int frame_size = CountFrameSize(frame->size);
            frames_size = frames_size - sizeof(frame->id) - sizeof(frame->size) - sizeof(frame->flags) - frame_size;
            frames.emplace_back(frame);
        } else if (group_id.contains(s)) {
            auto* frame = new GroupIdRegFrame;
            frame = frame->ReadFrame(s, file);
            int frame_size = CountFrameSize(frame->size);
            frames_size = frames_size - sizeof(frame->id) - sizeof(frame->size) - sizeof(frame->flags) - frame_size;
            frames.emplace_back(frame);
        } else if (private_fr.contains(s)) {
            auto* frame = new PrivateFrame;
            frame = frame->ReadFrame(s, file);
            int frame_size = CountFrameSize(frame->size);
            frames_size = frames_size - sizeof(frame->id) - sizeof(frame->size) - sizeof(frame->flags) - frame_size;
            frames.emplace_back(frame);
        } else if (seek.contains(s)) {
            auto* frame = new SeekFrame;
            frame = frame->ReadFrame(s, file);
            int frame_size = CountFrameSize(frame->size);
            frames_size = frames_size - sizeof(frame->id) - sizeof(frame->size) - sizeof(frame->flags) - frame_size;
            frames.emplace_back(frame);
        } else {
            char size[4];
            char flags[2];
            fread(size, 1, 4, file);
            fread(flags, 1, 2, file);
            int frame_size = CountFrameSize(size);
            frames_size = frames_size - sizeof(id) - sizeof(size) - sizeof(flags) - frame_size;
            char* buffer = new char[frame_size];
            fread(buffer, 1, frame_size, file);
            delete[] buffer;
        }
    }

    return frames;
}

TextFrame::~TextFrame() {
    delete info;
}

UFIFrame::~UFIFrame() {
    delete id_data;
}

URLFrame::~URLFrame() {
    delete url;
}

EventFrame::~EventFrame() {
    delete data;
}

UnsyncLyricsFrame::~UnsyncLyricsFrame() {
    delete lyrics;
}

SyncLyricsFrame::~SyncLyricsFrame() {
    delete text;
}

CommentFrame::~CommentFrame() {
    delete text;
}

RelativeVolumeAdjFrame::~RelativeVolumeAdjFrame() {
    delete data;
}

EquFrame::~EquFrame() {
    delete data;
}

PlayCounterFrame::~PlayCounterFrame() {
    delete counter;
}

PopularimeterFrame::~PopularimeterFrame() {
    delete counter;
}

BuffSizeFrame::~BuffSizeFrame() = default;

LinkInfoFrame::~LinkInfoFrame() {
    delete data;
}

PosSyncFrame::~PosSyncFrame() {
    delete position;
}

TermsOfUseFrame::~TermsOfUseFrame() {
    delete text;
}

OwnershipFrame::~OwnershipFrame() {
    delete info;
}

CommercialFrame::~CommercialFrame() {
    delete info;
}

EncryptionMethodRegFrame::~EncryptionMethodRegFrame() {
    delete data;
}

GroupIdRegFrame::~GroupIdRegFrame() {
    delete data;
}

PrivateFrame::~PrivateFrame() {
    delete private_data;
}

SeekFrame::~SeekFrame() = default;
