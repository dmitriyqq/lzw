#pragma once

#include <vector>
#include <string>
#include "EncoderDictionary.hpp"

using std::vector;
using std::string;

class Encoder {
    EncoderDictionary dictionary;
public:
    Encoder(size_t _data_size = 0) {

    }

    void resetDict() {
        dictionary.reset();
    }

    // To Do: don't use vector push back as output use buffer instead
    void encode(const char* const data_ptr, size_t data_size, vector<uint32_t> &output) {
        char *ptr = (char *) data_ptr;
        char *end = (char *)data_ptr + data_size;
        while(ptr < end) {
            output.push_back(dictionary.insertNewCode(ptr, end - ptr));
        }
    }

    uint32_t dictSize() const {
        return dictionary.size();
    }
};