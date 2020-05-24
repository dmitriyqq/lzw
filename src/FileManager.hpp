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
};

class Packager {
    char numberOfBits;
    static constexpr char bitsInInt = 32;
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

    bool writeBits(const vector<uint32_t> &in, vector<uint32_t> &out, char numberOfBits, uint32_t blockNumber) {
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
            }
        }
    }
};

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