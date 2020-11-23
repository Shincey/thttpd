#ifndef __BUFFER_H__
#define __BUFFER_H__

#include <stdint.h>
#include "tassert.h"
#include "tools.h"

class buffer {
public:
    explicit buffer(const uint32_t __size) : size_(__size), in_(0), out_(0) {
        tassert(__size >= 0, "parameter passed shoud be bigger than zero"); // not nesscary
        data_ = new char[__size];
    }

    ~buffer() {
        delete[] data_;
    }

    inline bool in(const void *__data, const uint32_t __size) {

        if (size_ - length() < __size) { return false; }

        if (size_ - in_ < __size) {
            if (in_ == out_) {
                in_ = 0;
                out_ = 0;
            } else {
                tool::smemcpy(data_, size_, data_ + out_, in_ - out_);
                in_ -= out_;
                out_ = 0;
            }
        }

        tassert(in_ < size_ && out_ < size_, "something wrong here");
        tool::smemcpy(data_ + in_, size_ - length(), __data, __size);
        in_ += __size;
        tassert(in_ <= size_, "something wrong here");

        return true;
    }

    inline bool out(const uint32_t __size) {
        tassert(out_ + __size <= in_ && in_ <= size_, "something wrong here");

        if (out_ + __size > in_) { return false; }
        
        out_ += __size;

        return true;
    }

    inline const void *data() const {
        return data_ + out_;
    }

    inline const int32_t length() const {
        tassert(in_ >= out_ && in_ <= size_, "something wrong here");
        return in_ - out_;
    }

private:
    const uint32_t size_;
    uint32_t in_;
    uint32_t out_;
    char *data_;
};


#endif // __BUFFER_H__