#include <stdio.h>
#include <stdlib.h>
#include <string_view>
#include <iostream>
#include <unordered_map>

int print_file_head_hex() {
    
}

int calculate_file_entropy() {

}

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
    // See CLI arguments
    // for (int i = 0; i < argc; i++) {
    //     printf("%d: %s\n", i, argv[i]);
    // }

    // std::string str = std::string("123123123");
    // char* cstr = (char*) "123123123aaa";
    // std::string_view view(cstr, 9);
    // std::cout << "==" << (str == view) << std::endl;

    // std::unordered_map<std::string, int> m;
    // m[str] = 1;
    // std::cout << "find" << (m.find(view) != m.end()) << std::endl;


    // Decide encode or decode
    if (argc != 4) {
        print_usage();
        exit(1);
    }

    char *op = argv[1];
    char *input = argv[2];
    char *output = argv[3];  
}