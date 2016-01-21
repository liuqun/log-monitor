#define DEBUG
#include <stdio.h>

#include <stdlib.h>
#include <memory.h>
#include <assert.h>
#include "puzzle.h"

struct _PuzzlePiece {
	int position;
	int size;
	char *content;
};

static void ErrorAbort(char errmsg[]);

PuzzlePiece CreateNewPuzzlePiece(int position, int initial_size, char *initial_content)
{
    struct _PuzzlePiece *piece = NULL;
    char *content;

    piece = malloc(sizeof(struct _PuzzlePiece));
    assert(NULL!=piece && "malloc() failed");
    if (!piece)
    {
        ErrorAbort("Error: Not enough memory");
        return (UNDEFINED_PUZZLE_PIECE);
    }
#   ifdef DEBUG
    memset(piece, 0x00, sizeof(struct _PuzzlePiece));
#   endif/*DEBUG*/

    assert(initial_size >= 0 && "initial piece size must >= 0");
    if (initial_size <= 0)
    {
        initial_size = 0;
        content = UNDEFINED_CHAR_PTR;
    }else
    {
        content = malloc(initial_size);
        assert(NULL!=content && "malloc() failed");
        if(!content)
        {
            free(piece);
            ErrorAbort("Error: Not enough memory");
            return (UNDEFINED_PUZZLE_PIECE);
        }
    }

    assert(NULL!=initial_content && "initial content should not be NULL");
    if (!initial_content)
    {
        memset(content, 0x00, initial_size);
    }else
    {
        memcpy(content, initial_content, initial_size);
    }

    piece->position = position;
    piece->size = initial_size;
    piece->content = content;
    return (piece);
}

int DestroyPuzzlePiece(PuzzlePiece *p)
{
    PuzzlePiece piece;

    assert(NULL!=p && "invalid input parameter");
    if (!p)
    {
        ErrorAbort("Error: Invalid parameter detected");
        return (-1);
    }
    piece = *p;
    assert(NULL!=piece && UNDEFINED_PUZZLE_PIECE!=piece && "memory not allocated or already freed");
    if (!piece || UNDEFINED_PUZZLE_PIECE==piece)
    {
        ErrorAbort("Error: Invalid parameter detected");
        return (-2);
    }

    if (NULL!=piece->content && UNDEFINED_CHAR_PTR!=piece->content )
    {
        free(piece->content);
    }
    free(piece);
    piece = *p = UNDEFINED_PUZZLE_PIECE;
    return (0);
}

//
//int CheckPuzzleIntegrity(Puzzle puzzle)
//{
//    return (false);
//}
//
//size_t GetPuzzleSize(Puzzle data)
//{
//    return (0);
//}

struct _Puzzle {
	int size;
	char *content;
	int *refmap;/*refmap[i]反方向映射到碎片，i<size && refmap[i]<cnt。初始值refmap[i]=-1表示content[i]单元格此时没有填入拼图碎片，refmap[i]>=0表示链接到碎片pieces[refmap[i]]*/
	int cnt;
	int max;/*Max pieces*/
	struct _PuzzlePiece *pieces;
};

extern int GetPuzzleSize(Puzzle puzzle)
{
    assert(NULL!=puzzle && "invalid object pointer");
    assert(UNDEFINED_PUZZLE!=puzzle && "UNDEFINED_OBJECT_PTR detected");
    if (!puzzle || UNDEFINED_PUZZLE==puzzle)
    {
        ErrorAbort("Error: Invalid parameter detected");
        return (-1);
    }
    return (puzzle->size);
}

