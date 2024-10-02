#include <stdio.h>
#include <stdbool.h>
#include "Utility.h"

#define BOARD_SIZE 32 //Squares 0-31
#define BLACK 0
#define WHITE 1

unsigned int FlipBit(unsigned int value, int position){
    return value ^ (1 << position);
}

unsigned int SetBit(unsigned int value, int position){
    return value | (1 << position);
}

unsigned int ClearBit(unsigned int value, int position){
    //ClearBit(5,2); 5=0101
    //0001 << 2 == 0100
    //~(0100) = 1011
    //0101 = 1011
    //0001
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

void ToBinaryString(unsigned int value, char* output){
    for(int i = 31; i >= 0; i--){
        output[31-i] = ((value >> i) & 1) ? '1' : '0';
    }
    output[32] = '\0';
}

void ToHexString(unsigned int value, char* output){
    sprintf(output, "%08x", value);
}

/*
  28  29  30  31
24  25  26  27
  20  21  22  23
16  17  18  19
  12  13  14  15
08  09  10  11
  04  05  06  07
00  01  02  03
*/

//DONE
void InitializeBoard(unsigned int* blackRegular, unsigned int* blackKings, unsigned int* whiteRegular, unsigned int* whiteKings) {
     // Black regular pieces (top 3 rows): bits 20 to 31
    *blackRegular = 0x00000FFF;  //(last 12 bits set to 1)
    *blackKings = 0x00000000;    // No black kings initially

    // White regular pieces (bottom 3 rows): bits 0 to 11
    *whiteRegular = 0xFFF00000;  // (first 12 bits set to 1)
    *whiteKings = 0x00000000;    // No white kings initially
}
//DONE, Method to determine if the piece is a king
int IsKingPiece(unsigned int* kingBoard, int position){
    return CheckBit(*kingBoard, position);
}
//Need to fix end pieces and implement jump captures
int IsLegalMove(unsigned int* p1Board, unsigned int* p2Board, unsigned int* p1KingBoard, unsigned int* p2KingBoard, int start, int end, int turn) {

    //Check if end position is in range of the board
    if(end < 0 || end > BOARD_SIZE) return 0;
    //Check if start has a piece
    if(!CheckBit(*p1Board,start) & !CheckBit(*p1KingBoard,start)) return 0;
    //Check if end position is blocked by player1's own piece
    if(!CheckBit(*p1Board, end) & !CheckBit(*p1KingBoard, end)) return 0;

    int startRow = start/4;
    int endRow = end/4;
    int isBlack = (turn % 2) == 0 ? BLACK : WHITE;
    int isKing = IsKingPiece(*p1KingBoard, start) ? 1 : 0;
    int rowDisplacement = endRow-startRow;
    int displacement = end-start;

    //Generate possible move set for black turn, negate move set if white turn
    int moveSet[4];
    if(isKing && startRow % 2 == 0){
        moveSet[0] = -5;
        moveSet[1] = -4;
        moveSet[2] = 3;
        moveSet[3] = 4;
    }
    else if(isKing && startRow % 2 == 1){
        moveSet[0] = -4;
        moveSet[1] = -3;
        moveSet[2] = 4;
        moveSet[3] = 5;
    }
    else if(!isKing && startRow % 2 == 0){
        moveSet[0] = isBlack ? 3 : -3;
        moveSet[1] = isBlack ? 4 : -4;
    }
    else{
        moveSet[0] = isBlack ? 4 : -4;
        moveSet[1] = isBlack ? 5 : -5;
    }
    
    //If rowDisplacement = 2, check capture move
    //If rowDisplacement = 1, check normal move
    //Else, invalid move
    //Handle capture moves
    if (abs(rowDisplacement) == 2) {
        //Generate possible jump move set, values are all the same no matter side or row
        int jumpMoveSet[] = {-9,-7,7,9};
        for (int i = 0; i < (isKing ? 4 : 2); i++) {
            if (displacement == jumpMoveSet[i]) {
                int mid = start + (jumpMoveSet[i]/2); // Mid-position for capturing
                //Check if mid-position has an opponent piece
                if (CheckBit(*p2Board, mid) || CheckBit(*p2KingBoard, mid)) {
                    //Capture the opponent's piece
                    CapturePiece(IsKingPiece(*p2KingBoard, mid) ? *p2KingBoard : *p2Board, mid);
                    //Move the piece
                    MovePiece(p1Board, start, end);
                    //Promote to king if necessary
                    if (!isKing && ((isBlack && end > 27) || (!isBlack && end < 4))) {
                        promoteKing(p1Board, p1KingBoard, end);
                    }
                    return 1; //Valid capture move
                }
            }
        }
    }

    //Handle regular moves
    if (abs(rowDisplacement) == 1) {
        for (int i = 0; i < (isKing ? 4 : 2); i++) {
            if (displacement == moveSet[i]) {
                //Check if end position is open
                if (!(CheckBit(*p2Board, end) || CheckBit(*p2KingBoard, end))) {
                    MovePiece(p1Board, start, end);
                    return 1; //Valid move
                }
            }
        }
    }

    //rowDisplacement invalid, displacement invalid, cannot capture opponent
    return 0;
}
//DONE
void MovePiece(unsigned int* board, int start, int end) {
          //Clear bit at starting position
          *board = ClearBit(*board, start);
          //Flip bit at position moved to 
          *board = FlipBit(*board, end);
}
//DONE
void promoteKing(int* piecesBoard, int* kingBoard, int position){
    //Clears piece from pieces board
    *piecesBoard = ClearBit(*piecesBoard, position);
    //Sets king piece in king board
    *kingBoard = SetBit(*kingBoard, position);
}

//DONE
void CapturePiece(unsigned int* board, int position) {
    //Clears bit of opponents board at capture position
     *board = ClearBit(board, position);
}

//DONE, changed to work with number instead of array
void PrintBoard(unsigned int board) { 
     // Start from the highest bit (bit 31) and work down to bit 0
    for (int row = 7; row >= 0; row--) {
        for (int col = 0; col < 4; col++) {
            // Calculate the bit index for this "checkers board" layout
            int bitIndex = row * 4 + col;
            // Print the bit value for each position
            printf("%d ", (board >> bitIndex) & 1);
        }
        printf("\n");  // New line after each row
    }
}

void UpdateGameState(unsigned int* board, int currentPlayer) { 
     // Implement logic to update the game state after each move 
     // Switch turns, check for game-ending conditions, etc.
     int turn = 0;
     bool winner = false;

     while(!winner){
          //Black turn
          if(turn%2 == 0){
               printf("Player 1's turn.\n");

          }
          //white turn
          else{
               printf("Player 2's turn.\n");
          }
          turn++;
     }
}

int main(){
     unsigned int p1Board, p1KingBoard = 0; //Black
     unsigned int p2Board, p2KingBoard = 0; //White

     InitializeBoard(&p1Board, &p1KingBoard, &p2Board, &p2KingBoard);
     printf("Black Pieces:\n");
     PrintBoard(p1Board);
     MovePiece(&p1Board,11,15);
     PrintBoard(p1Board);
     return 0;
}