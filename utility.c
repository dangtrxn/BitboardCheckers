#include <stdio.h>
#include "utility.h"

//Utility Class, defining methods
//Methods----------------------------------------------------
//Position indexing starts at 0 from LSB
unsigned int FlipBit(unsigned int value, int position){
    return value ^ (1 << position);
}

unsigned int SetBit(unsigned int value, int position){
    return value | (1 << position);
}

unsigned int ClearBit(unsigned int value, int position){
    return value & ~(1 << position);
}

unsigned int ToggleBit(unsigned int value, int position){
    return value ^ (1 << position);
}

int CheckBit(unsigned int value, int position){
    return (value >> position) & 1;
}

void PrintBinary(unsigned int value){ 
    for (int i = 31; i >= 0; i--){ 
        printf("%d", (value >> i) & 1); 
        if (i % 4 == 0) printf(" "); 
    } 
    printf("\n"); 
}

unsigned int ShiftLeft(unsigned int value, int positions){
    return value << positions;
}

unsigned int ShiftRight(unsigned int value, int positions){
    return value >> positions;
}

unsigned int CreateBitMask(int start, int end){
    return ((1 << (end - start + 1)) - 1) << start;
}

void ToBinaryString(unsigned int value, char* output) {
     for (int i = 31; i >= 0; i--) {
         output[31-i] = ((value >> i) & 1) ? '1' : '0'; 
    }
     output[32] = '\0'; 
}

void ToHexString(unsigned int value, char* output) {
     sprintf(output, "%08X", value); 
}