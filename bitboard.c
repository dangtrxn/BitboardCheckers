#include <stdio.h>
#include <stdlib.h>
#include "utility.h"

#define BOARD_SIZE 32 //Squares 0-31
#define BLACK 0
#define WHITE 1

/* Visual representation of board and position indexes
  28  29  30  31
24  25  26  27
  20  21  22  23
16  17  18  19
  12  13  14  15
08  09  10  11
  04  05  06  07
00  01  02  03
*/

void InitializeBoard(unsigned int* blackRegular, unsigned int* blackKings, unsigned int* whiteRegular, unsigned int* whiteKings) {
    *blackRegular = 0x00000FFF;  //Black pawn pieces
    *blackKings = 0x00000000;    //No black kings initially

    *whiteRegular = 0xFFF00000;  //White pawn pieces
    *whiteKings = 0x00000000;    //No white kings initially
}

int IsKingPiece(unsigned int* kingBoard, int position){
    //Helper method to determine if a piece is a king or not
    //Checks the player's king board at position, return 1 if there is a king there
    return CheckBit(*kingBoard, position);
}

void MovePiece(unsigned int* board, int start, int end) {
    //Clear bit at starting position
    *board = ClearBit(*board, start);
    //Set bit at end position
    *board = SetBit(*board, end);
}

void PromoteKing(unsigned int* pawnBoard, unsigned int* kingBoard, int position){
    //Helper method to promote a piece to a king
    //Clears piece from player's pawn board
    *pawnBoard = ClearBit(*pawnBoard, position);
    //Sets king piece in king board
    *kingBoard = SetBit(*kingBoard, position);
}

void CapturePiece(unsigned int* board, int position) {
    //Only clears piece at the position, the rest is handled in IsLegalMove
    *board = ClearBit(*board, position);
}

