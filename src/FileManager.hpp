#pragma once

#include <string>
#include <fstream>
#include <filesystem>
#include <vector>

#include "Encoder.hpp"
#include "Decoder.hpp"

class Packager {
    char numberOfBits;
    static constexpr uint8_t bitsInInt = 32;
public:
    Packager(char _numberOfBits) : numberOfBits(_numberOfBits) {
    }

    void writeToInt(uint32_t &a, uint32_t &b, uint32_t x, char &usedBits) {
        if (usedBits + numberOfBits <= bitsInInt) {
            // can safely write x to a;
            a |= x << usedBits; 
            usedBits += numberOfBits;
        } else {
            // we can't write whole x, we need to split it between a and b
            char bitsAvailable = bitsInInt - usedBits;
            a |= x << usedBits;
            b |= x >> bitsAvailable;
            usedBits = numberOfBits - bitsAvailable;
        }
    }

    void writeBits(const vector<uint32_t> &in, vector<uint32_t> &out) {
        uint32_t number = 0;
        uint32_t a = 0, b = 0;
        char usedBits = 0;

        for (const uint32_t &x: in) {
            writeToInt(a, b, x, usedBits);
            if (usedBits == 32) {
                // we have whole 4 bytes to write then reset everything
                out.push_back(a);
                a = b = usedBits = 0;
            } else if (b != 0) {
                // we have 4 bytes ready to write then move b -> a
                out.push_back(a);
                a = b;
                b = 0;
            }
        }
        if (a != 0) {
            out.push_back(a);
        }
    }
};

class FileManager {
    static constexpr uint32_t BLOCK_SIZE = 1 << 10;
    char buffer[BLOCK_SIZE];
public:
    bool encode(std::string input_path, std::string output_path) {
        auto file_size = std::filesystem::file_size(input_path);
        std::cout << "input file size: " << file_size << " bytes" << std::endl;

        std::ifstream in_file(input_path, std::ios_base::binary);
        std::ofstream out_file(output_path, std::ios_base::binary);
        if (!in_file || !out_file) {
            return false;
        }

        Encoder encoder;
        uintmax_t read = 0;
        uint32_t number_of_blocks = 0;

        // leave 4 bytes at the beggining of the file to count number of blocks;
        out_file.write((char*)&number_of_blocks, 4);
        out_file.write((char*)&BLOCK_SIZE, 4);

        while (read != file_size) {
            number_of_blocks++;
            // std::cout << "BLOCK_NUMBER: " << number_of_blocks << std::endl;

            uint32_t to_read = (read + BLOCK_SIZE) < file_size ? BLOCK_SIZE : file_size - read;

            // std::cout << "to read " << to_read << std::endl;

            in_file.read(buffer, to_read);
            read += to_read;

            // std::cout << "READ" << std::endl;

            std::vector<uint32_t> coded;
            encoder.encode(buffer, to_read, coded);

            // std::cout << "ENCODED" << std::endl;

            uint32_t max_code = 0;
            for (const uint32_t &code: coded) {
                max_code = std::max(max_code, code);
            }

            // std::cout << "MAX_CODE" << max_code << std::endl;

            char number_of_bits = CHAR_BIT;
            uint32_t code = UCHAR_MAX;
            while (code < max_code) { 
                code <<= 1;
                number_of_bits++; 
            }

            // std::cout << "NUMBER_OF_BITS " << (int)number_of_bits << std::endl;

            Packager packager(number_of_bits);
            std::vector<uint32_t> out;
            packager.writeBits(coded, out);
            out_file.write(&number_of_bits, 1);
            out_file.write((char*)&out[0], out.size() * sizeof(out[0]));
        }

        out_file.seekp(0, std::ios::beg);
        // rewrite number of blocks
        out_file.write((char*)&number_of_blocks, 4);
        out_file.close();
        in_file.close();
        return true;
    }

    bool decode(std::string input_path, std::string output_path) {
        return true;
    }
};