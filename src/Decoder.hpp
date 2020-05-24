#pragma once

#include <vector>
#include <string>
#include "EncoderDictionary.hpp"

using std::vector;
using std::string;

class Decoder {
    // Dictionary dictionary;
    vector<char> output;
public:
    void decode(const int* const data, size_t data_size, string &output) {
        // decoding part
        int coder = 255 + 1;
        string current = "";
        output = "";

        vector<string> dict;

        for (int i = 0; i <= 255; i++) {
            string str = "";
            str += char(i);
            dict.push_back(str);
        }

        for (int i = 0; i < data_size; i++)
        {
            current += dict[data[i]];
            output += current;

            // the first half of the decoding string will be 'current'
            // Library newLib = Library(coder, current);
            dict.push_back(current);

            // the second half will be the first char of the next lib
            if (i < data_size - 1) {
                dict[coder] += dict[data[i + 1]][0];
            }

            //cout << "---------------------------"<<endl;
            coder++; // next library
            current = "";
        }
    }
};