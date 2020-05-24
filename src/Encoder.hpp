#pragma once

#include <vector>
#include <string>
#include "EncoderDictionary.hpp"

using std::vector;
using std::string;

class Encoder {
    EncoderDictionary dictionary;
    vector<uint32_t> output;
public:
    // To Do: don't use vector push back as output use buffer instead
    void encode(const char* const data_ptr, size_t data_size, vector<uint32_t> &output) {
        int current_sequence_start = 0;
        uint32_t code;
        for (int i = 1; i < data_size; i++) {
            if (dictionary.findOrInsert(data_ptr, current_sequence_start, i - current_sequence_start, code)) {
                i--;
                current_sequence_start = i;
                output.push_back(code);
            } 
        }

        // add the last sequence
        if (current_sequence_start + 1 != data_size) {
            dictionary.find(data_ptr, current_sequence_start, data_size - current_sequence_start, code);
            output.push_back(code);
        }
    }
};