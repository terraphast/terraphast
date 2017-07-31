#include <sstream>
#include <iostream>

class Stream {
public:
    virtual Stream& operator<<(const char* string) = 0;
};

class FileStream : public Stream {
private:
    FILE *file;

public:
    FileStream(FILE *file) : file(file) {}

    FileStream& operator<<(const char* string) {
        fprintf(this->file, "%s", string);
        return *this;
    }
};

class StringStream : public Stream {
private:
    std::stringstream ss;

public:
    StringStream() : ss(std::stringstream()) {}

    StringStream& operator<<(const char* string) {
        this->ss << string;
        return *this;
    }

    std::string to_string() {
        return ss.str();
    }
};
