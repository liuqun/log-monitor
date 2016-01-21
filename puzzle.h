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
 * {   PuzzlePiece piece=a[i];
 *     DestroyPuzzlePiece(&piece);
 *     assert(piece==UNDEFINED_PUZZLE_PIECE);
 * }
 *
 * Error handling notes:
 * CreateNewPuzzlePiece(...) may abort() or return error value UNDEFINED_PUZZLE_PIECE.
 *
 * DestroyPuzzlePiece(&piece) will reset piece to UNDEFINED_PUZZLE_PIECE when object is successfully destroyed.
 * When input parameter &piece is invalid, DestroyPuzzlePiece(&piece) may abort() or return negative value -1 or -2.
 */
typedef struct _PuzzlePiece *PuzzlePiece;
extern PuzzlePiece CreateNewPuzzlePiece(int position, int size, char *content);
extern int DestroyPuzzlePiece(PuzzlePiece *p);


/**
 * Public Class: Puzzle
 * -------------------------
 * Example Usage:
 * Puzzle puzzle = CreateNewPuzzle(PUZZLE_SIZE);
 * ...
 * DestoryPuzzle(&puzzle);
 *
 *
 * Error handling notes:
 * CreateNewPuzzle(...) may abort() or return error value UNDEFINED_PUZZLE.
 *
 * DestroyPuzzle(&puzzle) will reset puzzle to UNDEFINED_PUZZLE when object is successfully destroyed.
 * When input parameter &puzzle is invalid, DestroyPuzzle(&puzzle) may abort() or return negative value -1 or -2.
 *
 * The following functions may abort() when puzzle/piece is invalid:
 * Insert(puzzle, piece)
 * PuzzlePieceExistsAt(puzzle, position)
 * PuzzlePieceMissingAt(puzzle, position)
 * GetPositonOfTheFirstMissingPiece(puzzle)
 * PuzzleIsFinished(puzzle)
 * GetPuzzleSize(puzzle)
 */
typedef struct _Puzzle *Puzzle;
extern Puzzle CreateNewPuzzle(int puzzle_size);
extern int DestroyPuzzle(Puzzle *p);
extern void Insert(Puzzle puzzle, PuzzlePiece piece);
extern int PuzzlePieceExsitsAt(Puzzle puzzle, int position);
extern int PuzzlePieceMissingAt(Puzzle puzzle, int position);
extern int GetPositionOfTheFirstMissingPiece(Puzzle puzzle);
extern int PuzzleIsFinished(Puzzle puzzle);
extern int GetPuzzleSize(Puzzle puzzle);



/**
 * Error handling value
 * --------------------
 * Both CreateNewPuzzle() and CreateNewPuzzlePiece() may return
 * UNDEFINED_OBJECT_PTR when any unexpected error happens.
 *
 * Both UNDEFINED_OBJECT_PTR, UNDEFINED_PUZZLE_PIECE and
 * UNDEFINED_PUZZLE will point to the same memory address.
 */
extern void *const UNDEFINED_OBJECT_PTR;
extern char *const UNDEFINED_CHAR_PTR; // This is used only for debug
extern Puzzle const UNDEFINED_PUZZLE;
extern PuzzlePiece const UNDEFINED_PUZZLE_PIECE;
#endif // PUZZLE_H_INCLUDED
