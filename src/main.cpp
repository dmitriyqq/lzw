#include <iostream>
#include <unordered_map>
#include "FileManager.hpp"

void print_usage() {
    printf("lzw encode/decode {source} {destination}");
}

// usage: 
// 
// 0: program name
// 1: encode/decode
// 2: source
// 3: destination
int main(int argc, char** argv) {
    if (argc != 4) {
        print_usage();
        exit(1);
    }

    // Decide encode or decode
    char *op = argv[1];

    char *input = argv[2];
    char *output = argv[3];  

    FileManager manager;

    if (std::string(op) == "encode") {
        bool result = manager.encode(input, output);
        exit(!result);
    } else if (std::string(op) == "decode") {
        bool result = manager.decode(input, output);
        exit(!result);
    } else {
        print_usage();
        exit(1);
    }
}