Puzzle CreateNewPuzzle(int puzzle_size)
{
    struct _Puzzle *puzzle = NULL;
    char *content = NULL;
    int  *refmap   = NULL;

    puzzle = malloc(sizeof(struct _Puzzle));
    assert(NULL!=puzzle && "malloc() failed");
    if (!puzzle)
    {
        ErrorAbort("Error: Not enough memory");
        return (UNDEFINED_PUZZLE);
    }
#   ifdef DEBUG
    memset(puzzle, 0x00, sizeof(struct _Puzzle));
#   endif/*DEBUG*/

    assert(puzzle_size > 0 && "invalid initial puzzle size");
    if (puzzle_size <= 0)
    {
        return (UNDEFINED_PUZZLE);
    }else
    {
        content = malloc(puzzle_size);
        assert(NULL!=content && "malloc() failed");
        if (!content)
        {
            free(puzzle);
            ErrorAbort("Error: Not enough memory for content string");
            return (UNDEFINED_PUZZLE);
        }
        refmap = calloc(sizeof(refmap[0]), puzzle_size);
        assert(NULL!=refmap && "calloc() failed");
        if (!refmap)
        {
            free(puzzle);
            free(content);
            ErrorAbort("Error: Not enough memory for refmap");
            return (UNDEFINED_PUZZLE);
        }
        memset(refmap, 0xFF, sizeof(refmap[0])*puzzle_size); // 0xFF即-1
    }
    puzzle->size = puzzle_size;
    puzzle->content = content;
    puzzle->refmap = refmap;
    puzzle->cnt = 0;
    puzzle->max = 2;
    puzzle->pieces = calloc(puzzle->max, sizeof(struct _PuzzlePiece));
    assert(NULL!=puzzle->pieces && "calloc() failed");
    if (!puzzle->pieces)
    {
        free(content);
        free(refmap);
        free(puzzle);
        ErrorAbort("Error: Not enough memory for pieces array");
        return (UNDEFINED_PUZZLE);
    }
    return (puzzle);
}

int DestroyPuzzle(Puzzle *p)
{
    Puzzle puzzle;

    assert(NULL!=p && "invalid input parameter");
    if (!p)
    {
        ErrorAbort("Error: NULL PTR detected");
        return (-1);
    }
    puzzle = *p;
    assert(NULL!=puzzle && UNDEFINED_PUZZLE!=puzzle && "puzzle invalid or already destroyed");
    if (!puzzle || UNDEFINED_PUZZLE==puzzle)
    {
        ErrorAbort("Error: invalid input parameter detected");
        return (-2);
    }
    if (puzzle->size > 0 && NULL != puzzle->content && UNDEFINED_CHAR_PTR != puzzle->content)
    {
        free(puzzle->content);
    }
    if (puzzle->max > 0 && NULL != puzzle->pieces  && UNDEFINED_PUZZLE_PIECE != puzzle->pieces)
    {
        free(puzzle->pieces);
    }
    free(puzzle);
    puzzle = *p = UNDEFINED_PUZZLE;
    return (0);
}

void Insert(Puzzle puzzle, PuzzlePiece piece)
{
    int i;
    int j;
    struct _PuzzlePiece local;
    /*插入超长的拼图碎片时默认截断尾巴；新碎片内容将直接覆盖掉旧碎片*/
    char *to;
    to = puzzle->content + piece->position;
    char *from;
    from = piece->content;
    int len;
    len = piece->size;

    assert(NULL!=puzzle);
    assert(NULL!=piece);
    assert(NULL!=puzzle->content);
    assert(NULL!=piece->content);
    assert(piece->position >= 0);
    assert(piece->size > 0);
    assert(puzzle->cnt <= puzzle->max && puzzle->cnt+1 <= puzzle->max*2);
    assert(NULL!=puzzle->pieces);

    if (piece->position + piece->size > puzzle->size)
    {
        len = puzzle->size - piece->position;
    }
    memcpy(to, from, len);

    assert(puzzle->cnt <= puzzle->max && puzzle->cnt+1 <= puzzle->max*2);
    if (puzzle->cnt == puzzle->max)
    {
        struct _PuzzlePiece *ptr;
        ptr = calloc(puzzle->max*2, sizeof(struct _PuzzlePiece));
        assert(NULL != ptr && "calloc() failed");
        if (!ptr)
        {
            ErrorAbort("Error: Not enough memory for piece array");
            return;
        }
        memcpy(ptr, puzzle->pieces, puzzle->cnt*(sizeof(struct _PuzzlePiece)));
        free(puzzle->pieces);
        puzzle->pieces = ptr;
        puzzle->max = puzzle->max*2;
    }
    local.position = piece->position;
    local.size     = len;
    local.content  = to; /* Only address is copied here */
    memcpy(puzzle->pieces + puzzle->cnt, &local, sizeof(local));
    for(i=piece->position,j=0; j<len; i++,j++)
    {
        puzzle->refmap[i] = puzzle->cnt;
    }
    puzzle->cnt += 1;
    assert(puzzle->cnt <= puzzle->max); /* check again */
    return;
}

