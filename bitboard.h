#ifndef BITBOARD_H
#define BITBOARD_H

void InitializeBoard(unsigned int* blackRegular, unsigned int* blackKings, unsigned int* whiteRegular, unsigned int* whiteKings);
int IsKingPiece(unsigned int* kingBoard, int position);
void MovePiece(unsigned int* board, int start, int end);
void promoteKing(unsigned int* piecesBoard, unsigned int* kingBoard, int position);
void CapturePiece(unsigned int* board, int position);
int IsLegalMove(unsigned int* p1Board, unsigned int* p2Board, unsigned int* p1KingBoard, unsigned int* p2KingBoard, int start, int end, int turn);
void UpdateGameState(unsigned int* blackBoard, unsigned int* blackKingBoard, unsigned int* whiteBoard, unsigned int* whiteKingBoard);
void PrintPlayerBoard(unsigned int board);
void PrintGameBoard(unsigned int p1Board, unsigned int p2Board, unsigned int p1KingBoard,unsigned int p2KingBoard);
int CountPieces(unsigned int pawnBoard, unsigned int kingBoard);
#endif