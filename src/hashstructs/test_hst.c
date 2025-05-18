#include <stdint.h>
#include <stdio.h>
#include "hashset.h"
#include <string.h>

uint64_t hash16digits(const char *str) {
    uint64_t t = 0;
    for(int i = 0; i < 16; i++) {
        t <<= 4;
        t |= str[i] - '0';
    }
    return t;
}

char* digitsfromhash(uint64_t h) {
    char *str = (char*)malloc(17);
    for(int i = 0; i < 16; i++) {
        str[15 - i] = (h & 0xF) + '0';
        h >>= 4;
    }
    str[16] = '\0';
    return str;
}

int main() {
    const char *input = "2912551968550732";
    uint64_t h = hash16digits(input);
    printf("Hash: %08x\n", h);

    printf("Digits from hash: %s, %d\n", digitsfromhash(h), strcmp(digitsfromhash(h), input));
    return 0;
}
