#include "greatest.h"
#include "stdio.h"
#include "../src/Dictionary.hpp"

TEST dictionary_should_be_initialized(void) {
    Dictionary dictionary;

    const char* data_ptr = "abc";
    uint32_t code;
    bool result;

    size_t size_before = dictionary.size();

    result = dictionary.findOrInsert(data_ptr, 0, 1, code);
    ASSERT_FALSE(result);
    ASSERT_EQ(0, code);

    result = dictionary.findOrInsert(data_ptr, 1, 1, code);
    ASSERT_FALSE(result);
    ASSERT_EQ(2, code);

    result = dictionary.findOrInsert(data_ptr, 2, 1, code);
    ASSERT_FALSE(result);
    ASSERT_EQ(1, code);

    ASSERT_EQm("sizes are not equal", size_before, dictionary.size());

    PASS();
}

TEST dictionary_should_add_new_sequence(void) {
    int x = 1;
    /* Compare, with an automatic "1 != x" failure message */
    ASSERT_EQ(1, x);

    /* Compare, with a custom failure message */
    ASSERT_EQm("Yikes, x doesn't equal 1", 1, x);

    /* Compare, and if they differ, print both values,
     * formatted like `printf("Expected: %d\nGot: %d\n", 1, x);` */
    ASSERT_EQ_FMT(1, x, "%d");
    
    PASS();
}

TEST dictionary_shoud_return_code_and_dont_add(void) {
    PASS();
}

TEST dictionary_should_find(void) {
    PASS();
}

/* Suites can group multiple tests with common setup. */
SUITE(DictionaryTests) {
    RUN_TEST(dictionary_should_be_initialized);
    RUN_TEST(dictionary_should_add_new_sequence);
    RUN_TEST(dictionary_shoud_return_code_and_dont_add);
    RUN_TEST(dictionary_should_find);
}

SUITE(EncoderTests) {
    RUN_TEST(dictionary_should_be_initialized);
    RUN_TEST(dictionary_should_add_new_sequence);
    RUN_TEST(dictionary_shoud_return_code_and_dont_add);
    RUN_TEST(dictionary_should_find);
}


/* Add definitions that need to be in the test runner's main file. */
GREATEST_MAIN_DEFS();

int main(int argc, char **argv) {
    GREATEST_MAIN_BEGIN();      /* command-line options, initialization. */

    /* Tests can also be gathered into test suites. */
    RUN_SUITE(DictionaryTests);
    RUN_SUITE(EncoderTests);

    GREATEST_MAIN_END();        /* display results */
}