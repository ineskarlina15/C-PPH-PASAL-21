////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                    FIRST PROJECT BAHASA C                                                      //
//                                                         PPH PASAL 21                                                           //
//                                                 ============================                                                   //
//                                                                                                                                //
//                                                         INES KARLINA                                                           //
//                                                         S1 AKUNTANSI                                                           //
//                                                        PUB A'23 SYNTAK                                                         //
//                                                                                                                                //
//                                                   DIMULAI : 31 DESEMBER 2024                                                   //
//                                                    SELESAI : 9 JANUARI 2025                                                    //
//                                                                                                                                //
//                                                       SCREEN BUFFER SIZE                                                       //
//                                                   WIDTH : 110    HEIGHT : 35                                                   //
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <windows.h>
#include <conio.h>
#include <time.h>

#define handle GetStdHandle(STD_OUTPUT_HANDLE)

/////////////////////////////////////////////////////////////// PROTOTYPE //////////////////////////////////////////////////////////////////////
void setConsoleSize(int width, int height);
void setupCustomPalette();
void gotoxy(int x, int y);
void textcolor(int new_color);
void backgroundcolor(int new_color);
void setcolor(int text, int background);
void clearScreen();
void bingkai(int x, int y, int panjang, int lebar, int text, int background);
void isi_bingkai(int x, int y, int panjang, int lebar, int text, int background);

void bismillah();
void teks_loading();
void loading();
void welcome();
void menu();
int signup();
int login();
void update_profile();
int delete_profile();
void menu_fitur();
void gambar_tombol_menu(int no, int aktif);
void gambar_tombol_fitur(int no, int aktif);
void info_pph();
void hitpph();
void laporspt();
void histori_kosong();
void data_histori();

//////////////////////////////////////////////////////////////// koordinat & warna /////////////////////////////////////////////////////////////
void gotoxy(int x, int y)
{
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(handle, coord);
}

int text = 15, background = 4;
void textcolor(int new_color)
{
    SetConsoleTextAttribute(handle, new_color + background * 16);
    text = new_color;
}

void backgroundcolor(int new_color)
{
    SetConsoleTextAttribute(handle, text + new_color * 16);
    background = new_color;
}

#ifndef ENABLE_VIRTUAL_TERMINAL_PROCESSING
#define ENABLE_VIRTUAL_TERMINAL_PROCESSING 0x0004
#endif

typedef struct {
    int r, g, b;
} PaletteColor;

// 4 Warna Palette dari Foto User:
// 1. Vanilla:    RGB(243, 227, 178) -> HEX #F3E3B2
// 2. Dark Green: RGB(70, 70, 42)    -> HEX #46462A
// 3. Blood Red:  RGB(116, 7, 13)    -> HEX #74070D
// 4. Bean:       RGB(49, 15, 16)    -> HEX #310F10
static const PaletteColor PALETTE[16] = {
    {0, 0, 0},         // 0: Black
    {49, 15, 16},      // 1: Bean (#310F10)
    {70, 70, 42},      // 2: Dark Green (#46462A)
    {70, 70, 42},      // 3: Dark Green
    {116, 7, 13},      // 4: Blood Red (#74070D)
    {49, 15, 16},      // 5: Bean
    {70, 70, 42},      // 6: Dark Green
    {243, 227, 178},   // 7: Vanilla (#F3E3B2)
    {49, 15, 16},      // 8: Bean (#310F10)
    {70, 70, 42},      // 9: Dark Green
    {70, 70, 42},      // 10: Dark Green
    {243, 227, 178},   // 11: Vanilla (#F3E3B2)
    {225, 45, 45},     // 12: Warning Red
    {243, 227, 178},   // 13: Vanilla
    {243, 227, 178},   // 14: Vanilla (#F3E3B2)
    {243, 227, 178}    // 15: Vanilla (#F3E3B2)
};

static int cur_text = -1, cur_bg = -1;

void setcolor(int text, int background)
{
    if (text < 0 || text > 15) text = 15;
    if (background < 0 || background > 15) background = 4;

    if (text == cur_text && background == cur_bg)
    {
        return;
    }
    cur_text = text;
    cur_bg = background;

    // Fallback: ganti indeks biru bawaan konsol agar tidak muncul warna biru sama sekali
    int win_text = text;
    int win_bg = background;
    if (win_bg == 1 || win_bg == 8) win_bg = 4;
    if (win_text == 11 || win_text == 9) win_text = 14;

    SetConsoleTextAttribute(handle, win_bg * 16 + win_text);

    // ANSI 24-bit TrueColor untuk Windows Terminal & Windows 10/11
    printf("\033[38;2;%d;%d;%dm\033[48;2;%d;%d;%dm",
           PALETTE[text].r, PALETTE[text].g, PALETTE[text].b,
           PALETTE[background].r, PALETTE[background].g, PALETTE[background].b);
}

void clearScreen()
{
    cur_text = -1;
    cur_bg = -1;
    printf("\033[48;2;116;7;13m\033[38;2;243;227;178m");
    system("cls");
}

void enableVirtualTerminal()
{
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hOut != INVALID_HANDLE_VALUE)
    {
        DWORD dwMode = 0;
        if (GetConsoleMode(hOut, &dwMode))
        {
            dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
            SetConsoleMode(hOut, dwMode);
        }
    }
}

typedef struct _CONSOLE_SCREEN_BUFFER_INFOEX_CUSTOM {
    ULONG cbSize;
    COORD dwSize;
    COORD dwCursorPosition;
    WORD wAttributes;
    SMALL_RECT srWindow;
    COORD dwMaximumWindowSize;
    WORD wPopupAttributes;
    BOOL bFullscreenSupported;
    COLORREF ColorTable[16];
} CONSOLE_SCREEN_BUFFER_INFOEX_CUSTOM;

typedef BOOL (WINAPI *PGetConsoleScreenBufferInfoEx)(HANDLE, CONSOLE_SCREEN_BUFFER_INFOEX_CUSTOM*);
typedef BOOL (WINAPI *PSetConsoleScreenBufferInfoEx)(HANDLE, CONSOLE_SCREEN_BUFFER_INFOEX_CUSTOM*);

void setupCustomPalette()
{
    HANDLE hOut = CreateFileA("CONOUT$", GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);
    if (hOut == INVALID_HANDLE_VALUE)
    {
        hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    }
    HMODULE hKernel = GetModuleHandleA("kernel32.dll");
    if (hKernel)
    {
        PGetConsoleScreenBufferInfoEx pGet = (PGetConsoleScreenBufferInfoEx)GetProcAddress(hKernel, "GetConsoleScreenBufferInfoEx");
        PSetConsoleScreenBufferInfoEx pSet = (PSetConsoleScreenBufferInfoEx)GetProcAddress(hKernel, "SetConsoleScreenBufferInfoEx");

        if (pGet && pSet)
        {
            CONSOLE_SCREEN_BUFFER_INFOEX_CUSTOM csbi;
            csbi.cbSize = sizeof(CONSOLE_SCREEN_BUFFER_INFOEX_CUSTOM);
            if (pGet(hOut, &csbi))
            {
                csbi.srWindow.Right += 1;
                csbi.srWindow.Bottom += 1;

                // 4 Warna Palette Sesuai Permintaan User:
                // 1. Vanilla    : HEX #F3E3B2 -> RGB(243, 227, 178)
                // 2. Dark Green : HEX #46462A -> RGB(70, 70, 42)
                // 3. Blood Red  : HEX #74070D -> RGB(116, 7, 13)
                // 4. Bean       : HEX #310F10 -> RGB(49, 15, 16)

                // Background utama (Layar Luar)
                csbi.ColorTable[4] = RGB(116, 7, 13);     // Blood Red

                // Background Kartu / Panel Formulir
                csbi.ColorTable[1] = RGB(49, 15, 16);     // Bean
                csbi.ColorTable[8] = RGB(49, 15, 16);     // Bean

                // Tombol Aktif & Aksen Garis
                csbi.ColorTable[2] = RGB(70, 70, 42);     // Dark Green
                csbi.ColorTable[9] = RGB(70, 70, 42);     // Dark Green (garis pembatas)
                csbi.ColorTable[10] = RGB(100, 110, 60);  // Dark Green Accent

                // Warna Teks Utama & Border Kartu
                csbi.ColorTable[14] = RGB(243, 227, 178); // Vanilla (Border & Header)
                csbi.ColorTable[15] = RGB(243, 227, 178); // Vanilla (Teks Utama & Input)
                csbi.ColorTable[7]  = RGB(220, 205, 160); // Vanilla Lembut (Tombol Inaktif)
                csbi.ColorTable[11] = RGB(243, 227, 178); // Vanilla

                // Warna Peringatan / Error
                csbi.ColorTable[12] = RGB(225, 45, 45);

                pSet(hOut, &csbi);
            }
        }
    }
    if (hOut != INVALID_HANDLE_VALUE && hOut != GetStdHandle(STD_OUTPUT_HANDLE))
    {
        CloseHandle(hOut); 
    }
}

void setConsoleSize(int width, int height)
{
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    SMALL_RECT minRect = {0, 0, 1, 1};
    SetConsoleWindowInfo(hOut, TRUE, &minRect);
    COORD coord = {(SHORT)width, (SHORT)height};
    SetConsoleScreenBufferSize(hOut, coord);
    SMALL_RECT rect = {0, 0, (SHORT)(width - 1), (SHORT)(height - 1)};
    SetConsoleWindowInfo(hOut, TRUE, &rect);

    char cmd[64];
    sprintf(cmd, "mode con: cols=%d lines=%d", width, height);
    system(cmd);
    system("color 4F"); // Latar belakang Blood Red (4), teks Vanilla (F)

    enableVirtualTerminal();
    setupCustomPalette();
}

#include "validasi.c"

//////////////////////////////////////////////////////////////// VARIABLE /////////////////////////////////////////////////////////////////////
int tekan = 1;
int inp;

char pilih;
char balik;
char swp;
char namawp[50], npwpspt[50], kodewp[50], tanggal[50], pajakwp[50];
char tamp[100][5][100];
char gaji[20];
char iuran_bulan[20];
char no_hp[20];
char nama[50];
char user[20];
char pass[20];

long long gajiperbulan;
long long gaji_lembur;
long long pbruto;
long long biayajabatan;
long long iuranpensiun;
long long totalpengurangan;
long long netto;
long long netto_setahun;
long long ptkp;
long long pkp;
long long pajakpph;
long long total_pengurang;

