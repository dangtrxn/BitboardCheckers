#ifndef UTILITY_H
#define UTILITY_H

//Header file, declaring utility methods
unsigned int FlipBit(unsigned int value, int position);

unsigned int SetBit(unsigned int value, int position);

unsigned int ClearBit(unsigned int value, int position);

unsigned int ToggleBit(unsigned int value, int position);

int CheckBit(unsigned int value, int position);

void PrintBinary(unsigned int value);

unsigned int ShiftLeft(unsigned int value, int positions);

unsigned int ShiftRight(unsigned int value, int positions);

unsigned int CreateBitMask(int start, int end);

void ToBinaryString(unsigned int value, char* output);

void ToHexString(unsigned int value, char* output);

#endif