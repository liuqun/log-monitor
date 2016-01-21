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
    fprintf(stderr, "%s:%d:\n\t%s\n", from_file, at_line, msg);
}
#include <assert.h>
#include <ctype.h>
int IntegerValueFromHexCh(char hexch)
{
    const int ERROR = (~0x0F); //返回负数值0xFFFFFFF0表示错误
    const int MASK=0x000000FF;

    static char *charset = "0123456789ABCDEF";
    static int vtab[256]={0};
    int i;

    i = 0;
    while('\0'!=*charset)
    {
        vtab[(int)*charset] = i;
        charset++;
        i++;
    }

    if (!isxdigit(hexch&MASK) )
    {
        return (ERROR);
    }

    return (vtab[(int)toupper(hexch)]);
}
//int DecodeByteFromSource(const char *src)
//{
//    const int ERROR = (~0x00FF); // 错误返回负数值0xFFFFFF00；正常返回值应为[0,255] [0x0000,0x007F], [0x0080,0x00FF]
//    int result;
//
//    assert(NULL!=src);
//    if (!src)
//    {
//        return (ERROR);
//    }
//    if (!(isxdigit(src[0]) && isxdigit(src[1])))
//    {
//        return (ERROR);
//    }
//
//    result = 0xF & IntegerValueFromHexCh(toupper(src[0]));
//    result <<= 4;
//    result |= IntegerValueFromHexCh(toupper(src[1]))
//    return (result);
//}
const char *DiscardUselessPrefix(const char *src, int len)
{
    assert(NULL!=src);
    if (!src)
    {
        return (NULL);
    }
    assert(len>0);
    if (len<=0)
    {
        if (0==len)
        {
            return (NULL);
        }

        return (NULL);
    }
    while (len > 0)
    {
        if (':'==*src || isxdigit(*src))
        {
            break;
        }
        src++;
        len--;
    }
    return (src);
}
int DetectFirstRecordLen(const char *src, int limit)
{
    int i;
    assert(NULL!=src);
    if (!src)
    {
        return (-1);
    }
    assert(limit>0);
    if (limit<=0)
    {
        if (0==limit)
        {
            return (0);
        }

        return (-1);
    }
    i = 0;
    while (i<limit && (':'==src[i] || isxdigit(src[i])))
    {
        src++;
        i++;
    }
    while (i<limit)
    {
        if (':'==src[i] || isxdigit(src[i]))
        {
            break;
        }
        src++;
        i++;
    }
    return (i);
}
const char *MoveToNextRecordFromSource(const char *src, int len)
{
    assert(NULL!=src);
    if (!src)
    {
        return (NULL);
    }
    assert(len>0);
    if (len<=0)
    {
        if (0==len)
        {
            return (NULL);
        }

        return (NULL);
    }
    while (len > 0 && (':'==*src || isxdigit(*src)))
    {
        src++;
        len--;
    }
    while (len > 0)
    {
        if (':'==*src || isxdigit(*src))
        {
            break;
        }
        src++;
        len--;
    }
    return (src);
}
void SEND_DATA(int position, int len, char *data)
{
    int i;
    printf("%d:", position);
    printf("%d:", len);
    for (i=0; i<len; i++)
    {
        printf("%02X", data[i]);
    }
    printf("\n");
    return;
}
int main()
{
    const int PZZL_SIZ = 15;
    int at1 = 5, at2 = 0, at3 = 3;
    char *const str1 = "FGHIJKLMNO";
    char *const str2 = "ABC";
    char *const str3 = "DE";
    char source[] =
        "5:5:464748494A4B4C4D4E4F\n"
        "0:3:414243\n"
        "3:2:4445\n"
        ;

    DEBUG_LOG("--BEGIN--");
    SEND_DATA(at1, strlen(str1), str1);
    SEND_DATA(at2, strlen(str2), str2);
    SEND_DATA(at3, strlen(str3), str3);
    int v=IntegerValueFromHexCh('f');
    DEBUG_LOG("---END---");
    return (0);
}