///////////////////////////////////////////////////////////// bingkai //////////////////////////////////////////////////////////////////////////
void bingkai(int x, int y, int panjang, int lebar, int text, int background)
{
    char baris_border[115];
    char baris_spasi[115];
    int w = panjang + 1;
    if (w > 110) w = 110;

    for (int k = 0; k < w; k++)
    {
        baris_border[k] = (char)219;
    }
    baris_border[w] = '\0';

    int spasi_len = (panjang > 1) ? (panjang - 1) : 0;
    if (spasi_len > 110) spasi_len = 110;
    for (int s = 0; s < spasi_len; s++)
    {
        baris_spasi[s] = ' ';
    }
    baris_spasi[spasi_len] = '\0';

    for (int j = 0; j <= lebar; j++)
    {
        if (y + j >= 35) break;

        if (j == 0 || j == lebar)
        {
            gotoxy(x, y + j);
            setcolor(text, text);
            if (x + panjang >= 109 && y + j >= 34)
            {
                if (panjang > 0)
                {
                    char temp[115];
                    strncpy(temp, baris_border, panjang);
                    temp[panjang] = '\0';
                    printf("%s", temp);
                }
                COORD coord = {109, 34};
                DWORD written;
                WORD attr = (WORD)(text | (text << 4));
                char ch = (char)219;
                WriteConsoleOutputCharacterA(handle, &ch, 1, coord, &written);
                WriteConsoleOutputAttribute(handle, &attr, 1, coord, &written);
            }
            else
            {
                printf("%s", baris_border);
            }
        }
        else
        {
            gotoxy(x, y + j);
            setcolor(text, text);
            printf("%c", (char)219);

            if (spasi_len > 0)
            {
                setcolor(background, background);
                printf("%s", baris_spasi);
            }

            if (panjang > 0)
            {
                setcolor(text, text);
                printf("%c", (char)219);
            }
        }
    }
}

void isi_bingkai(int x, int y, int panjang, int lebar, int text, int background)
{
    char baris[115];
    int w = panjang + 1;
    if (w > 110) w = 110;
    for (int k = 0; k < w; k++)
    {
        baris[k] = (char)219;
    }
    baris[w] = '\0';

    setcolor(text, background);
    for (int j = 0; j <= lebar; j++)
    {
        if (y + j < 35 && x < 110)
        {
            gotoxy(x, y + j);
            printf("%s", baris);
        }
    }
}

//////////////////////////////////////////////////////////////// PEMBUKAAN /////////////////////////////////////////////////////////////////////
void teks_loading()
{
    char s = 219, // blok █
         t = 187, // kanan atas ╗
         u = 188, // kanan bawah ╝
         v = 186, // lurus atas ║
         x = 205, // lurus garis ═
         w = 201, // kiri atas ╔
         y = 200; // kiri bawah ╚

    setcolor(14, 4);
    gotoxy(26, 11);
    printf("%c%c%c      %c%c%c%c%c%c%c  %c%c%c%c%c%c %c%c%c%c%c%c%c %c%c%c%c%c%c%c   %c%c%c %c%c%c%c%c%c%c", s, s, v, s, s, s, s, s, s, t, s, s, s, s, s, t, s, s, s, s, s, s, t, s, s, t, s, s, s, t, s, s, t, s, s, s, s, s, s, t);
    gotoxy(26, 12);
    printf("%c%c%c     %c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c  %c%c%c%c%c%c%c%c%c%c%c", s, s, v, s, s, w, x, x, x, s, s, t, s, s, w, x, x, s, s, t, s, s, w, x, x, s, s, w, s, s, v, s, s, s, s, t, s, s, v, s, s, w, x, x, x, x, u);
    gotoxy(26, 13);
    printf("%c%c%c     %c%c%c   %c%c%c%c%c%c%c%c%c%c%c%c%c%c  %c%c%c%c%c%c%c%c%c%c%c%c %c%c%c%c%c%c  %c%c%c%c", s, s, v, s, s, v, s, s, v, s, s, s, s, s, s, s, v, s, s, v, s, s, v, s, s, v, s, s, w, s, s, t, s, s, v, s, s, v, s, s, s, t);
    gotoxy(26, 14);
    printf("%c%c%c     %c%c%c   %c%c%c%c%c%c%c%c%c%c%c%c%c%c  %c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c   %c%c%c", s, s, v, s, s, v, s, s, v, s, s, w, x, x, s, s, v, s, s, v, s, s, v, s, s, v, s, s, v, y, s, s, t, s, s, v, s, s, v, s, s, v);
    gotoxy(26, 15);
    printf("%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c  %c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c %c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c", s, s, s, s, s, s, s, t, y, s, s, s, s, s, s, w, u, s, s, v, s, s, v, s, s, s, s, s, s, w, u, s, s, v, s, s, v, y, s, s, s, s, v, y, s, s, s, s, s, s, w, u, s, s, t, s, s, t, s, s, t);
    gotoxy(26, 16);
    printf("%c%c%c%c%c%c%c%c %c%c%c%c%c%c%c %c%c%c  %c%c%c%c%c%c%c%c%c%c %c%c%c%c%c%c  %c%c%c%c%c %c%c%c%c%c%c%c %c%c%c%c%c%c%c%c%c", y, x, x, x, x, x, x, u, y, x, x, x, x, x, u, y, x, u, y, x, u, y, x, x, x, x, x, u, y, x, u, y, x, u, y, x, x, x, u, y, x, x, x, x, x, u, y, x, u, y, x, u, y, x, u);
}

void loading()
{
    char s = 219, // blok █
         x = 205; // lurus garis ═

    setcolor(14, 4);
    for (int j = 0; j < 66; j++)
    {
        gotoxy(22 + j, 19);
        printf("%c", x);
        gotoxy(22 + j, 21);
        printf("%c", x);
    }
    gotoxy(21, 20);
    printf("[");
    gotoxy(88, 20);
    printf("]");

    for (int i = 0; i < 33; i++)
    {
        Sleep(15);
        setcolor(15, 4);
        gotoxy(22 + i * 2, 20);
        printf("%c%c", s, s);

        int persen = ((i + 1) * 100) / 33;
        gotoxy(50, 23);
        setcolor(14, 4);
        printf("Memuat: %3d%%", persen);
    }
}

void bismillah()
{
    setConsoleSize(110, 35);
    clearScreen();
    bingkai(0, 0, 109, 34, 14, 4);
    teks_loading();
    loading();
    clearScreen();
    welcome();
}

