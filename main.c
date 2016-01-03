#include "puzzle.h"
#include <stdio.h>
#include <string.h>
#define DEBUG_LOG(msg)    Log(__FILE__, __LINE__, msg)
void Log(char *from_file, int at_line, char *msg)
{
    if (!from_file)
    {
        from_file = "(Unknown)";
    }
    if (!msg)
    {
        return;
    }
    fprintf(stderr, "%s:%d:\n\t%s\n", from_file, at_line, msg);//#define DEBUG_PRINTF(...) fprintf(stderr, __VA_ARGS__)
}

void SolvePuzzle(Puzzle puzzle)
{
    int position;
    int size;
    int i;
    int len;
    int at1 = 5, at2 = 0, at3 = 3;
    char *str1 = "FGHIJ";
    char *str2 = "ABC";
    char *str3 = "DE";
    PuzzlePiece piece = CreateNewPuzzlePiece(at1, strlen(str1), str1);
    Insert(puzzle, piece);
    DestroyPuzzlePiece(&piece);
    piece = CreateNewPuzzlePiece(at2, strlen(str2), str2);
    Insert(puzzle, piece);
    DestroyPuzzlePiece(&piece);

    /*检查缺少哪一块拼图*/
    size     = GetPuzzleSize(puzzle);
    position = GetPositionOfTheFirstMissingPiece(puzzle);
    if (position>=0 && position<size)
    {
        for (i=position+1; i<size&&PuzzlePieceMissingAt(puzzle, i); i++)
        {
            ;
        }
        len = i-position;
        /*填补缺少的一块拼图*/
        piece = CreateNewPuzzlePiece(at3, strlen(str3), str3);
        Insert(puzzle, piece);
        DestroyPuzzlePiece(&piece);
    }

    return;
}
int main()
{
    const int PZZL_SIZ = 10;
    DEBUG_LOG("--BEGIN--");
    Puzzle puzzle = CreateNewPuzzle(PZZL_SIZ);
    SolvePuzzle(puzzle);
    DestroyPuzzle(&puzzle);
    DEBUG_LOG("---END---");
    return (0);
}
