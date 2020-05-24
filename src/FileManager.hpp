#pragma once

#include <string>
#include <fstream>
#include <filesystem>
#include <vector>
#include <chrono>

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

class PerformanceClock {
    uintmax_t &m_time;
    std::chrono::high_resolution_clock::time_point m_start;

    public:
        PerformanceClock(uintmax_t &time): m_time(time) {
            m_start = std::chrono::high_resolution_clock::now();
        }

        ~PerformanceClock() {
            std::chrono::high_resolution_clock::time_point end = std::chrono::high_resolution_clock::now();
            m_time += std::chrono::duration_cast<std::chrono::microseconds>(end - m_start).count();
        }
};

class FileManager {
    static constexpr uint32_t BLOCK_SIZE = 1 << 10;
    static constexpr double DISPLAY_INTERVAL = 5.0;
    static constexpr uint32_t MAX_DICT_SIZE = 10000000;

    char buffer[BLOCK_SIZE];
public:
    bool encode(std::string input_path, std::string output_path) {
        auto file_size = std::filesystem::file_size(input_path);
        
        std::cout << "encoding " << input_path << " in " << output_path << std::endl;
        std::cout << "input file size: " << file_size << " bytes" << std::endl;
        
        
        std::ifstream in_file(input_path, std::ios_base::binary);
        std::ofstream out_file(output_path, std::ios_base::binary);
        
        if (!in_file || !out_file) {
            std::cout << "file stream error" << std::endl;
            return false;
        }

        // pass some expectations about dictionary size to encoder
        // this is due to reduce resizing of hash table and vector
        Encoder encoder(MAX_DICT_SIZE + BLOCK_SIZE);
        uintmax_t read = 0;
        uint32_t number_of_blocks = 0;

        auto time_start = std::chrono::high_resolution_clock::now();

        // leave 4 bytes at the beggining of the file to count number of blocks;
        out_file.write((char*)&number_of_blocks, 4);
        out_file.write((char*)&BLOCK_SIZE, 4);

        uintmax_t time_read = 0, time_write = 0, time_encode = 0, time_pack = 0;

        double last_displayed_value = 0.0;
        while (read != file_size) {
            
            number_of_blocks++;
            uint32_t to_read = (read + BLOCK_SIZE) < file_size ? BLOCK_SIZE : file_size - read;
            {
                PerformanceClock clock(time_read);
                in_file.read(buffer, to_read);
                read += to_read;
            }

            double progress = (read / (double) file_size) * 100.0;
            if (progress > last_displayed_value + DISPLAY_INTERVAL || last_displayed_value == 0) {
                std::cout << std::setprecision(4) << progress << "% dictionary size: " << encoder.dictSize() << " entries" << std::endl;
                last_displayed_value = progress;
            }

            std::vector<uint32_t> coded;
            uint32_t max_code = 0;
            char number_of_bits = CHAR_BIT;
            uint32_t code = UCHAR_MAX;
            {
                PerformanceClock clock(time_encode);
                encoder.encode(buffer, to_read, coded);
                
                for (const uint32_t &code: coded) {
                    max_code = std::max(max_code, code);
                }

                while (code < max_code) { 
                    code <<= 1;
                    number_of_bits++; 
                }
            }

            if (encoder.dictSize() >= MAX_DICT_SIZE) {
                encoder.resetDict();
            }

            Packager packager(number_of_bits);
            std::vector<uint32_t> out;
            {
                PerformanceClock clock(time_pack);
                packager.writeBits(coded, out);
            }

            {
                PerformanceClock clock(time_write);
                out_file.write(&number_of_bits, 1);
                out_file.write((char*)&out[0], out.size() * sizeof(out[0]));
            }
        }

        // rewrite number of blocks
        out_file.seekp(0, std::ios::beg);
        out_file.write((char*)&number_of_blocks, 4);
        out_file.close();
        in_file.close();

        auto time_finish = std::chrono::high_resolution_clock::now();
        auto total_time = std::chrono::duration_cast<std::chrono::milliseconds>(time_finish - time_start).count();        

        std::cout <<  std::setprecision(6) << "done " << total_time / 1000.0 << "s." << std::endl;

        std::cout << "time to read: " << time_read / 1000000.0 << "s." << std::endl;
        std::cout << "time to write: " << time_write / 1000000.0 << "s." << std::endl;
        std::cout << "time to encode: " << time_encode / 1000000.0 << "s." << std::endl;
        std::cout << "time to pack: " << time_pack / 1000000.0 << "s." << std::endl;

        auto out_file_size = std::filesystem::file_size(output_path);

        std::cout << "out file size: " << out_file_size << std::endl;
        std::cout << "data compression ratio: " << ((double) file_size / (double) out_file_size) << std::endl;

        

        return true;
    }

    bool decode(std::string input_path, std::string output_path) {
        return true;
    }
};