void welcome()
{
    bingkai(0, 0, 109, 34, 14, 4);
    char s = 219,
         t = 176;

    int anim_colors[] = {14, 2, 14, 2, 14, 15, 2, 14};
    for (int a = 0; a < 8; a++)
    {
        setcolor(anim_colors[a], 4);

        gotoxy(19, 2);
        printf("                          %c%c%c%c", s, s, s, s);
        gotoxy(19, 3);
        printf("                         %c%c%c%c%c", t, t, s, s, s);
        gotoxy(19, 4);
        printf(" %c%c%c%c%c %c%c%c %c%c%c%c%c  %c%c%c%c%c%c  %c%c%c%c   %c%c%c%c%c%c   %c%c%c%c%c%c  %c%c%c%c%c%c%c%c%c%c%c%c%c    %c%c%c%c%c%c ", s, s, s, s, s, s, s, s, s, s, s, s, s, s, s, s, s, s, s, t, s, s, s, s, s, s, s, s, s, s, s, s, s, s, s, s, s, s, s, s, s, s, s, s, s, s, s, s, s, s, s, s, s, s);
        gotoxy(19, 5);
        printf("%c%c%c%c%c %c%c%c%c%c%c%c%c%c  %c%c%c%c%c%c%c%c %c%c%c%c  %c%c%c%c%c%c%c%c %c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c  %c%c%c%c%c%c%c%c", t, t, s, s, s, t, s, s, s, t, t, s, s, s, s, s, s, t, t, s, s, s, t, s, s, s, s, s, s, t, t, s, s, s, s, s, s, t, t, s, s, s, t, t, s, s, s, t, t, s, s, s, t, t, s, s, s, s, s, s, t, t, s, s, s);
        gotoxy(19, 6);
        printf(" %c%c%c%c %c%c%c%c %c%c%c%c %c%c%c%c%c%c%c%c  %c%c%c%c %c%c%c%c %c%c%c %c%c%c%c %c%c%c%c %c%c%c%c %c%c%c%c %c%c%c%c %c%c%c%c%c%c%c%c ", t, s, s, s, t, s, s, s, t, s, s, s, t, s, s, s, s, s, s, s, t, s, s, s, t, s, s, s, t, t, t, t, s, s, s, t, s, s, s, t, s, s, s, t, s, s, s, t, s, s, s, t, s, s, s, s, s, s, s);
        gotoxy(19, 7);
        printf(" %c%c%c%c%c%c%c%c%c%c%c%c%c  %c%c%c%c%c%c%c   %c%c%c%c %c%c%c%c  %c%c%c%c%c%c%c %c%c%c%c %c%c%c%c %c%c%c%c %c%c%c%c %c%c%c%c%c%c%c  ", t, t, s, s, s, s, s, s, s, s, s, s, s, t, s, s, s, t, t, t, t, s, s, s, t, s, s, s, s, s, s, t, s, s, s, t, s, s, s, t, s, s, s, t, s, s, s, t, s, s, s, t, s, s, s, t, t, t);
        gotoxy(19, 8);
        printf("  %c%c%c%c%c%c%c%c%c%c%c   %c%c%c%c%c%c%c%c  %c%c%c%c%c%c%c%c%c%c%c%c%c %c%c%c%c%c%c%c%c  %c%c%c%c%c%c%c%c%c %c%c%c%c%c%c%c%c%c%c%c%c%c ", t, t, s, s, s, s, t, s, s, s, s, t, t, s, s, s, s, s, s, s, s, s, s, s, t, t, s, s, s, s, s, s, t, t, s, s, s, s, s, s, s, s, s, s, s, t, s, s, s, s, s, s, s, s, t, t, s, s, s, s, s, s);
        gotoxy(19, 9);
        printf("   %c%c%c%c %c%c%c%c     %c%c%c%c%c%c  %c%c%c%c%c  %c%c%c%c%c%c   %c%c%c%c%c%c  %c%c%c%c%c %c%c%c %c%c%c%c%c  %c%c%c%c%c%c  ", t, t, t, t, t, t, t, t, t, t, t, t, t, t, t, t, t, t, t, t, t, t, t, t, t, t, t, t, t, t, t, t, t, t, t, t, t, t, t, t, t, t, t, t, t, t, t, t, t, t);

        gotoxy(28, 11);
        printf("  %c%c%c%c%c                 %c%c%c", s, s, s, s, s, s, s, s);
        gotoxy(28, 12);
        printf(" %c%c%c%c%c                 %c%c%c", t, t, s, s, s, t, t, t);
        gotoxy(28, 13);
        printf(" %c%c%c%c%c%c%c    %c%c%c%c%c%c     %c%c%c%c  %c%c%c%c%c%c%c%c    %c%c%c%c%c%c   %c%c%c%c%c", s, s, s, s, s, s, s, s, s, s, s, s, s, s, s, s, s, s, s, s, s, s, s, s, s, s, s, s, s, s, s, s, s, s, s, s);
        gotoxy(28, 14);
        printf("%c%c%c%c%c%c%c    %c%c%c%c%c%c%c%c   %c%c%c%c%c %c%c%c%c%c%c%c%c%c%c  %c%c%c%c%c%c%c%c %c%c%c%c%c", t, t, t, s, s, s, t, s, s, s, t, t, s, s, s, t, t, s, s, s, t, t, s, s, s, t, t, s, s, s, s, s, s, t, t, s, s, s, s, s, s, t, t);
        gotoxy(28, 15);
        printf("  %c%c%c%c    %c%c%c%c %c%c%c%c    %c%c%c%c  %c%c%c%c %c%c%c%c %c%c%c%c%c%c%c%c %c%c%c%c%c%c%c", t, s, s, s, t, s, s, s, t, s, s, s, t, s, s, s, t, s, s, s, t, s, s, s, t, s, s, s, s, s, s, s, t, t, s, s, s, s, s);
        gotoxy(28, 16);
        printf("  %c%c%c%c %c%c%c%c%c%c%c %c%c%c%c    %c%c%c%c  %c%c%c%c %c%c%c%c %c%c%c%c%c%c%c   %c%c%c%c%c%c%c", t, s, s, s, s, s, s, t, s, s, s, t, s, s, s, t, s, s, s, t, s, s, s, t, s, s, s, t, s, s, s, t, t, t, t, t, t, t, s, s, s);
        gotoxy(28, 17);
        printf("  %c%c%c%c%c%c%c %c%c%c%c%c%c%c%c     %c%c%c%c%c %c%c%c%c %c%c%c%c%c%c%c%c%c%c%c%c%c  %c%c%c%c%c%c", t, t, s, s, s, s, s, t, t, s, s, s, s, s, s, s, s, s, s, s, s, s, s, s, s, s, s, s, s, t, t, s, s, s, s, s, s, s, s, s, s, s, s);
        gotoxy(28, 18);
        printf("   %c%c%c%c%c   %c%c%c%c%c%c     %c%c%c%c%c %c%c%c%c %c%c%c%c%c  %c%c%c%c%c%c  %c%c%c%c%c%c", t, t, t, t, t, t, t, t, t, t, t, t, t, t, t, t, t, t, t, t, t, t, t, t, t, t, t, t, t, t, t, t, t, t, t, t, t);

        gotoxy(23, 20);
        printf("                                   %c%c%c                     %c%c%c%c%c", s, s, s, s, s, s, s, s);
        gotoxy(23, 21);
        printf("                                  %c%c%c                     %c%c%c%c%c", t, t, t, t, t, s, s, s);
        gotoxy(23, 22);
        printf(" %c%c%c%c%c%c%c%c  %c%c%c%c%c%c%c%c   %c%c%c%c%c%c      %c%c%c%c%c  %c%c%c%c%c%c   %c%c%c%c%c%c  %c%c%c%c%c%c%c", s, s, s, s, s, s, s, s, s, s, s, s, s, s, s, s, s, s, s, s, s, s, s, s, s, s, s, s, s, s, s, s, s, s, s, s, s, s, s, s, s, s, s, s, s, s);
        gotoxy(23, 23);
        printf("%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c %c%c%c%c%c%c%c%c    %c%c%c%c%c  %c%c%c%c%c%c%c%c %c%c%c%c%c%c%c%c%c%c%c%c%c%c%c", t, t, s, s, s, t, t, s, s, s, t, t, s, s, s, t, t, s, s, s, s, s, s, t, t, s, s, s, t, t, s, s, s, s, s, s, t, t, s, s, s, s, s, s, t, t, s, s, s, t, t, t, s, s, s, t);
        gotoxy(23, 24);
        printf(" %c%c%c%c %c%c%c%c %c%c%c%c %c%c%c %c%c%c%c %c%c%c%c     %c%c%c%c %c%c%c%c%c%c%c%c %c%c%c%c %c%c%c   %c%c%c%c", t, s, s, s, t, s, s, s, t, s, s, s, t, t, t, t, s, s, s, t, s, s, s, t, s, s, s, t, s, s, s, s, s, s, s, t, s, s, s, t, t, t, t, s, s, s);
        gotoxy(23, 25);
        printf(" %c%c%c%c %c%c%c%c %c%c%c%c     %c%c%c%c %c%c%c%c     %c%c%c%c %c%c%c%c%c%c%c  %c%c%c%c  %c%c%c  %c%c%c%c %c%c%c", t, s, s, s, t, s, s, s, t, s, s, s, t, s, s, s, t, s, s, s, t, s, s, s, t, s, s, s, t, t, t, t, s, s, s, s, s, s, t, s, s, s, s, s, s);
        gotoxy(23, 26);
        printf(" %c%c%c%c%c%c%c%c  %c%c%c%c%c    %c%c%c%c%c%c%c%c      %c%c%c%c %c%c%c%c%c%c%c%c %c%c%c%c%c%c%c%c   %c%c%c%c%c%c%c", t, s, s, s, s, s, s, s, s, s, s, s, s, t, t, s, s, s, s, s, s, t, s, s, s, t, t, s, s, s, s, s, s, t, t, s, s, s, s, s, s, t, t, s, s, s, s, s);
        gotoxy(23, 27);
        printf(" %c%c%c%c%c%c%c  %c%c%c%c%c      %c%c%c%c%c%c       %c%c%c%c  %c%c%c%c%c%c   %c%c%c%c%c%c     %c%c%c%c%c", t, s, s, s, t, t, t, t, t, t, t, t, t, t, t, t, t, t, t, s, s, s, t, t, t, t, t, t, t, t, t, t, t, t, t, t, t, t, t);
        gotoxy(23, 28);
        printf(" %c%c%c%c                         %c%c%c %c%c%c%c", t, s, s, s, s, s, s, t, s, s, s);
        gotoxy(23, 29);
        printf(" %c%c%c%c%c                       %c%c%c%c%c%c%c%c", s, s, s, s, s, t, t, s, s, s, s, s, s);
        gotoxy(23, 30);
        printf("%c%c%c%c%c                         %c%c%c%c%c%c", t, t, t, t, t, t, t, t, t, t, t);

        Sleep(35);
    }

    clearScreen();
}

void gambar_tombol_menu(int no, int aktif)
{
    int x = (no == 1) ? 10 : (no == 2) ? 43 : 76;
    int y = 17;
    int borderColor = aktif ? 14 : 2;
    int bgColor     = aktif ? 2  : 1;
    int txtColor    = aktif ? 15 : 7;

    bingkai(x, y, 24, 5, borderColor, bgColor);
    isi_bingkai(x + 1, y + 1, 22, 3, bgColor, bgColor);

    const char *teks = (no == 1) ? "S I G N   U P" : (no == 2) ? "L O G I N" : "E X I T";
    int textX = (no == 1) ? 16 : (no == 2) ? 51 : 85;

    gotoxy(textX, 19);
    setcolor(txtColor, bgColor);
    printf("%s", teks);
}

/////////////////////////////////////////////////////////////// M E N U ////////////////////////////////////////////////////////////////////////
void menu()
{
    while (1)
    {
        clearScreen();
        bingkai(0, 0, 109, 34, 14, 4);

        setcolor(15, 4);
        gotoxy(17, 4);
        printf("                               Selamat Datang di                            ");
        gotoxy(17, 5);
        setcolor(14, 4);
        printf("                         ==============================                     ");
        gotoxy(17, 7);
        printf(":::::::::::            :::::::::     :::     ::::::::::: :::     :::    ::: ");
        gotoxy(17, 8);
        printf("    :+:                :+:    :+:  :+: :+:       :+:   :+: :+:   :+:   :+:  ");
        gotoxy(17, 9);
        printf("    +:+                +:+    +:+ +:+   +:+      +:+  +:+   +:+  +:+  +:+   ");
        gotoxy(17, 10);
        printf("    +#+                +#++:++#+ +#++:++#++:     +#+ +#++:++#++: +#++:++    ");
        gotoxy(17, 11);
        printf("    +#+                +#+       +#+     +#+     +#+ +#+     +#+ +#+  +#+   ");
        gotoxy(17, 12);
        printf("    #+#                #+#       #+#     #+# #+# #+# #+#     #+# #+#   #+#  ");
        gotoxy(17, 13);
        printf("########### ########## ###       ###     ###  #####  ###     ### ###    ### ");

        setcolor(14, 4);
        gotoxy(27, 25);
        printf(">> Anda belum punya akun? Silahkan ke menu  S I G N   U P  <<");

        setcolor(14, 4);
        gotoxy(35, 27);
        printf("Navigasi: Panah [<- / ->]  |  Pilih: [ENTER]");

        for (int i = 1; i <= 3; i++)
        {
            gambar_tombol_menu(i, i == tekan);
        }

        while (1)
        {
            int ch = getch();
            if (ch == 13)
            {
                break;
            }
            if (ch == 0 || (unsigned char)ch == 224)
            {
                int ch2 = getch();
                int tekan_lama = tekan;
                if (ch2 == 77) // ke kanan
                {
                    if (tekan == 1)      tekan = 2;
                    else if (tekan == 2) tekan = 3;
                    else if (tekan == 3) tekan = 1;
                }
                else if (ch2 == 75) // ke kiri
                {
                    if (tekan == 3)      tekan = 2;
                    else if (tekan == 2) tekan = 1;
                    else if (tekan == 1) tekan = 3;
                }

                if (tekan != tekan_lama)
                {
                    gambar_tombol_menu(tekan_lama, 0);
                    gambar_tombol_menu(tekan, 1);
                }
            }
        }

        if (tekan == 1)
        {
            clearScreen();
            int to_login = signup();
            if (to_login == 1)
            {
                clearScreen();
                int ok = login();
                if (ok == 1)
                {
                    menu_fitur();
                }
            }
        }
        else if (tekan == 2)
        {
            clearScreen();
            int ok = login();
            if (ok == 1)
            {
                menu_fitur();
            }
        }
        else if (tekan == 3)
        {
            exit(0);
        }
    }
}

