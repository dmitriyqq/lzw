#pragma once

#include <string>
#include <fstream>
#include <filesystem>
#include <vector>

#include "Encoder.hpp"
#include "Decoder.hpp"

class BlockMetadata {
    uint8_t numberOfBits;
    uint32_t blockNumber;
}

class Packager {
    char numberOfBits;
    constexpr char bitsInInt = 32;

    void writeToInt(uint32_t &a, uint32_t &b, uint32_t x, char &usedBits) {
        if (usedBits + numberOfBits <= bitsInInt) {
            // can safely write x to a;
            usedBits += numberOfBits;
            a |= x << numberOfBits; 
        } else {
            // we can't write whole x, we need to split it between a and b
            char bitsAvailable = bitsInInt - usedBits;
            a |= x << usedBits;
            b |= x >> bitsAvailable;
            usedBits = bitsInInt - numberOfBits + bitsAvailable;
        }
    }

public:
    Packager(char _numberOfBits) : numberOfBits(_numberOfBits) {
    }

    bool writeBits(const vector<uint32_t> &in, const vector<uint32_t> &out, char numberOfBits, uint32_t blockNumber) {
        uint32_t number = 0;
        uint32_t a = 0, b = 0;
        char usedBits = 

        for (const uint32_t &x: in) {
            writeToInt(a, b, x, usedBits)
        }
    }
}

class FileManager {
    static constexpr uint32_t READ_BLOCK_SIZE = 1 << 16;
public:
    bool encode(std::string input_path, std::string output_path) {
        auto file_size = std::filesystem::file_size(input_path);
        std::cout << "input file size: " << file_size << " bytes" << std::endl;

        std::ifstream in(input_path);
        if (!in) {
            return false;
        }

        Encoder encoder;
        Packager packager;
        uintmax_t read = 0;
        uint32_t number_of_blocks = 0;

        while (read != file_size) {
            number_of_blocks++;
            char buffer[READ_BLOCK_SIZE];

            uint32_t to_read = (read + READ_BLOCK_SIZE) < file_size ? READ_BLOCK_SIZE : file_size - read;

            in.read(buffer, to_read);

            std::vector<uint32_t> out;
            encoder.encode(buffer, to_read, out);
            uint32_t max_code = 0;
            for (const uint32_t &code: out) {
                max_code = std::max(max_code, code);
            }

            char number_of_bits = 8;
            uint32_t code = 255;
            while (code < max_code) { 
                code << 1;
                number_of_bits++; 
            }

        }

        in.close();
    }

    bool decode(std::string input_path, std::string output_path) {

    }
};