#include <stdio.h>
#include <stdlib.h>
#include "utility.h"
#include "bitboard.h"

int main(){
    unsigned int blackBoard, blackKingBoard = 0; //Black
    unsigned int whiteBoard, whiteKingBoard = 0; //White
    int playButton = 1;
    
    printf("Bitboard Checkers:\n------------------\n");
    printf("\'b\' = Black Pawn, \'B\' = Black King\n\'w\' = White Pawn, \'W\' = White King\n");
    printf("\'_\' = Black Tile, \'.\' = White Tile\n\n");
    do{
        InitializeBoard(&blackBoard, &blackKingBoard, &whiteBoard, &whiteKingBoard);
        UpdateGameState(&blackBoard, &blackKingBoard, &whiteBoard, &whiteKingBoard);
        printf("Do you want to play again? (Enter 0 or 1)\n0: Exit Game\n1: Play Again\n");
        scanf("%d",&playButton);
    }while(playButton != 0);
    
    return 0;
}