int PuzzlePieceExsitsAt(Puzzle puzzle, int position)
{
    const int FALSE = 0;

    assert(NULL!=puzzle && "invalid object pointer");
    assert(UNDEFINED_PUZZLE!=puzzle && "invalid input");
    if (!puzzle)
    {
        ErrorAbort("Error: NULL pointer detected");
        return (FALSE);
    }else if (UNDEFINED_PUZZLE==puzzle)
    {
        return (FALSE);
    }
    assert(position>=0 && position<puzzle->size && "safety check to avoid position overflow");
    return (position >= 0 &&
            position < puzzle->size &&
            puzzle->refmap[position] >= 0);
    /* FIXME: Forget to check puzzle->refmap pointer safety */
}

inline int PuzzlePieceMissingAt(Puzzle puzzle, int position) { return(!PuzzlePieceExsitsAt(puzzle, position)); };

int GetPositionOfTheFirstMissingPiece(Puzzle puzzle)
{
    int i;
    struct _PuzzlePiece *p;

    assert(NULL!=puzzle && "invalid object pointer");
    if (!puzzle)
    {
        ErrorAbort("Error: NULL pointer detected");
        return (-3);
    }else if (UNDEFINED_PUZZLE==puzzle)
    {
        return (-2);
    }
    i = 0;
    while (i<puzzle->size)
    {
        if (puzzle->refmap[i] < 0)
        {
            return (i);
        }
        assert(NULL!=puzzle->pieces && "invalid object pointer");
        assert(NULL!=puzzle->refmap && "invalid object pointer");
        assert(i < puzzle->size && "check index overflow");
        assert(puzzle->refmap[i]>=0 && (puzzle->refmap[i] < puzzle->cnt) && "check index overflow");
        assert(puzzle->cnt <= puzzle->max && "check puzzle piece index");
        p = &(puzzle->pieces[puzzle->refmap[i]]);
        assert(NULL!=p && "invalid object pointer");
        i = p->position + p->size; /* goto next puzzle piece */
    }
    return (-1); /* When all puzzle pieces are finished */
}

inline int PuzzleIsFinished(Puzzle puzzle) {return (GetPositionOfTheFirstMissingPiece(puzzle)<0);};

/**
 * Error handling stuff
 * --------------------
 * The following pointers are used when faild to create new objects:
 * UNDEFINED_OBJECT_PTR
 * UNDEFINED_PUZZLE_PIECE
 * UNDEFINED_PUZZLE
 */
union _OBJECT {
    struct _PuzzlePiece piece;
    struct _Puzzle      puzzle;
    char                byte;
};
static union _OBJECT undefined_object={/* Just leave initial value undefined here*/};
void *const UNDEFINED_OBJECT_PTR = &undefined_object;
char *const UNDEFINED_CHAR_PTR   = &(undefined_object.byte); // This is used only for debugging
Puzzle const UNDEFINED_PUZZLE    = &(undefined_object.puzzle);
PuzzlePiece const UNDEFINED_PUZZLE_PIECE = &(undefined_object.piece);

/**
 * Function: ErrorAbort()
 * Usage: ErrorAbort("Fatal error: Something happened...");
 * --------------------------------------------------------
 * Report an error message then abort immediately.
 */
static void ErrorAbort(char errmsg[])
{
    (void) errmsg;// TODO: Report the error message, anyway...
    abort();
}