int IsLegalMove(unsigned int* p1Board, unsigned int* p2Board, unsigned int* p1KingBoard, unsigned int* p2KingBoard, int start, int end, int turn) {

    //Check if start/end position is in range of the board (0-31)
    if(start < 0 || start > BOARD_SIZE || < 0 || end > BOARD_SIZE) return 0;
    //Check if start position has a piece
    if(!(CheckBit(*p1Board,start) | CheckBit(*p1KingBoard, start))) return 0;
    //Check if end position is blocked by either player
    if(CheckBit(*p1Board, end) | CheckBit(*p1KingBoard, end) | (CheckBit(*p2Board, end) | CheckBit(*p2KingBoard, end))) return 0;

    //Calculate starting and ending row, used to determine what kind of move is made (regular or jump move)
    //Calculate displacement
    int startRow = start/4;
    int endRow = end/4;
    int rowDisplacement = endRow-startRow;
    int displacement = end-start;

    //Determine certain conditions, player turn, king piece, is piece on left or right border
    int isBlack = (turn % 2) == 0 ? 1 : 0;
    int isKing = IsKingPiece(p1KingBoard, start) ? 1 : 0;
    int isLeftBorder = start % 8 == 0 ? 1 : 0;
    int isRightBorder = (start+1) % 8 == 0 ? 1 : 0;

    //Generate possible moveSet and jumpMoveSet, depending on piece type, position on board, and player turn
    //1-2 moves if regular piece, 2 or 4 moves if king piece
    int moveSet[4];
    int jumpMoveSet[4];
    //Assign moveSet
    if(isKing){
        if(isLeftBorder || isRightBorder){
            moveSet[0], moveSet[2] = -4;
            moveSet[1], moveSet[3] = 4;
        }
        else{
            if(startRow % 2 == 0){
                moveSet[0] = -5; 
                moveSet[1] = -4; 
                moveSet[2] = 3; 
                moveSet[3] = 4;
            }
            else{
                moveSet[0] = -4; 
                moveSet[1] = -3; 
                moveSet[2] = 4; 
                moveSet[3] = 5;
            }
        }
    }
    else{
        if(isLeftBorder || isRightBorder){
            moveSet[0] = isBlack ? 4 : -4; 
            moveSet[1] = isBlack ? 4 : -4;
        }
        else{
            if(startRow % 2 == 0){
                moveSet[0] = isBlack ? 3 : -4;
                moveSet[1] = isBlack ? 4 : -5;
            }
            else{
                moveSet[0] = isBlack ? 4 : -3;
                moveSet[1] = isBlack ? 5 : -4;
            }
        }
    }
    //Assign jumpMoveSet
    if(isKing){
        if(isLeftBorder){
            jumpMoveSet[0], jumpMoveSet[2] = 9;
            jumpMoveSet[1], jumpMoveSet[3] = -7;
        }
        else if(isRightBorder){
            jumpMoveSet[0], jumpMoveSet[2] = 7;
            jumpMoveSet[1], jumpMoveSet[3] = -9;
        }
        else{
            jumpMoveSet[0] = -9;
            jumpMoveSet[1] = -7;
            jumpMoveSet[2] = 7;
            jumpMoveSet[3] = 9;
        }
    }
    else{
        if(isLeftBorder){
            jumpMoveSet[0], jumpMoveSet[1] = isBlack ? 9 : -7;
        }
        else if (isRightBorder){
            jumpMoveSet[0], jumpMoveSet[1] = isBlack ? 7 : -9;
        }
        else{
            jumpMoveSet[0] = isBlack ? 7 : -7;
            jumpMoveSet[1] = isBlack ? 9 : -9;
        }
    }
    
    //If rowDisplacement = 2, check capture move
    //If rowDisplacement = 1, check normal move
    //Every other rowDisplacement is an invalid move

    //Handle capture moves
    if(abs(rowDisplacement) == 2){
        //Loop through jumpMoveSet, 2-4 times depending on piece type
        for(int i = 0; i < (isKing ? 4 : 2); i++){
            if(displacement == jumpMoveSet[i]){
                //Calculate mid-position
                int mid = startRow%2 == 0 ? (start+end)/2 : ((start+end)/2)+1;
                //Check if mid-position has an opponent piece
                if((CheckBit(*p2Board, mid) | CheckBit(*p2KingBoard, mid))){
                    //Capture the opponent's piece
                    CapturePiece(IsKingPiece(p2KingBoard, mid) ? p2KingBoard : p2Board, mid);
                    //Move the piece
                    MovePiece(isKing ? p1KingBoard : p1Board, start, end);
                    //Promote to king if necessary
                    if(!isKing && ((isBlack && end > 27) || (!isBlack && end < 4))){
                        PromoteKing(p1Board, p1KingBoard, end);
                    }
                    //Recursively check for possible consecutive jump capture moves
                    for(int j = 0; j < (isKing ? 4 : 2); j++){
                        IsLegalMove(p1Board, p2Board, p1KingBoard, p2KingBoard, end, end+jumpMoveSet[j], turn);
                    }
                    return 2; //Return 2 to indicate a valid capture move(s) was made
                }
            }
        }
    }

    //Handle regular moves
    if(abs(rowDisplacement) == 1){
        //Loop through moveSet, 2-4 times depending on piece type
        for(int i = 0; i < (isKing ? 4 : 2); i++){
            if(displacement == moveSet[i]){
                    //If move is in moveSet, move piece
                    MovePiece(isKing ? p1KingBoard : p1Board, start, end);
                    //Promote to king if necessary
                    if (!isKing && ((isBlack && end > 27) || (!isBlack && end < 4))) {
                        PromoteKing(p1Board, p1KingBoard, end);
                    }
                    return 1; //Return 1 to indicate a valid move was made
            }
        }
    }

    //rowDisplacement invalid, displacement invalid, cannot capture opponent, jump is not allowed
    //Return 0 to indicate invalid move
    return 0;
}

void PrintBoard(unsigned int* board) {
    for (int i = 0; i < 32; i++) {
        printf("%c", (board[i / 8] & (1 << (i % 8))) ? '1' : '0'); 
        if ((i + 1) % 4 == 0) printf("\n"); 
    } 
}

