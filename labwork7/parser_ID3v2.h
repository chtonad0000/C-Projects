#include <iostream>
#include <vector>

class ID3v2Header {
public:
    int ReadHeader(FILE* file);

    char id[3];
    char version[2];
    char flags[1];
    int tag_size;

    ~ID3v2Header() = default;
};

class ID3v2Frame {
public:
    char id[4];
    char size[4];
    char flags[2];

    virtual ~ID3v2Frame() = default;

    virtual void PrintFrame() = 0;

    virtual ID3v2Frame* ReadFrame(std::string& s, FILE* file) = 0;

};

class UFIFrame : public ID3v2Frame {
public:
    void PrintFrame() override;

    UFIFrame* ReadFrame(std::string& s, FILE* file) override;

    ~UFIFrame() override;

private:
    std::string owner_id;
    char* id_data;
};

class TextFrame : public ID3v2Frame {
public:
    void PrintFrame() override;

    TextFrame* ReadFrame(std::string& s, FILE* file) override;

    ~TextFrame() override;

private:
    char encoding[1];
    char* info;
};

class URLFrame : public ID3v2Frame {
public:
    void PrintFrame() override;

    URLFrame* ReadFrame(std::string& s, FILE* file) override;

    ~URLFrame() override;

private:
    char* url;
};

class EventFrame : public ID3v2Frame {
public:
    void PrintFrame() override;

    EventFrame* ReadFrame(std::string& s, FILE* file) override;

    ~EventFrame() override;

private:
    char time_format[1];
    unsigned char* data;
};

class UnsyncLyricsFrame : public ID3v2Frame {
public:
    void PrintFrame() override;

    UnsyncLyricsFrame* ReadFrame(std::string& s, FILE* file) override;

    ~UnsyncLyricsFrame() override;

private:
    char encoding[1];
    char language[3];
    std::string content_descriptor;
    char* lyrics;
};

class SyncLyricsFrame : public ID3v2Frame {
public:
    void PrintFrame() override;

    SyncLyricsFrame* ReadFrame(std::string& s, FILE* file) override;

    ~SyncLyricsFrame() override;

private:
    char encoding[1];
    char language[3];
    char stamp_format[1];
    char content_type[1];
    std::string descriptor;
    char* text;
};

class CommentFrame : public ID3v2Frame {
public:
    void PrintFrame() override;

    CommentFrame* ReadFrame(std::string& s, FILE* file) override;

    ~CommentFrame() override;

private:
    char encoding[1];
    char language[3];
    std::string short_descriptor;
    char* text;
};

class RelativeVolumeAdjFrame : public ID3v2Frame {
public:
    void PrintFrame() override;

    RelativeVolumeAdjFrame* ReadFrame(std::string& s, FILE* file) override;

    ~RelativeVolumeAdjFrame() override;

private:
    char* data;
};

class EquFrame : public ID3v2Frame {
public:
    void PrintFrame() override;

    EquFrame* ReadFrame(std::string& s, FILE* file) override;

    ~EquFrame() override;

private:
    char interpolation[1];
    std::string identification;
    char* data;
};

class PlayCounterFrame : public ID3v2Frame {
public:
    void PrintFrame() override;

    PlayCounterFrame* ReadFrame(std::string& s, FILE* file) override;

    ~PlayCounterFrame() override;

private:
    char* counter;
};

class PopularimeterFrame : public ID3v2Frame {
public:
    void PrintFrame() override;

    PopularimeterFrame* ReadFrame(std::string& s, FILE* file) override;

    ~PopularimeterFrame() override;

private:
    std::string email;
    unsigned char rating[1];
    char* counter;
};

class BuffSizeFrame : public ID3v2Frame {
public:
    void PrintFrame() override;

    BuffSizeFrame* ReadFrame(std::string& s, FILE* file) override;

    ~BuffSizeFrame() override;

private:
    char buff_size[3];
    char flag[1];
    char offset[4];
};

class LinkInfoFrame : public ID3v2Frame {
public:
    void PrintFrame() override;

    LinkInfoFrame* ReadFrame(std::string& s, FILE* file) override;

    ~LinkInfoFrame() override;

private:
    char frame_id[4];
    std::string url;
    char* data;
};

class PosSyncFrame : public ID3v2Frame {
public:
    void PrintFrame() override;

    PosSyncFrame* ReadFrame(std::string& s, FILE* file) override;

    ~PosSyncFrame() override;

private:
    char stamp_format[1];
    char* position;
};

class TermsOfUseFrame : public ID3v2Frame {
public:
    void PrintFrame() override;

    TermsOfUseFrame* ReadFrame(std::string& s, FILE* file) override;

    ~TermsOfUseFrame() override;

private:
    char encoding[1];
    char language[3];
    char* text;
};

class OwnershipFrame : public ID3v2Frame {
public:
    void PrintFrame() override;

    OwnershipFrame* ReadFrame(std::string& s, FILE* file) override;

    ~OwnershipFrame() override;

private:
    char encoding[1];
    std::string price_paid;
    char date[8];
    char* info;

};

class CommercialFrame : public ID3v2Frame {
public:
    void PrintFrame() override;

    CommercialFrame* ReadFrame(std::string& s, FILE* file) override;

    ~CommercialFrame() override;

private:
    char encoding[1];
    char* info;
};

class EncryptionMethodRegFrame : public ID3v2Frame {
public:
    void PrintFrame() override;

    EncryptionMethodRegFrame* ReadFrame(std::string& s, FILE* file) override;

    ~EncryptionMethodRegFrame() override;

private:
    std::string owner_id;
    char method[1];
    char* data;
};

class GroupIdRegFrame : public ID3v2Frame {
public:
    void PrintFrame() override;

    GroupIdRegFrame* ReadFrame(std::string& s, FILE* file) override;

    ~GroupIdRegFrame() override;

private:
    std::string owner_id;
    char group[1];
    char* data;
};

class PrivateFrame : public ID3v2Frame {
public:
    void PrintFrame() override;

    PrivateFrame* ReadFrame(std::string& s, FILE* file) override;

    ~PrivateFrame() override;

private:
    std::string owner_id;
    char* private_data;
};

class SeekFrame : public ID3v2Frame {
public:
    void PrintFrame() override;

    SeekFrame* ReadFrame(std::string& s, FILE* file) override;

    ~SeekFrame() override;

private:
    char min_offset[4];
};

std::vector<ID3v2Frame*> ParseFrames(FILE* file, int tag_size);

void PrintChars(char* s, int size);

int CountFrameSize(const char* size);