//////////////////////////////////////////////////////////// S I G N  U P //////////////////////////////////////////////////////////////////////
int signup()
{
    clearScreen();
    bingkai(0, 0, 109, 34, 14, 4);

    char s = 219, // blok █
         t = 187, // kanan atas ╗
         u = 188, // kanan bawah ╝
         v = 186, // lurus atas ║
         x = 205, // lurus garis ═
         w = 201, // kiri atas ╔
         y = 200; // kiri bawah ╚

    setcolor(14, 4);
    gotoxy(30, 3);
    printf("%c%c%c%c%c%c%c%c%c%c%c %c%c%c%c%c%c%c %c%c%c%c%c   %c%c%c    %c%c%c   %c%c%c%c%c%c%c%c%c%c", s, s, s, s, s, s, s, t, s, s, t, s, s, s, s, s, s, t, s, s, s, s, t, s, s, t, s, s, t, s, s, t, s, s, s, s, s, s, s);
    gotoxy(30, 4);
    printf("%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c %c%c%c%c%c  %c%c%c    %c%c%c   %c%c%c%c%c%c%c%c%c%c%c", s, s, w, x, x, x, x, u, s, s, v, s, s, w, x, x, x, x, u, s, s, s, s, t, s, s, v, s, s, v, s, s, v, s, s, w, x, x, s, s, t);
    gotoxy(30, 5);
    printf("%c%c%c%c%c%c%c%c%c%c%c%c%c%c  %c%c%c%c%c%c%c%c%c%c %c%c%c    %c%c%c   %c%c%c%c%c%c%c%c%c%c%c", s, s, s, s, s, s, s, t, s, s, v, s, s, v, s, s, s, t, s, s, w, s, s, t, s, s, v, s, s, v, s, s, v, s, s, s, s, s, s, w, u);
    gotoxy(30, 6);
    printf("%c%c%c%c%c%c%c%c%c%c%c%c%c%c   %c%c%c%c%c%c%c%c%c%c%c%c%c    %c%c%c   %c%c%c%c%c%c%c%c%c%c ", y, x, x, x, x, s, s, v, s, s, v, s, s, v, s, s, v, s, s, v, y, s, s, t, s, s, v, s, s, v, s, s, v, s, s, w, x, x, x, u);
    gotoxy(30, 7);
    printf("%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c %c%c%c%c%c%c    %c%c%c%c%c%c%c%c%c%c%c%c     ", s, s, s, s, s, s, s, v, s, s, v, y, s, s, s, s, s, s, w, u, s, s, v, y, s, s, s, s, v, y, s, s, s, s, s, s, w, u, s, s, v);
    gotoxy(30, 8);
    printf("%c%c%c%c%c%c%c%c%c%c%c %c%c%c%c%c%c%c %c%c%c  %c%c%c%c%c     %c%c%c%c%c%c%c %c%c%c     ", y, x, x, x, x, x, x, u, y, x, u, y, x, x, x, x, x, u, y, x, u, y, x, x, x, u, y, x, x, x, x, x, u, y, x, u);

    bingkai(22, 10, 66, 16, 14, 1);
    isi_bingkai(23, 11, 64, 14, 1, 1);

    setcolor(14, 4);
    gotoxy(34, 27);
    printf("SILAHKAN SELESAIKAN PENDAFTARAN AKUN ANDA");

    setcolor(15, 4);
    gotoxy(5, 32);
    printf("Tekan >> [ESC] << untuk kembali ke Menu Utama");

    setcolor(14, 1);
    gotoxy(26, 12);
    printf("NAMA LENGKAP : ");
    gotoxy(42, 12);
    setcolor(15, 1);
    vnama(nama);
    if (nama[0] == 27)
    {
        nama[0] = '\0';
        return 0;
    }

    setcolor(9, 1);
    gotoxy(26, 14);
    printf("----------------------------------------------------------");

    setcolor(14, 1);
    gotoxy(26, 16);
    printf("NO HANDPHONE : ");
    gotoxy(42, 16);
    setcolor(15, 1);
    vnohp(no_hp);
    if (no_hp[0] == 27)
    {
        no_hp[0] = '\0';
        return 0;
    }

    setcolor(9, 1);
    gotoxy(26, 18);
    printf("----------------------------------------------------------");

    setcolor(14, 1);
    gotoxy(26, 20);
    printf("USERNAME     : ");
    gotoxy(42, 20);
    setcolor(15, 1);
    vusername(user);
    if (user[0] == 27)
    {
        user[0] = '\0';
        return 0;
    }

    setcolor(9, 1);
    gotoxy(26, 22);
    printf("----------------------------------------------------------");

    setcolor(14, 1);
    gotoxy(26, 24);
    printf("PASSWORD     : ");
    gotoxy(42, 24);
    setcolor(15, 1);
    vpass(pass);
    if (pass[0] == 27)
    {
        pass[0] = '\0';
        return 0;
    }

    FILE *ines = fopen("signup.txt", "a");
    if (!ines)
    {
        printf("Gagal membuka file.\n");
        return 0;
    }
    fprintf(ines, "%s/%s/%s/%s\n", nama, no_hp, user, pass);
    fclose(ines);

    setcolor(15, 2);
    gotoxy(33, 27);
    printf("        << DATA BERHASIL DISIMPAN KE DATABASE >>        ");
    gotoxy(39, 28);
    setcolor(14, 4);
    printf("========================================");

    setcolor(14, 4);
    gotoxy(75, 32);
    printf("Tekan >> [ENTER] << ke LOG-IN");

    char inp;
    while (1)
    {
        inp = getch();
        if (inp == 27)
        {
            return 0;
        }
        else if (inp == 13)
        {
            return 1;
        }
    }
}

///////////////////////////////////////////////////////////// L O G I N ////////////////////////////////////////////////////////////////////////
int login()
{
    while (1)
    {
        clearScreen();
        bingkai(0, 0, 109, 34, 14, 4);

        char s = 219, // blok █
             t = 187, // kanan atas ╗
             u = 188, // kanan bawah ╝
             v = 186, // lurus atas ║
             x = 205, // lurus garis ═
             w = 201, // kiri atas ╔
             y = 200; // kiri bawah ╚

        setcolor(14, 4);
        gotoxy(35, 3);
        printf("%c%c%c      %c%c%c%c%c%c%c  %c%c%c%c%c%c%c  %c%c%c%c%c%c%c   %c%c%c", s, s, t, s, s, s, s, s, s, t, s, s, s, s, s, s, t, s, s, t, s, s, s, t, s, s, t);
        gotoxy(35, 4);
        printf("%c%c%c     %c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c  %c%c%c%c%c%c%c%c  %c%c%c", s, s, v, s, s, w, x, x, x, s, s, t, s, s, w, x, x, x, x, u, s, s, v, s, s, s, s, t, s, s, v);
        gotoxy(35, 5);
        printf("%c%c%c     %c%c%c   %c%c%c%c%c%c  %c%c%c%c %c%c%c%c%c%c%c%c%c %c%c%c", s, s, v, s, s, v, s, s, v, s, s, v, s, s, s, t, s, s, v, s, s, w, s, s, t, s, s, v);
        gotoxy(35, 6);
        printf("%c%c%c     %c%c%c   %c%c%c%c%c%c   %c%c%c %c%c%c%c%c%c%c%c%c%c%c%c%c", s, s, v, s, s, v, s, s, v, s, s, v, s, s, v, s, s, v, s, s, v, y, s, s, t, s, s, v);
        gotoxy(35, 7);
        printf("%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c %c%c%c%c%c%c %c%c%c%c%c%c", s, s, s, s, s, s, s, t, y, s, s, s, s, s, s, w, u, y, s, s, s, s, s, s, w, u, s, s, v, s, s, v, y, s, s, s, s, v);
        gotoxy(35, 8);
        printf("%c%c%c%c%c%c%c%c %c%c%c%c%c%c%c  %c%c%c%c%c%c%c  %c%c%c%c%c%c  %c%c%c%c%c", y, x, x, x, x, x, x, u, y, x, x, x, x, x, u, y, x, x, x, x, x, u, y, x, u, y, x, u, y, x, x, x, u);

        bingkai(26, 11, 58, 11, 14, 1);
        isi_bingkai(27, 12, 56, 9, 1, 1);

        char username1[20], password1[20];
        setcolor(15, 4);
        gotoxy(5, 32);
        printf("Tekan >> [ESC] << untuk kembali ke Menu Utama");

        setcolor(14, 1);
        gotoxy(31, 13);
        printf("MASUKKAN AKUN PENGGUNA TERDAFTAR");
        setcolor(9, 1);
        gotoxy(29, 14);
        printf("--------------------------------------------------");

        setcolor(14, 1);
        gotoxy(31, 16);
        printf("USERNAME    : ");
        gotoxy(46, 16);
        setcolor(15, 1);
        vusername(user);
        if (user[0] == 27)
        {
            user[0] = '\0';
            return 0;
        }

        setcolor(9, 1);
        gotoxy(29, 18);
        printf("--------------------------------------------------");

        setcolor(14, 1);
        gotoxy(31, 20);
        printf("PASSWORD    : ");
        gotoxy(46, 20);
        setcolor(15, 1);
        vpass(pass);
        if (pass[0] == 27)
        {
            pass[0] = '\0';
            return 0;
        }

        int ditemukan = 0;
        FILE *nenes = fopen("signup.txt", "r");
        if (nenes != NULL)
        {
            while (fscanf(nenes, "%[^/]/%[^/]/%[^/]/%[^\n]\n", nama, no_hp, username1, password1) != EOF)
            {
                if (strcmp(user, username1) == 0 && strcmp(pass, password1) == 0)
                {
                    ditemukan = 1;
                    break;
                }
            }
            fclose(nenes);
        }

        if (ditemukan == 1)
        {
            setcolor(14, 4);
            gotoxy(62, 32);
            printf("Tekan >> [ENTER] << menuju Menu Fitur");
            for (int i = 1; i < 4; i++)
            {
                setcolor(15, 2);
                gotoxy(40, 24);
                printf(">> L O G I N   B E R H A S I L  <<");
                Sleep(250);
                gotoxy(40, 24);
                printf("                                  ");
                Sleep(150);
            }
            setcolor(15, 2);
            gotoxy(40, 24);
            printf(">> L O G I N   B E R H A S I L  <<");
            getch();
            return 1;
        }
        else
        {
            for (int a = 1; a < 3; a++)
            {
                setcolor(12, 4);
                gotoxy(43, 24);
                printf(">> LOGIN GAGAL: Akun Salah <<");
                Sleep(500);
                gotoxy(43, 24);
                printf("                             ");
                Sleep(250);
            }
            setcolor(14, 4);
            gotoxy(36, 26);
            printf("[ENTER] Coba Lagi  |  [ESC] Menu Utama");
            char cl = getch();
            if (cl == 27)
            {
                return 0;
            }
        }
    }
}