void PrintGameBoard(unsigned int p1Board, unsigned int p2Board, unsigned int p1KingBoard,unsigned int p2KingBoard) { 
    //Print the entire game board and the position indexes for easability
    for(int i = 7; i >= 0; i--){
        //Prints pieces
        for(int j = 0; j < 4; j++){
            if(i % 2 == 1) printf(". ");

            if(CheckBit(p1Board, i * 4 + j)){
                printf("b ");
            }
            else if(CheckBit(p1KingBoard, i * 4 + j)){
                printf("B ");
            }
            else if(CheckBit(p2Board, i * 4 + j)){
                printf("w ");
            }
            else if(CheckBit(p2KingBoard, i * 4 + j)){
                printf("W ");
            }
            else{
                printf("_ ");
            }

            if(i % 2 == 0) printf(". ");
        }
        printf("\t|\t ");

        //Prints position indexes
        for(int k = 0; k < 4; k++){
            if(i % 2 == 1) printf("00  ");
            if(i*4+k < 10) printf("0%d  ",i*4+k);
            else printf("%d  ", i*4+k);
            if(i % 2 == 0) printf("00  ");
        }
        printf("\n");
    }
    printf("\n");
}

int CountPieces(unsigned int pawnBoard, unsigned int kingBoard) {
    //Method used to count a players pieces
    int count = 0;
    
    //Loop through bitboards, increment count where a piece is
    for(int i = 0; i < BOARD_SIZE; i++){
        if(CheckBit(pawnBoard, i) | CheckBit(kingBoard,i)) count++;
    }
    return count;
}

void UpdateGameState(unsigned int* blackBoard, unsigned int* blackKingBoard, unsigned int* whiteBoard, unsigned int* whiteKingBoard) { 
     //Keep track of player turn and player pieces
     int turn = 0;
     int blackPieces = 12;
     int whitePieces = 12;
     int start, end;
     char hexString[9];

     while(blackPieces > 0 && whitePieces > 0){
        //Start by printing the game board and piece count
        PrintGameBoard(*blackBoard, *whiteBoard, *blackKingBoard, *whiteKingBoard);
        printf("Black:\n  -Pieces: %d\n  -Binary: ", blackPieces);
        PrintBinary(*blackBoard);
        ToHexString(*blackBoard, hexString);
        printf("  -Hex: %s\n", hexString);
        printf("White:\n  -Pieces: %d\n  -Binary: ", whitePieces);
        PrintBinary(*whiteBoard);
        ToHexString(*whiteBoard, hexString);
        printf("  -Hex: %s\n", hexString);


        if(turn%2 == BLACK){
            printf("\nBlack Turn:\n");
        }
        else{
            printf("\nWhite Turn: \n");
        }
        //Ask user for start and end positions
        printf("Enter the start position (Enter -1 to forfeit): ");
        scanf("%d", &start);
        if(start == -1){
            whitePieces = 0;
            break;
        }
        printf("Enter the end position: ");
        scanf("%d", &end);

        //Try move
        int legalMove = IsLegalMove(turn%2 == BLACK ? blackBoard : whiteBoard, turn%2 == BLACK ? whiteBoard : blackBoard, turn%2 == BLACK ? blackKingBoard : whiteKingBoard, turn%2 == BLACK ? whiteKingBoard : blackKingBoard, start, end, turn);

        //Further steps depending on return value of IsLegalMove()
        if(legalMove == 1){ 
            //Piece is moved successfully
        }
        else if(legalMove == 2){ 
            //Capture move is made successfully
            blackPieces = CountPieces(*blackBoard, *blackKingBoard);
            whitePieces = CountPieces(*whiteBoard, *whiteKingBoard);
        }
        else{ 
            //Move is invalid
            while(legalMove == 0){
                printf("Invalid move! Please try again.\n");
                printf("Enter the start position (Enter -1 to forfeit): ");
                scanf("%d", &start);
                if(start == -1){
                    whitePieces = 0;
                    break;
                }       
                printf("Enter the end position: ");
                scanf("%d", &end);
                legalMove = IsLegalMove(turn%2 == BLACK ? blackBoard : whiteBoard, turn%2 == BLACK ? whiteBoard : blackBoard, turn%2 == BLACK ? blackKingBoard : whiteKingBoard, turn%2 == BLACK ? whiteKingBoard : blackKingBoard, start, end, turn);
            }
        }
        turn++;
    }

    //Print winner!!
    if(blackPieces == 0){
        printf("White has won the game!\n");
    }
    if(whitePieces == 0){
        printf("Black has won the game!\n");
    }
    PrintGameBoard(*blackBoard, *whiteBoard, *blackKingBoard, *whiteKingBoard);
}