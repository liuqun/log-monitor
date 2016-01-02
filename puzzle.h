/*
 * File: puzzle.h
 * -----------------
 */
#ifndef PUZZLE_H_INCLUDED
#define PUZZLE_H_INCLUDED
#include <stdlib.h>

/**
 * Public Class: PuzzlePiece
 * -------------------------
 * Example Usage:
 * PuzzlePiece a[10];
 * for(int i=0,j=0; i<10; i++)
 * {
 *     len = strlen(str[i]);
 *     a[i] = CreateNewPuzzlePiece(j, len, str[i]);
 *     j += strlen;
 * }
 * ...
 * for(int i=0; i<10; i++)
 * {   DestroyPuzzlePiece(&(a[i]));
 * }
 */
typedef struct _PuzzlePiece *PuzzlePiece;
extern PuzzlePiece CreateNewPuzzlePiece(int position, int size, char *content);
extern int DestroyPuzzlePiece(PuzzlePiece *p);


/**
 * Public Class: Puzzle
 * -------------------------
 * Example Usage:
 * Puzzle puzzle = CreateNewPuzzle();
 * ...
 * DestoryPuzzle(puzzle);
 */
typedef struct _Puzzle *Puzzle;
extern Puzzle CreateNewPuzzle(int puzzle_size);
extern int DestroyPuzzle(Puzzle *p);
extern void Insert(Puzzle puzzle, PuzzlePiece piece);
extern int PuzzlePieceExsitsAt(Puzzle puzzle, int position);
extern int PuzzlePieceMissingAt(Puzzle puzzle, int position);
extern int GetPositionOfTheFirstMissingPiece(Puzzle puzzle);
extern int PuzzleIsFinished(Puzzle puzzle);
//extern void CollectPieces(Puzzle puzzle, PuzzlePiece pieces[], int cnt);
//extern size_t GetPuzzleSize(Puzzle puzzle);
//extern int CheckPuzzleIntegrity(Puzzle puzzle);



#endif // PUZZLE_H_INCLUDED