//=============================================================== C R U D (UPDATE) ===========================================================//
void update_profile()
{
    char tam_nama[100], tam_nohp[100], tam_user[100], tam_password[100];
    char s_user[100];
    int sama = 0;
    int pilihan;

    clearScreen();
    bingkai(0, 0, 109, 34, 14, 4);

    setcolor(14, 4);
    gotoxy(24, 2);
    printf("  _     _                   _               _                  ");
    gotoxy(24, 3);
    printf("/  |  /  |                /  |            /  |                 ");
    gotoxy(24, 4);
    printf("$$ |  $$ |    __         _$$ |     ___    $$ |        __       ");
    gotoxy(24, 5);
    printf("$$ |  $$ | /      \\  /    $$ | /      \\ / $$   |   /      \\    ");
    gotoxy(24, 6);
    printf("$$ |  $$ |/$$$$$$  |/$$$$$$$ | $$$$$$  |$$$$$$/   /$$$$$$  |   ");
    gotoxy(24, 7);
    printf("$$ |  $$ |$$ |  $$ |$$ |  $$ | /    $$ |  $$ | __ $$    $$ |   ");
    gotoxy(24, 8);
    printf("$$ \\_ $$ |$$ |$$ | $$ \\_$$ |/$$$$$$$ |  $$ |/  |$$$$$$$$/    ");
    gotoxy(24, 9);
    printf("$$    $$/ $$    $$/ $$    $$ |$$    $$ |  $$  $$/ $$       |   ");
    gotoxy(24, 10);
    printf(" $$$$$$/  $$$$$$$/   $$$$$$$/  $$$$$$$/    $$$$/   $$$$$$$/    ");

    strcpy(s_user, user);
    strcpy(tam_nama, nama);
    strcpy(tam_nohp, no_hp);
    strcpy(tam_user, user);
    strcpy(tam_password, pass);

    bingkai(17, 11, 76, 20, 14, 1);
    isi_bingkai(18, 12, 74, 18, 1, 1);

    setcolor(14, 1);
    gotoxy(46, 12);
    printf("[ PROFIL SAAT INI ]");
    setcolor(9, 1);
    gotoxy(20, 13);
    printf("----------------------------------------------------------------------");
    gotoxy(23, 14);
    setcolor(14, 1);
    printf("NAMA     : ");
    setcolor(15, 1);
    printf("%-20s  ", nama);
    setcolor(14, 1);
    printf("NO HP    : ");
    setcolor(15, 1);
    printf("%s", no_hp);

    gotoxy(23, 15);
    setcolor(14, 1);
    printf("USERNAME : ");
    setcolor(15, 1);
    printf("%-20s  ", user);
    setcolor(14, 1);
    printf("PASSWORD : ");
    setcolor(15, 1);
    printf("%s", pass);

    setcolor(9, 1);
    gotoxy(20, 16);
    printf("----------------------------------------------------------------------");

    setcolor(14, 1);
    gotoxy(23, 18);
    printf("Pilih Bagian Yang Ingin Diperbarui:");
    setcolor(15, 1);
    gotoxy(25, 19);
    printf("1. Nama        2. No HP        3. Username        4. Password");

    setcolor(15, 4);
    gotoxy(5, 32);
    printf("Tekan >> [ESC] << untuk kembali ke Menu Fitur");

    setcolor(11, 1);
    gotoxy(23, 21);
    printf("Masukkan nomor pilihan [1-4]: ");
    pilihan = valpilih_profil();
    if (pilihan <= 0)
    {
        return;
    }

    switch (pilihan)
    {
    case 1:
        gotoxy(23, 23);
        setcolor(14, 1);
        printf("Masukkan Nama Baru     : ");
        setcolor(15, 1);
        vnamaupdate(nama);
        if (nama[0] == 27) { strcpy(nama, tam_nama); return; }
        break;
    case 2:
        gotoxy(23, 23);
        setcolor(14, 1);
        printf("Masukkan No HP Baru    : ");
        setcolor(15, 1);
        vnohp(no_hp);
        if (no_hp[0] == 27) { strcpy(no_hp, tam_nohp); return; }
        break;
    case 3:
        gotoxy(23, 23);
        setcolor(14, 1);
        printf("Masukkan Username Baru : ");
        setcolor(15, 1);
        vusername(user);
        if (user[0] == 27) { strcpy(user, tam_user); return; }
        break;
    case 4:
        gotoxy(23, 23);
        setcolor(14, 1);
        printf("Masukkan Password Baru : ");
        setcolor(15, 1);
        vpass(pass);
        if (pass[0] == 27) { strcpy(pass, tam_password); return; }
        break;
    default:
        return;
    }

    FILE *ptr = fopen("signup.txt", "r");
    FILE *tamp_file = fopen("tamp.txt", "w");

    if (ptr != NULL && tamp_file != NULL)
    {
        while (fscanf(ptr, "%[^/]/%[^/]/%[^/]/%[^\n]\n", tam_nama, tam_nohp, tam_user, tam_password) != EOF)
        {
            if (strcmp(s_user, tam_user) == 0)
            {
                sama = 1;
                fprintf(tamp_file, "%s/%s/%s/%s\n", nama, no_hp, user, pass);
            }
            else
            {
                fprintf(tamp_file, "%s/%s/%s/%s\n", tam_nama, tam_nohp, tam_user, tam_password);
            }
        }
        fclose(ptr);
        fclose(tamp_file);
    }

    if (sama == 1)
    {
        remove("signup.txt");
        rename("tamp.txt", "signup.txt");
        setcolor(15, 2);
        gotoxy(38, 26);
        printf(">> U P D A T E   S U K S E S <<");
        setcolor(14, 1);
        gotoxy(32, 28);
        printf("Tekan sembarang tombol ke Menu Fitur");
        getch();
        return;
    }
    else
    {
        remove("tamp.txt");
        setcolor(12, 1);
        gotoxy(34, 26);
        printf(">> Akun tidak ditemukan di database! <<");
        Sleep(1000);
        return;
    }
}

//=========================================================== C R U D (DELETE) ===============================================================//
int delete_profile()
{
    char tam_nama[100], tam_nohp[100], tam_user[100], tam_password[100];
    char s_user[100];
    int sama = 0;

    clearScreen();
    bingkai(0, 0, 109, 34, 14, 4);

    setcolor(14, 4);
    gotoxy(23, 2);
    printf("  _     _                   _                _                  ");
    gotoxy(23, 3);
    printf("/  |  /  |                /  |             /  |                 ");
    gotoxy(23, 4);
    printf("$$ |  $$ |    __         _$$ |     ___     $$ |        __       ");
    gotoxy(23, 5);
    printf("$$ |  $$ | /      \\  /    $$ | /      \\ / $$   |   /      \\    ");
    gotoxy(23, 6);
    printf("$$ |  $$ |/$$$$$$  |/$$$$$$$ | $$$$$$  |$$$$$$/   /$$$$$$  |   ");
    gotoxy(23, 7);
    printf("$$ |  $$ |$$ |  $$ |$$ |  $$ | /    $$ |  $$ | __ $$    $$ |   ");
    gotoxy(23, 8);
    printf("$$ \\_ $$ |$$ |$$ |$$ \\_$$ |/$$$$$$$ |  $$ |/  |$$$$$$$$/    ");
    gotoxy(23, 9);
    printf("$$    $$/ $$    $$/ $$    $$ |$$    $$ |  $$  $$/ $$       |   ");
    gotoxy(23, 10);
    printf(" $$$$$$/  $$$$$$$/   $$$$$$$/  $$$$$$$/    $$$$/   $$$$$$$/    ");

    strcpy(s_user, user);

    bingkai(21, 12, 68, 18, 14, 1);
    isi_bingkai(22, 13, 66, 16, 1, 1);

    setcolor(14, 1);
    gotoxy(46, 14);
    printf("[ PROFIL SAAT INI ]");
    setcolor(9, 1);
    gotoxy(24, 15);
    printf("--------------------------------------------------------------");

    gotoxy(28, 17);
    setcolor(14, 1);
    printf("NAMA     : ");
    setcolor(15, 1);
    printf("%s", nama);

    gotoxy(28, 18);
    setcolor(14, 1);
    printf("NO HP    : ");
    setcolor(15, 1);
    printf("%s", no_hp);

    gotoxy(28, 19);
    setcolor(14, 1);
    printf("USERNAME : ");
    setcolor(15, 1);
    printf("%s", user);

    gotoxy(28, 20);
    setcolor(14, 1);
    printf("PASSWORD : ");
    setcolor(15, 1);
    printf("%s", pass);

    setcolor(9, 1);
    gotoxy(24, 21);
    printf("--------------------------------------------------------------");

    setcolor(15, 4);
    gotoxy(5, 32);
    printf("Tekan >> [ESC] << untuk kembali ke Menu Fitur");

    setcolor(12, 1);
    gotoxy(28, 23);
    printf("Apakah Anda yakin ingin menghapus akun ini? (y/t): ");

    char confir = getch();
    if (confir != 'y' && confir != 'Y')
    {
        gotoxy(28, 25);
        setcolor(14, 1);
        printf("Penghapusan akun dibatalkan.");
        Sleep(600);
        return 0;
    }

    FILE *ptr = fopen("signup.txt", "r");
    FILE *tamp_file = fopen("tamp.txt", "w");

    if (ptr != NULL && tamp_file != NULL)
    {
        while (fscanf(ptr, "%[^/]/%[^/]/%[^/]/%[^\n]\n", tam_nama, tam_nohp, tam_user, tam_password) != EOF)
        {
            if (strcmp(s_user, tam_user) != 0)
            {
                fprintf(tamp_file, "%s/%s/%s/%s\n", tam_nama, tam_nohp, tam_user, tam_password);
            }
            else
            {
                sama = 1;
            }
        }
        fclose(ptr);
        fclose(tamp_file);
    }

    if (sama == 1)
    {
        remove("signup.txt");
        rename("tamp.txt", "signup.txt");
        user[0] = '\0';
        pass[0] = '\0';
        nama[0] = '\0';
        no_hp[0] = '\0';
        pajakwp[0] = '\0';
        tekan = 1;
        setcolor(15, 2);
        gotoxy(37, 26);
        printf(">> A K U N   B E R H A S I L   D I H A P U S <<");
        Sleep(1200);
        return 1;
    }
    else
    {
        remove("tamp.txt");
        setcolor(12, 1);
        gotoxy(34, 26);
        printf(">> Akun tidak ditemukan di database! <<");
        Sleep(1000);
        return 0;
    }
}

//===========================================================================================================================================//
/////////////////////////////////////////////////////////////// F I T U R /////////////////////////////////////////////////////////////////////
//===========================================================================================================================================//
void gambar_tombol_fitur(int no, int aktif)
{
    int x, y;
    char teks[40];
    if (no == 1)      { x = 10; y = 11; strcpy(teks, "1.  I N F O   P A J A K"); }
    else if (no == 2) { x = 10; y = 17; strcpy(teks, "2.  H I T U N G   P P H   2 1"); }
    else if (no == 3) { x = 10; y = 23; strcpy(teks, "3.  L A P O R   S P T"); }
    else if (no == 4) { x = 58; y = 11; strcpy(teks, "4.  D A T A   H I S T O R I"); }
    else if (no == 5) { x = 58; y = 17; strcpy(teks, "5.  U P D A T E   P R O F I L"); }
    else if (no == 6) { x = 58; y = 23; strcpy(teks, "6.  D E L E T E   P R O F I L E"); }

    int borderColor = aktif ? 14 : 2;
    int bgColor     = aktif ? 2  : 1;
    int txtColor    = aktif ? 15 : 7;

    bingkai(x, y, 42, 4, borderColor, bgColor);
    isi_bingkai(x + 1, y + 1, 40, 2, bgColor, bgColor);

    int len = (int)strlen(teks);
    int textX = x + (42 - len) / 2;
    int textY = y + 2;

    setcolor(txtColor, bgColor);
    gotoxy(textX, textY);
    if (aktif)
    {
        printf("%s", teks);
    }
    else
    {
        printf("%s", teks);
    }
}

