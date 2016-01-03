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


PuzzlePiece CreateNewPuzzlePiece(int position, int initial_size, char *initial_content)
{
    struct _PuzzlePiece *piece = NULL;
    char *content = NULL;

    piece = malloc(sizeof(struct _PuzzlePiece));
    assert(NULL!=piece && "malloc() failed");
    if (!piece)
    {
        return (NULL);
    }
#   ifdef DEBUG
    memset(piece, 0x00, sizeof(struct _PuzzlePiece));
#   endif/*DEBUG*/

    content = NULL;
    assert(initial_size >= 0 && "initial piece size must >= 0");
    if (initial_size <= 0)
    {
        initial_size = 0;
        content = NULL;
    }else
    {
        content = malloc(initial_size);
        assert(NULL!=content && "malloc() failed");
        if(!content)
        {
            free(piece);
            piece = NULL;
            return(NULL);
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
        return (-1);
    }

    piece = *p;
    assert(NULL!=piece && "memory not allocated or already freed");
    if (!piece)
    {
        return (-2);
    }

    if (NULL!=piece->content)
    {
        free(piece->content);
    }
#   ifdef DEBUG
    piece->content = NULL;
    piece->size = 0;
#   endif/*DEBUG*/
    free(piece);
    piece = *p = NULL;
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
    if (!puzzle)
    {
        return (-1); /* Error */
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
        return (NULL);
    }
#   ifdef DEBUG
    memset(puzzle, 0x00, sizeof(struct _Puzzle));
#   endif/*DEBUG*/

    assert(puzzle_size > 0 && "invalid initial puzzle size");
    if (puzzle_size <= 0)
    {
        puzzle_size = 0;
        content = NULL;
    }else
    {
        content = malloc(puzzle_size);
        assert(NULL!=content && "malloc() failed");
        if (!content)
        {
            free(puzzle);
            puzzle = NULL;
            return (NULL);
        }
        refmap = calloc(sizeof(refmap[0]), puzzle_size);
        assert(NULL!=refmap && "calloc() failed");
        if (!refmap)
        {
            free(puzzle);
            puzzle = NULL;
            free(content);
            content = NULL;
            return (NULL);
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
        puzzle->content = content = NULL;
        free(refmap);
        puzzle->refmap = refmap = NULL;
        free(puzzle);
        puzzle = NULL;
        return (NULL);
    }
    return (puzzle);
}

int DestroyPuzzle(Puzzle *p)
{
    Puzzle puzzle;

    assert(NULL!=p && "invalid input parameter");
    if (!p)
    {
        return (-1);
    }
    puzzle = *p;
    assert(NULL!=puzzle && "memory not allocated or already freed");
    if (!puzzle)
    {
        return (-2);
    }
    if (puzzle->size > 0 && NULL != puzzle->content)
    {
        free(puzzle->content);
    }
    puzzle->content = NULL;
    puzzle->size = 0;
    if (puzzle->max > 0 && NULL != puzzle->pieces)
    {
        free(puzzle->pieces);
    }
    puzzle->cnt = puzzle->max = 0;
    puzzle->pieces = NULL;
    free(puzzle);
    puzzle = *p = NULL;
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
    if (!puzzle)
    {
        return (FALSE);
    }
    assert(position>=0 && position<puzzle->size && "safety check to avoid position overflow");
    return (position >= 0 &&
            position < puzzle->size &&
            puzzle->refmap[position] >= 0);
}

inline int PuzzlePieceMissingAt(Puzzle puzzle, int position) { return(!PuzzlePieceExsitsAt(puzzle, position)); };

int GetPositionOfTheFirstMissingPiece(Puzzle puzzle)
{
    int i;
    struct _PuzzlePiece *p=NULL;

    assert(NULL!=puzzle && "invalid object pointer");
    if (!puzzle)
    {
        return (-1);
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
