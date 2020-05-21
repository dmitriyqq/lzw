#include <stdio.h>
#include <stdlib.h>
#include <undered_map>

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

    // Decide encode or decode
    if (argc != 4) {
        print_usage();
        exit(1);
    }

    char *op = argv[1];
    char *input = argv[2];
    char *output = argv[3];  
}