void menu_fitur()
{
    int jalan = 1;
    int pilih = 1;

    while (jalan)
    {
        clearScreen();
        bingkai(0, 0, 109, 34, 14, 4);

        setcolor(14, 4);
        gotoxy(19, 2);
        printf("   __  __ ______ _   _ _    _   ______ _____ _______ _    _ _____   ");
        gotoxy(19, 3);
        printf("  |  \\/  |  ____| \\ | | |  | | |  ____|_   _|__   __| |  | |  __ \\  ");
        gotoxy(19, 4);
        printf("  | \\  / | |__  |  \\| | |  | | | |__    | |    | |  | |  | | |__) | ");
        gotoxy(19, 5);
        printf("  | |\\/| |  __| | . ` | |  | | |  __|   | |    | |  | |  | |  _  /  ");
        gotoxy(19, 6);
        printf("  | |  | | |____| |\\  | |__| | | |     _| |_   | |  | |__| | | \\ \\  ");
        gotoxy(19, 7);
        printf("  |_|  |_|______|_| \\_|\\____/  |_|    |_____|  |_|   \\____/|_|  \\_\\ ");

        setcolor(15, 4);
        gotoxy(28, 9);
        printf("Gunakan Tombol Panah [<-] [->] [Atas] [Bawah] lalu Tekan [ENTER]");

        setcolor(14, 4);
        gotoxy(5, 32);
        printf("Tekan >> [ESC] << Log Out Akun          Navigasi: [Panah] | Pilih: [ENTER]");

        int aksi_pilih = 0;
        for (int i = 1; i <= 6; i++)
        {
            gambar_tombol_fitur(i, i == pilih);
        }

        while (1)
        {
            inp = getch();
            if (inp == 0 || inp == 224)
            {
                int pilih_lama = pilih;
                inp = getch();
                if (inp == 80) // Bawah
                {
                    if (pilih == 1) pilih = 2;
                    else if (pilih == 2) pilih = 3;
                    else if (pilih == 3) pilih = 1;
                    else if (pilih == 4) pilih = 5;
                    else if (pilih == 5) pilih = 6;
                    else if (pilih == 6) pilih = 4;
                }
                else if (inp == 72) // Atas
                {
                    if (pilih == 1) pilih = 3;
                    else if (pilih == 2) pilih = 1;
                    else if (pilih == 3) pilih = 2;
                    else if (pilih == 4) pilih = 6;
                    else if (pilih == 5) pilih = 4;
                    else if (pilih == 6) pilih = 5;
                }
                else if (inp == 77) // Kanan
                {
                    if (pilih <= 3) pilih += 3;
                    else pilih -= 3;
                }
                else if (inp == 75) // Kiri
                {
                    if (pilih > 3) pilih -= 3;
                    else pilih += 3;
                }

                if (pilih != pilih_lama)
                {
                    gambar_tombol_fitur(pilih_lama, 0);
                    gambar_tombol_fitur(pilih, 1);
                }
            }
            else if (inp == 13) // Enter
            {
                aksi_pilih = pilih;
                break;
            }
            else if (inp == 27) // ESC: Konfirmasi Log Out
            {
                clearScreen();
                bingkai(0, 0, 109, 34, 14, 4);
                bingkai(32, 12, 46, 9, 14, 1);
                isi_bingkai(33, 13, 44, 7, 1, 1);
                setcolor(14, 1);
                gotoxy(45, 14);
                printf("[ KONFIRMASI LOG OUT ]");
                setcolor(9, 1);
                gotoxy(35, 15);
                printf("----------------------------------------");
                setcolor(15, 1);
                gotoxy(36, 17);
                printf("Apakah Anda yakin ingin Log Out? (y/t): ");
                char c = getch();
                if (c == 'y' || c == 'Y')
                {
                    setcolor(15, 2);
                    gotoxy(44, 19);
                    printf("  >> LOG OUT BERHASIL <<  ");
                    Sleep(800);
                    memset(user, 0, sizeof(user));
                    memset(pass, 0, sizeof(pass));
                    memset(nama, 0, sizeof(nama));
                    memset(no_hp, 0, sizeof(no_hp));
                    pajakwp[0] = '\0';
                    tekan = 1;
                    jalan = 0;
                    break;
                }
                else
                {
                    break;
                }
            }
        }

        if (jalan && aksi_pilih > 0)
        {
            clearScreen();
            switch (aksi_pilih)
            {
            case 1:
                info_pph();
                break;
            case 2:
                hitpph();
                break;
            case 3:
                pajakwp[0] = '\0';
                laporspt();
                break;
            case 4:
                data_histori();
                break;
            case 5:
                update_profile();
                break;
            case 6:
            {
                int res = delete_profile();
                if (res == 1)
                {
                    jalan = 0;
                }
                break;
            }
            }
        }
    }
}

////////////////////////////////////////////////////// Informasi Tentang PPh Pasal 21 /////////////////////////////////////////////////////////
void info_pph()
{
    clearScreen();
    bingkai(0, 0, 109, 34, 14, 4);

    bingkai(5, 2, 99, 29, 14, 1);
    isi_bingkai(6, 3, 97, 27, 1, 1);

    setcolor(14, 1);
    gotoxy(27, 3);
    printf("INFORMASI & KETENTUAN PAJAK PENGHASILAN (PPH) PASAL 21");
    setcolor(9, 1);
    gotoxy(7, 4);
    printf("===============================================================================================");

    setcolor(15, 1);
    gotoxy(8, 5);
    printf("PPh Pasal 21 adalah pemotongan pajak atas penghasilan sehubungan dengan pekerjaan, jasa,");
    gotoxy(8, 6);
    printf("atau kegiatan yang diterima Wajib Pajak Orang Pribadi dalam negeri.");
    setcolor(11, 1);
    gotoxy(8, 7);
    printf("Aplikasi ini menghitung PPh 21 perorangan dan pelaporan formulir SPT 1721 A1.");

    setcolor(9, 1);
    gotoxy(7, 8);
    printf("================================= KETENTUAN HITUNG PAJAK ======================================");

    gotoxy(8, 9);
    setcolor(14, 1);
    printf("a. Penghasilan Bruto : ");
    setcolor(15, 1);
    printf("Gaji sebulan + lembur (overtime). Syarat bruto harus > Rp. 4.500.000,00");

    gotoxy(8, 10);
    setcolor(14, 1);
    printf("b. Penghasilan Netto : ");
    setcolor(15, 1);
    printf("Penghasilan Bruto dikurangi Biaya Jabatan dan Iuran Pensiun.");

    gotoxy(8, 11);
    setcolor(14, 1);
    printf("c. Biaya Jabatan    : ");
    setcolor(15, 1);
    printf("PMK 250/2008 = 5%% dari bruto, setinggi-tingginya Rp 500.000,00 sebulan.");

    gotoxy(8, 12);
    setcolor(14, 1);
    printf("d. Iuran Pensiun    : ");
    setcolor(15, 1);
    printf("Maksimal Rp 200.000,00 sebulan.");

    gotoxy(8, 13);
    setcolor(14, 1);
    printf("e. Ketentuan PTKP (PMK No. 101/PMK.010/2016):");

    gotoxy(11, 14);
    setcolor(15, 1);
    printf("- WP Orang Pribadi : Rp 54.000.000,00         - Status Menikah : +Rp 4.500.000,00");

    gotoxy(11, 15);
    setcolor(15, 1);
    printf("- Tanggungan Anak  : Rp  4.500.000,00 / anak (maksimal 3 anak)");

    gotoxy(8, 16);
    setcolor(14, 1);
    printf("f. Tarif PPh 21 Berdasarkan UU Harmonisasi Peraturan Perpajakan (UU HPP):");

    gotoxy(11, 17);
    setcolor(15, 1);
    printf("1. Penghasilan Rp 0 s.d. Rp 60.000.000                         : Tarif PPh  5%%");

    gotoxy(11, 18);
    setcolor(15, 1);
    printf("2. Penghasilan > Rp 60.000.000 s.d. Rp 250.000.000             : Tarif PPh 15%%");

    gotoxy(11, 19);
    setcolor(15, 1);
    printf("3. Penghasilan > Rp 250.000.000 s.d. Rp 500.000.000            : Tarif PPh 25%%");

    gotoxy(11, 20);
    setcolor(15, 1);
    printf("4. Penghasilan > Rp 500.000.000 s.d. Rp 5.000.000.000          : Tarif PPh 30%%");

    gotoxy(11, 21);
    setcolor(15, 1);
    printf("5. Penghasilan > Rp 5.000.000.000                              : Tarif PPh 35%%");

    setcolor(9, 1);
    gotoxy(7, 23);
    printf("===============================================================================================");

    setcolor(14, 4);
    gotoxy(5, 32);
    printf("Tekan >> [ESC] << untuk kembali ke Menu Fitur");

    char inp;
    while (1)
    {
        inp = getch();
        if (inp == 27)
        {
            return;
        }
        else
        {
            printf("\a");
        }
    }
}

