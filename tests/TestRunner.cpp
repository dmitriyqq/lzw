#include "greatest.h"
#include <cstdio>
#include <filesystem>
#include <iostream>

#include "../src/EncoderDictionary.hpp"
#include "../src/Encoder.hpp"
#include "../src/Decoder.hpp"
#include "../src/FileManager.hpp"

TEST dictionary_should_be_initialized(void) {
    EncoderDictionary dictionary;

    const char* data_ptr = "abc";
    uint32_t code;
    bool result;

    size_t size_before = dictionary.size();

    result = dictionary.findOrInsert(data_ptr, 0, 1, code);
    ASSERT_FALSE(result);
    dictionary.find(data_ptr, 0, 1, code);
    ASSERT_EQ('a', code);

    result = dictionary.findOrInsert(data_ptr, 1, 1, code);
    ASSERT_FALSE(result);
    dictionary.find(data_ptr, 1, 1, code);
    ASSERT_EQ('b', code);

    result = dictionary.findOrInsert(data_ptr, 2, 1, code);
    ASSERT_FALSE(result);
    dictionary.find(data_ptr, 2, 1, code);
    ASSERT_EQ('c', code);

    ASSERT_EQm("sizes are not equal", size_before, dictionary.size());

    PASS();
}

TEST dictionary_should_add_new_sequence(void) {
    EncoderDictionary dictionary;

    const char* data_ptr = "abc";
    uint32_t code;
    bool result;

    size_t size_before = dictionary.size();

    result = dictionary.findOrInsert(data_ptr, 0, 2, code);
    ASSERT(result);
    ASSERT_EQ('a', code);

    uint32_t inserted_code;
    result = dictionary.find(data_ptr, 0, 2, inserted_code);
    ASSERT(result);

    result = dictionary.findOrInsert(data_ptr, 0, 3, code);
    ASSERT(result);
    ASSERT_EQ(inserted_code, code);

    result = dictionary.findOrInsert(data_ptr, 1, 2, code);
    ASSERT(result);
    ASSERT_EQ('b', code);

    ASSERT_EQm("3 items should be added", size_before + 3, dictionary.size());
    PASS();
}

TEST dictionary_shoud_return_code_and_dont_add(void) {
    EncoderDictionary dictionary;

    const char* data_ptr = "zxzxczxcdzx";
    uint32_t code;
    bool result;
    size_t size_before = dictionary.size();

    result = dictionary.findOrInsert(data_ptr, 0, 1, code);
    ASSERT_FALSE(result);
    result = dictionary.findOrInsert(data_ptr, 0, 2, code);
    ASSERT(result);

    result = dictionary.findOrInsert(data_ptr, 2, 1, code);
    ASSERT_FALSE(result);
    result = dictionary.findOrInsert(data_ptr, 2, 2, code);
    ASSERT_FALSE(result);
    result = dictionary.findOrInsert(data_ptr, 2, 3, code);
    ASSERT(result);

    result = dictionary.findOrInsert(data_ptr, 5, 1, code);
    ASSERT_FALSE(result);
    result = dictionary.findOrInsert(data_ptr, 5, 2, code);
    ASSERT_FALSE(result);
    result = dictionary.findOrInsert(data_ptr, 5, 3, code);
    ASSERT_FALSE(result);
    result = dictionary.findOrInsert(data_ptr, 5, 4, code);
    ASSERT(result);

    ASSERT_EQm("3 items should be added", size_before + 3, dictionary.size());

    PASS();
}

TEST encode_seq1(void) {
    Encoder encoder;

    const char *data = "nmgmghnmgmmgh";
    std::vector<uint32_t> out;

    encoder.encode(data, strlen(data), out);
    ASSERT_EQ(8, out.size());
    ASSERT_EQ('n', out[0]);
    ASSERT_EQ('m', out[1]);
    ASSERT_EQ('g', out[2]);
    ASSERT_EQ(257, out[3]); // mg
    ASSERT_EQ('h', out[4]); 
    ASSERT_EQ(256, out[5]); // nm
    ASSERT_EQ(258, out[6]); 
    ASSERT_EQ(259, out[7]); // mgh

    PASS();
}

TEST encode_seq2(void) {
    PASS();
}

TEST encode_seq3(void) {
    PASS();
}

TEST decode_seq1(void) {

    const int data[] = {'n', 'm', 'g', 257, 'h', 256, 258, 259};
    std::string out;
    Decoder decoder;
    decoder.decode(data, 8, out);
    ASSERT_EQ("nmgmghnmgmmgh", out);
    PASS();
}

TEST decode_seq2(void) {
    PASS();
}

TEST decode_seq3(void) {
    PASS();
}

char * readFileToBuffer(const std::string &input_path) {
    auto input_size = std::filesystem::file_size(input_path);
    char *buf = new char[input_size];
    std::ifstream in(input_path);
    in.read(buf, input_size);
    in.close();
    return buf;
}

bool equals(const char* buf1, const char* buf2, int size) {
    for (int i = 0; i < size; i++) {
        if (buf1[i] != buf2[i] ) {
            return false;
        }
    }

    return true;
}

bool testFile(const std::string &input_path) {
    try {
        std::filesystem::remove("compressed");
        std::filesystem::remove("out");

        FileManager manager;

        manager.encode(input_path, "compressed");
        manager.decode("compressed", "out");

        auto input_size = std::filesystem::file_size(input_path);
        auto out_size = std::filesystem::file_size("out");

        if (input_size != out_size) {
            return false;
        }

        char *in = readFileToBuffer(input_path);
        char *out = readFileToBuffer("out");
        
        PASS();

        delete[] in;
        delete[] out;

        return equals(in, out, input_size);
    } catch (std::exception e){
        std::cout << e.what() << std::endl;
        return false;
    }
}

TEST file_test_en(void) {
    ASSERT(testFile("in_en.txt"));
    PASS();
}

TEST file_test_rus(void) {
    ASSERT(testFile("in_rus.txt"));
    PASS();
}

TEST file_test_binary(void) {
    ASSERT(testFile("in_binary"));
    PASS();
}

/* Suites can group multiple tests with common setup. */
SUITE(DictionaryTests) {
    RUN_TEST(dictionary_should_be_initialized);
    RUN_TEST(dictionary_should_add_new_sequence);
    RUN_TEST(dictionary_shoud_return_code_and_dont_add);
}

SUITE(EncoderTests) {
    RUN_TEST(encode_seq1);
    RUN_TEST(encode_seq2);
    RUN_TEST(encode_seq3);
}

SUITE(DecoderTests) {
    RUN_TEST(decode_seq1);
    RUN_TEST(decode_seq2);
    RUN_TEST(decode_seq3);
}

SUITE(FileManagerTests) {
    RUN_TEST(file_test_en);
    RUN_TEST(file_test_rus);
    RUN_TEST(file_test_binary);
}

/* Add definitions that need to be in the test runner's main file. */
GREATEST_MAIN_DEFS();

int main(int argc, char **argv) {
    GREATEST_MAIN_BEGIN();      /* command-line options, initialization. */

    /* Tests can also be gathered into test suites. */
    RUN_SUITE(DictionaryTests);
    RUN_SUITE(EncoderTests);
    RUN_SUITE(DecoderTests);
    RUN_SUITE(FileManagerTests);

    GREATEST_MAIN_END();        /* display results */
}