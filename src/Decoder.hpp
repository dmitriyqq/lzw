#pragma once

#include <vector>
#include <string>
#include "DecoderDictionary.hpp"

using std::vector;
using std::string;

class Decoder {
    DecoderDictionary dictionary;
    vector<char> output;
public:
    void decode(const int* const data, size_t data_size, string &output) {
        // decoding part
        int coder = 255 + 1;
        string current = "";
        output = "";

        for (int i = 0; i < data_size; i++)
        {
            current += dictionary.get(data[i]);
            output += current;

            // the first half of the decoding string will be 'current'
            // Library newLib = Library(coder, current);
            dictionary.insert(current);

            // the second half will be the first char of the next lib
            if (i < data_size - 1) {
                uint32_t code = data[i+1];
                dictionary.update(coder, dictionary.get(code)[0]);
            }

            coder++; 
            current = "";
        }
    }
};