////////////////////////////////////////////////////////////// Hitung Pajak ////////////////////////////////////////////////////////////////////
void hitpph()
{
    while (1)
    {
        clearScreen();
        bingkai(0, 0, 109, 34, 14, 4);

        setcolor(14, 4);
        gotoxy(15, 2);
        printf("  _    _ _____ _______ _    _ _   _  _____   _____  _____  _    _   ___  __   ");
        gotoxy(15, 3);
        printf(" | |  | |_   _|__   __| |  | | \\ | |/ ____| |  __ \\|  __ \\| |  | | |__ \\/_ |  ");
        gotoxy(15, 4);
        printf(" | |__| | | |    | |  | |  | |  \\| | |  __  | |__) | |__) | |__| |    ) || |  ");
        gotoxy(15, 5);
        printf(" |  __  | | |    | |  | |  | | . ` | | |_ | |  ___/|  ___/|  __  |   / / | |  ");
        gotoxy(15, 6);
        printf(" | |  | |_| |_   | |  | |__| | |\\  | |__| | | |    | |    | |  | |  / /_ | |  ");
        gotoxy(15, 7);
        printf(" |_|  |_|_____|  |_|   \\____/|_| \\_|\\_____| |_|    |_|    |_|  |_| |____||_|  ");

        bingkai(5, 8, 100, 23, 14, 1);
        isi_bingkai(6, 9, 98, 21, 1, 1);

        setcolor(14, 4);
        gotoxy(5, 32);
        printf("Tekan [ESC] ke Menu Fitur   |   Tekan [0] untuk Hitung Ulang");

        setcolor(14, 1);
        gotoxy(8, 9);
        printf("[ 1. PENGHASILAN BRUTO ]");

        setcolor(15, 1);
        gotoxy(9, 10);
        printf("Gaji Pokok per bulan                                    : Rp. ");
        setcolor(15, 1);
        strcpy(gaji, valid_gaji(73, 10, 15));
        if (gaji[0] == 27)
        {
            return;
        }
        gajiperbulan = atoll(gaji);

        gotoxy(9, 11);
        setcolor(15, 1);
        printf("Uang Lembur / Overtime (tekan ENTER jika 0)              : Rp. ");
        char lembur_str[20];
        strcpy(lembur_str, valid_lembur(73, 11, 15));
        if (lembur_str[0] == 27)
        {
            return;
        }
        gaji_lembur = atoll(lembur_str);

        pbruto = gajiperbulan + gaji_lembur;

        gotoxy(9, 12);
        setcolor(15, 1);
        printf("Total Penghasilan Bruto (Gaji + Lembur)                 : Rp. %lld", pbruto);

        setcolor(9, 1);
        gotoxy(8, 13);
        printf("--------------------------------------------------------------------------------------------");

        if (pbruto <= 4500000)
        {
            setcolor(12, 1);
            gotoxy(44, 16);
            printf(">> TIDAK MEMENUHI SYARAT <<");
            gotoxy(28, 17);
            printf("Penghasilan Bruto harus lebih dari Rp. 4.500.000,00");
            do
            {
                setcolor(14, 1);
                gotoxy(36, 19);
                printf("Kembali ke Menu Fitur (y/t): ");
                balik = getche();
                if (balik == 'Y' || balik == 'y')
                {
                    return;
                }
                else if (balik == 'T' || balik == 't')
                {
                    break;
                }
                else
                {
                    setcolor(12, 1);
                    gotoxy(67, 19);
                    printf("I N V A L I D");
                    Sleep(500);
                    gotoxy(67, 19);
                    printf("             ");
                }
            } while (1);
        }
        else
        {
            setcolor(14, 1);
            gotoxy(8, 14);
            printf("[ 2. PENGURANGAN PENGHASILAN ]");

            biayajabatan = (long long)(pbruto * 0.05);
            if (biayajabatan > 500000)
            {
                biayajabatan = 500000;
            }
            setcolor(15, 1);
            gotoxy(9, 15);
            printf("Biaya Jabatan (5%% dari Bruto, maks Rp 500.000)          : Rp. %lld", biayajabatan);

            setcolor(15, 1);
            gotoxy(9, 16);
            printf("Iuran Pensiun (maksimal Rp 200.000 per bulan)           : Rp. ");
            int iuran_esc = 0;
            do
            {
                strcpy(iuran_bulan, valid_iuran(73, 16, 15));
                if (iuran_bulan[0] == 27)
                {
                    iuran_esc = 1;
                    break;
                }
                iuranpensiun = atoll(iuran_bulan);
                if (iuranpensiun > 200000)
                {
                    setcolor(12, 1);
                    gotoxy(73, 16);
                    printf("                             ");
                    gotoxy(73, 16);
                    printf("Max Rp 200.000!              ");
                    Sleep(500);
                    gotoxy(73, 16);
                    printf("                             ");
                    setcolor(15, 1);
                }
            } while (iuranpensiun > 200000);

            if (iuran_esc)
            {
                return;
            }

            totalpengurangan = biayajabatan + iuranpensiun;
            setcolor(11, 1);
            gotoxy(9, 17);
            printf("Total Pengurangan (Biaya Jabatan + Iuran Pensiun)       : (Rp. %lld)", totalpengurangan);

            setcolor(9, 1);
            gotoxy(8, 18);
            printf("--------------------------------------------------------------------------------------------");

            netto = pbruto - totalpengurangan;
            setcolor(15, 1);
            gotoxy(9, 19);
            printf("Penghasilan Netto Sebulan                               : Rp. %lld", netto);
            netto_setahun = 12 * netto;
            gotoxy(9, 20);
            printf("Penghasilan Netto Setahun (12 x Netto Sebulan)          : Rp. %lld", netto_setahun);

            setcolor(14, 1);
            gotoxy(9, 21);
            printf("Pilih PTKP: 1.TK/0  2.TK/1  3.TK/2  4.TK/3  ||  5.K/0  6.K/1  7.K/2  8.K/3");
            int ptkp_esc = 0;
            do
            {
                setcolor(15, 1);
                gotoxy(9, 22);
                printf("Masukkan Pilihan Status [1-8]: ");
                swp = getche();
                if (swp == 27)
                {
                    ptkp_esc = 1;
                    break;
                }
                else if (swp == '1')
                {
                    ptkp = 54000000;
                    break;
                }
                else if (swp == '2' || swp == '5')
                {
                    ptkp = 54000000 + 4500000;
                    break;
                }
                else if (swp == '3' || swp == '6')
                {
                    ptkp = 54000000 + (2 * 4500000);
                    break;
                }
                else if (swp == '4' || swp == '7')
                {
                    ptkp = 54000000 + (3 * 4500000);
                    break;
                }
                else if (swp == '8')
                {
                    ptkp = 54000000 + (4 * 4500000);
                    break;
                }
                else
                {
                    setcolor(12, 1);
                    gotoxy(42, 22);
                    printf("I N V A L I D");
                    Sleep(500);
                    gotoxy(42, 22);
                    printf("             ");
                }
            } while (1);

            if (ptkp_esc)
            {
                return;
            }

            setcolor(14, 1);
            gotoxy(45, 22);
            printf("|  Total PTKP: (Rp. %lld)", ptkp);

            setcolor(9, 1);
            gotoxy(8, 23);
            printf("--------------------------------------------------------------------------------------------");

            pkp = netto_setahun - ptkp;
            if (pkp < 0) pkp = 0;

            setcolor(15, 1);
            gotoxy(9, 24);
            printf("Penghasilan Kena Pajak (PKP = Netto Setahun - PTKP)     : Rp. %lld", pkp);

            long long pajaktotal = 0;
            if (pkp == 0)
            {
                pajaktotal = 0;
            }
            else if (pkp <= 60000000)
            {
                pajaktotal = (long long)(pkp * 0.05);
            }
            else if (pkp <= 250000000)
            {
                pajaktotal = (long long)(60000000 * 0.05 + (pkp - 60000000) * 0.15);
            }
            else if (pkp <= 500000000)
            {
                pajaktotal = (long long)(60000000 * 0.05 + 190000000 * 0.15 + (pkp - 250000000) * 0.25);
            }
            else if (pkp <= 5000000000LL)
            {
                pajaktotal = (long long)(60000000 * 0.05 + 190000000 * 0.15 + 250000000 * 0.25 + (pkp - 500000000) * 0.30);
            }
            else
            {
                pajaktotal = (long long)(60000000 * 0.05 + 190000000 * 0.15 + 250000000 * 0.25 + 4500000000LL * 0.30 + (pkp - 5000000000LL) * 0.35);
            }

            setcolor(10, 1);
            gotoxy(9, 26);
            printf(">> PPh Pasal 21 Terutang Setahun                        : Rp. %lld", pajaktotal);
            gotoxy(9, 27);
            printf(">> PPh Pasal 21 Terutang Sebulan                        : Rp. %lld", pajaktotal / 12);

            pajakpph = pajaktotal;

            setcolor(14, 4);
            gotoxy(5, 32);
            printf("Tekan [L] Lapor SPT Hasil Ini   |   [0] Hitung Ulang   |   [ESC] Menu Fitur               ");

            char inp;
            int hitung_ulang = 0;
            do
            {
                inp = getch();
                if (inp == 27)
                {
                    return;
                }
                else if (inp == '0')
                {
                    hitung_ulang = 1;
                    break;
                }
                else if (inp == 'l' || inp == 'L')
                {
                    sprintf(pajakwp, "%lld", pajaktotal);
                    laporspt();
                    return;
                }
                else
                {
                    printf("\a");
                }
            } while (1);

            if (!hitung_ulang)
            {
                return;
            }
        }
    }
}

///////////////////////////////////////////////////////////// Lapor SPT //////////////////////////////////////////////////////////////////////
void laporspt()
{
    clearScreen();
    bingkai(0, 0, 109, 34, 14, 4);

    setcolor(14, 4);
    gotoxy(23, 2);
    printf("  _            _____   ____  _____     _____ _____ _______   ");
    gotoxy(23, 3);
    printf(" | |   /\\     |  __ \\ / __ \\|  __ \\   / ____|  __ \\__   __|  ");
    gotoxy(23, 4);
    printf(" | |  /  \\    | |__) | |  | | |__) | | (___ | |__) | | |     ");
    gotoxy(23, 5);
    printf(" | | / /\\ \\   |  ___/| |  | |  _  /   \\___ \\|  ___/  | |     ");
    gotoxy(23, 6);
    printf(" | |/ ____ \\  | |    | |__| | | \\ \\   ____) | |      | |     ");
    gotoxy(23, 7);
    printf(" |_/_/    \\_\\_|_|     \\____/|_|  \\_\\ |_____/|_|      |_|     ");

    // Kotak Kiri (Formulir)
    bingkai(6, 9, 62, 21, 14, 1);
    isi_bingkai(7, 10, 60, 19, 1, 1);

    // Kotak Kanan (Status & Aksi)
    bingkai(72, 9, 32, 21, 2, 1);
    isi_bingkai(73, 10, 30, 19, 1, 1);

    setcolor(14, 1);
    gotoxy(17, 11);
    printf("FORMULIR IDENTITAS WAJIB PAJAK (1721 A1)");
    setcolor(9, 1);
    gotoxy(9, 12);
    printf("--------------------------------------------------------");

    setcolor(14, 1);
    gotoxy(10, 14);
    printf("Nama Wajib Pajak    : ");

    gotoxy(10, 16);
    printf("NPWP Wajib Pajak    : ");

    gotoxy(10, 18);
    printf("Kode Wajib Pajak    : 1. 1101.411.01.1 (Orang Pribadi)");
    gotoxy(10, 19);
    printf("                      2. 1101.411.01.2 (Badan/Masa)");
    gotoxy(10, 21);
    printf("Pilih Kode WP (1/2) : ");

    gotoxy(10, 24);
    printf("Tanggal Pelaporan   : ");

    gotoxy(10, 26);
    printf("Pajak Yang Terutang : Rp. ");

    setcolor(14, 1);
    gotoxy(79, 11);
    printf("STATUS PELAPORAN");
    setcolor(9, 1);
    gotoxy(75, 12);
    printf("----------------------------");
    setcolor(11, 1);
    gotoxy(76, 16);
    printf("Silahkan isi formulir");
    gotoxy(76, 17);
    printf("di sebelah kiri.");

    setcolor(15, 4);
    gotoxy(5, 32);
    printf("Tekan >> [ESC] << untuk kembali ke Menu Fitur");

    // Input data
    if (strlen(nama) > 0)
    {
        // Bersihkan area status di kotak kanan
        for (int b = 14; b <= 22; b++)
        {
            gotoxy(74, b);
            printf("                            ");
        }

        setcolor(14, 1);
        gotoxy(75, 14);
        printf("Akun Login Terdeteksi:");
        setcolor(15, 2);
        gotoxy(75, 16);
        printf(" %-24.24s ", nama);
        setcolor(14, 1);
        gotoxy(75, 18);
        printf("Gunakan nama akun ini?");
        setcolor(15, 1);
        gotoxy(75, 20);
        printf("[Y] / [ENTER] : Ya");
        gotoxy(75, 21);
        printf("[T]           : Ketik Baru");
        gotoxy(75, 22);
        printf("[ESC]         : Menu Fitur");

        char cp;
        int used_account = 0;
        do
        {
            cp = getch();
            if (cp == 'y' || cp == 'Y' || cp == 13)
            {
                strcpy(namawp, nama);
                used_account = 1;
                break;
            }
            else if (cp == 't' || cp == 'T')
            {
                used_account = 0;
                break;
            }
            else if (cp == 27)
            {
                pajakwp[0] = '\0';
                return;
            }
            else
            {
                printf("\a");
            }
        } while (1);

        // Bersihkan kembali kotak kanan ke panduan umum
        for (int b = 14; b <= 22; b++)
        {
            gotoxy(74, b);
            printf("                            ");
        }
        setcolor(11, 1);
        gotoxy(76, 16);
        printf("Silahkan isi formulir");
        gotoxy(76, 17);
        printf("di sebelah kiri.");

        if (used_account)
        {
            gotoxy(34, 14);
            setcolor(15, 1);
            printf("%s", namawp);
        }
        else
        {
            gotoxy(34, 14);
            setcolor(15, 1);
            val_nwp(namawp);
            if (namawp[0] == 27)
            {
                pajakwp[0] = '\0';
                return;
            }
        }
    }
    else
    {
        gotoxy(34, 14);
        setcolor(15, 1);
        val_nwp(namawp);
        if (namawp[0] == 27)
        {
            pajakwp[0] = '\0';
            return;
        }
    }

    valnpwp(34, 16, 15, npwpspt);
    if (npwpspt[0] == 27)
    {
        pajakwp[0] = '\0';
        return;
    }

    do
    {
        gotoxy(34, 21);
        setcolor(15, 1);
        pilih = getche();
        if (pilih == '1')
        {
            gotoxy(34, 21);
            printf("1101.411.01.1");
            strcpy(kodewp, "1101.411.01.1");
            break;
        }
        else if (pilih == '2')
        {
            gotoxy(34, 21);
            printf("1101.411.01.2");
            strcpy(kodewp, "1101.411.01.2");
            break;
        }
        else if (pilih == 27)
        {
            pajakwp[0] = '\0';
            return;
        }
        else
        {
            setcolor(12, 1);
            gotoxy(34, 21);
            printf("Invalid!     ");
            Sleep(500);
            gotoxy(34, 21);
            printf("             ");
        }
    } while (1);

    time_t ayeuna = time(NULL);
    struct tm waktu = *localtime(&ayeuna);
    char hari[][10] = {"Minggu", "Senin", "Selasa", "Rabu", "Kamis", "Jumat", "Sabtu"};
    char bulan[][12] = {"Januari", "Februari", "Maret", "April", "Mei", "Juni", "Juli", "Agustus", "September", "Oktober", "November", "Desember"};

    char tgl_lengkap[60];
    sprintf(tgl_lengkap, "%s, %d %s %d", hari[waktu.tm_wday], waktu.tm_mday, bulan[waktu.tm_mon], 1900 + waktu.tm_year);
    gotoxy(34, 24);
    setcolor(15, 1);
    printf("%s", tgl_lengkap);

    if (strlen(pajakwp) > 0)
    {
        gotoxy(37, 26);
        setcolor(15, 1);
        printf("%s ", pajakwp);
        Sleep(700);
    }
    else
    {
        valpajakterutang(37, 26, 13, pajakwp);
        if (pajakwp[0] == 27)
        {
            pajakwp[0] = '\0';
            return;
        }
    }

    // Simpan ke file data.txt
    FILE *nesi = fopen("data.txt", "a");
    if (nesi != NULL)
    {
        fprintf(nesi, "%s/%s/%s/%s/%s\n", namawp, npwpspt, kodewp, tgl_lengkap, pajakwp);
        fclose(nesi);
    }
    pajakwp[0] = '\0'; // Reset agar pelaporan SPT berikutnya bersih kembali

    // Bersihkan teks lama di kotak kanan dengan latar Bean
    setcolor(15, 1);
    for (int baris = 14; baris <= 18; baris++)
    {
        gotoxy(74, baris);
        printf("                            ");
    }

    // Update kotak kanan (status berhasil) terpusat rapi dan tidak bocor ke border
    setcolor(15, 2);
    gotoxy(77, 14);
    printf("  [ DATA TERSIMPAN ]  ");

    setcolor(15, 1);
    gotoxy(75, 16);
    printf("Laporan SPT berhasil");
    gotoxy(76, 17);
    printf("masuk ke database!");

    setcolor(14, 1);
    gotoxy(76, 20);
    printf("Ingin lihat histori?");
    gotoxy(76, 22);
    printf("Tekan >> [ENTER] <<");
    setcolor(15, 1);
    gotoxy(77, 24);
    printf("atau tekan [ESC]");
    gotoxy(77, 25);
    printf("ke Menu Fitur");

    char inp;
    while (1)
    {
        inp = getch();
        if (inp == 27)
        {
            return;
        }
        else if (inp == 13)
        {
            data_histori();
            return;
        }
    }
}

