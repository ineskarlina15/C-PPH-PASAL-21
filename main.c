#define __USE_MINGW_ANSI_STDIO 1
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <conio.h>
#include <windows.h>
#include "utama.c"

int main()
{
    setConsoleSize(110, 35);
    bismillah();
    menu();
    return 0;
}