///////////////////////////////////////////////////////// HISTORI KOSONG /////////////////////////////////////////////////////////////
void histori_kosong()
{
    isi_bingkai(14, 10, 82, 19, 1, 1);

    setcolor(14, 1);
    gotoxy(39, 14);
    printf("[ BELUM ADA DATA PELAPORAN SPT ]");

    setcolor(11, 1);
    gotoxy(37, 16);
    printf("Database pelaporan SPT masih kosong.");
    gotoxy(32, 17);
    printf("Silahkan buat laporan SPT baru terlebih dahulu.");

    setcolor(9, 1);
    gotoxy(25, 19);
    printf("------------------------------------------------------------");

    setcolor(15, 2);
    gotoxy(34, 21);
    printf("  Tekan [ENTER] : Buat Laporan SPT Baru  ");

    setcolor(14, 1);
    gotoxy(37, 23);
    printf("Tekan [ESC]   : Kembali ke Menu Fitur");

    char inp_empty;
    do
    {
        inp_empty = getch();
        if (inp_empty == 13)
        {
            pajakwp[0] = '\0';
            laporspt();
            return;
        }
        else if (inp_empty == 27)
        {
            return;
        }
        else
        {
            printf("\a");
        }
    } while (1);
}

//////////////////////////////////////////////////////////////// DATA HISTORI //////////////////////////////////////////////////////////////////
void data_histori()
{
    clearScreen();
    bingkai(0, 0, 109, 34, 14, 4);

    setcolor(14, 4);
    gotoxy(17, 2);
    printf("  _____         _______       _    _ _____  _____ _______ ____  _____  _____   ");
    gotoxy(17, 3);
    printf(" |  __ \\     /\\|__   __|/\\   | |  | |_   _|/ ____|__   __/ __ \\|  __ \\|_   _|  ");
    gotoxy(17, 4);
    printf(" | |  | |   /  \\  | |  /  \\  | |__| | | | | (___    | | | |  | | |__) | | |    ");
    gotoxy(17, 5);
    printf(" |  __  |  / /\\ \\ | | / /\\ \\ |  __  | | |  \\___ \\   | | | |  | |  _  /  | |    ");
    gotoxy(17, 6);
    printf(" | |__| | / ____ \\| |/ ____ \\| |  | |_| |_ ____) |  | | | |__| | | \\ \\ _| |_   ");
    gotoxy(17, 7);
    printf(" |_____/ /_/    \\_\\_/_/    \\_\\_|  |_|_____|_____/   |_|  \\____/|_|  \\_\\_____|  ");

    bingkai(13, 9, 84, 21, 14, 1);
    isi_bingkai(14, 10, 82, 19, 1, 1);

    setcolor(15, 4);
    gotoxy(5, 32);
    printf("Tekan >> [ESC] << untuk kembali ke Menu Fitur");

    // Baca data dari data.txt
    int total_data = 0;
    FILE *buka = fopen("data.txt", "r");
    if (buka != NULL)
    {
        while (fscanf(buka, "%[^/]/%[^/]/%[^/]/%[^/]/%[^\n]\n", tamp[total_data][0], tamp[total_data][1], tamp[total_data][2], tamp[total_data][3], tamp[total_data][4]) != EOF)
        {
            total_data++;
            if (total_data >= 100) break;
        }
        fclose(buka);
    }

    if (total_data == 0)
    {
        histori_kosong();
        return;
    }

    int cur = 0;
    while (1)
    {
        setcolor(14, 1);
        gotoxy(30, 11);
        printf("D A T A   P E L A P O R A N   W A J I B   P A J A K");
        setcolor(9, 1);
        gotoxy(17, 12);
        printf("----------------------------------------------------------------------------");

        setcolor(11, 1);
        gotoxy(19, 14);
        printf("Menampilkan Data ke-%d dari total %d data tersimpan                   ", cur + 1, total_data);

        setcolor(14, 1);
        gotoxy(21, 16);
        printf("NAMA WAJIB PAJAK   : ");
        setcolor(15, 1);
        printf("%-46s", tamp[cur][0]);

        setcolor(14, 1);
        gotoxy(21, 18);
        printf("NPWP WAJIB PAJAK   : ");
        setcolor(15, 1);
        printf("%-46s", tamp[cur][1]);

        setcolor(14, 1);
        gotoxy(21, 20);
        printf("KODE WAJIB PAJAK   : ");
        setcolor(15, 1);
        printf("%-46s", tamp[cur][2]);

        setcolor(14, 1);
        gotoxy(21, 22);
        printf("TANGGAL PELAPORAN  : ");
        setcolor(15, 1);
        printf("%-46s", tamp[cur][3]);

        setcolor(14, 1);
        gotoxy(21, 24);
        printf("PAJAK TERUTANG     : ");
        setcolor(10, 1);
        printf("Rp. %-42s", tamp[cur][4]);

        setcolor(9, 1);
        gotoxy(17, 26);
        printf("----------------------------------------------------------------------------");

        setcolor(14, 1);
        gotoxy(20, 27);
        printf("Navigasi: [<-] [->] Geser  |  [T] Lapor SPT Baru  |  [H] Hapus  |  [ESC] Menu");

        int key = getch();
        if (key == 27)
        {
            return;
        }
        else if (key == 't' || key == 'T')
        {
            pajakwp[0] = '\0';
            laporspt();
            return;
        }
        else if (key == 'h' || key == 'H')
        {
            setcolor(12, 1);
            gotoxy(23, 28);
            printf("Yakin hapus data pelaporan ini? (y/t): ");
            char cdel = getch();
            if (cdel == 'y' || cdel == 'Y')
            {
                FILE *frew = fopen("data.txt", "w");
                if (frew != NULL)
                {
                    for (int k = 0; k < total_data; k++)
                    {
                        if (k != cur)
                        {
                            fprintf(frew, "%s/%s/%s/%s/%s\n", tamp[k][0], tamp[k][1], tamp[k][2], tamp[k][3], tamp[k][4]);
                        }
                    }
                    fclose(frew);
                }

                for (int k = cur; k < total_data - 1; k++)
                {
                    for (int c = 0; c < 5; c++)
                    {
                        strcpy(tamp[k][c], tamp[k + 1][c]);
                    }
                }
                total_data--;
                if (cur >= total_data && cur > 0)
                {
                    cur--;
                }
                gotoxy(23, 28);
                printf("Data berhasil dihapus!                       ");
                Sleep(800);
                gotoxy(23, 28);
                printf("                                             ");

                if (total_data == 0)
                {
                    histori_kosong();
                    return;
                }
            }
            else
            {
                setcolor(14, 1);
                gotoxy(23, 28);
                printf("Penghapusan data dibatalkan.                      ");
                Sleep(600);
                gotoxy(23, 28);
                printf("                                                  ");
            }
        }
        else if (key == 0 || key == 224)
        {
            key = getch();
            if (key == 77) // Kanan
            {
                if (cur < total_data - 1) cur++;
                else cur = 0;
            }
            else if (key == 75) // Kiri
            {
                if (cur > 0) cur--;
                else cur = total_data - 1;
            }
        }
    }
}
