#include <stdio.h>
#include <string.h>
#include <conio.h>
#include <windows.h>

void gotoxy(int x, int y);

#define besar inp >= 'A' && inp <= 'Z'
#define kecil inp >= 'a' && inp <= 'z'
#define angka inp >= '0' && inp <= '9'

int cek = 0;
////////////////////////////////////////////////////////////// USERNAME /////////////////////////////////////////////////////////////////////////
char *vusername(char *tamp)
{
    while (cek == 0)
    {
        char inp;
        int idx = 0;
        while (1)
        {
            inp = getch();
            if (idx < 20 && (kecil || besar || angka || inp == '_'))
            {
                printf("%c", inp);
                tamp[idx++] = inp;
            }
            else if (inp == 8 && idx > 0)
            {
                printf("\b \b");
                tamp[--idx] = '\0';
            }
            else if (inp == 13)
            {
                tamp[idx] = '\0';
                break;
            }
            else if (inp == 27)
            {
                tamp[0] = 27;
                tamp[1] = '\0';
                return tamp;
            }
            else
            {
                printf("\a");
            }
        }
        return tamp;
    }
    return tamp;
}

//////////////////////////////////////////////////////////////// PASSWORD ///////////////////////////////////////////////////////////////////////
char *vpass(char *pass)
{
    char inp;
    int idx = 0, x = 0, y = 0;
    while ((inp = getch()))
    {
        if (idx <= 10 && inp >= '0' && inp <= '9')
        {
            x++;
            printf("*");
            pass[idx++] = inp;
        }
        else if (idx <= 10 && inp >= 'a' && inp <= 'z')
        {
            y++;
            printf("*");
            pass[idx++] = inp;
        }
        else if (inp == 8 && idx > 0 && (pass[idx - 1] >= '0' && pass[idx - 1] <= '9'))
        {
            x--;
            printf("\b \b");
            pass[--idx] = '\0';
        }
        else if (inp == 8 && idx > 0 && (pass[idx - 1] >= 'a' && pass[idx - 1] <= 'z'))
        {
            y--;
            printf("\b \b");
            pass[--idx] = '\0';
        }
        else if (inp == 13 && idx >= 8 && x >= 4 && y >= 4)
        {
            printf("\n");
            pass[idx] = '\0';
            break;
        }
        else if (inp == 27)
        {
            pass[0] = 27;
            pass[1] = '\0';
            return pass;
        }
        else
        {
            printf("\a");
        }
    }
    return pass;
}
///////////////////////////////////////////////////////////////// NAMA //////////////////////////////////////////////////////////////////////////
char *vnama(char *tam)
{
    char inp;
    char kumpulan[20] = {};
    char idx = 0;

    while ((inp = getch()) != 13 || idx <= 5)
    {
        if (idx <= 20 && (((idx == 0 || kumpulan[idx - 1] == ' ') && (inp >= 'A' && inp <= 'Z')) ||
                          (idx > 0 && (inp >= 'a' && inp <= 'z') && (inp != kumpulan[idx - 1] || inp != kumpulan[idx - 2]) && kumpulan[idx - 1] != ' ') || idx > 2 && inp == 32 && kumpulan[idx - 1] != 32))
        {
            printf("%c", inp);
            kumpulan[idx] = inp;
            idx++;
        }
        else if (inp == 8 && idx != 0)
        {
            printf("\b \b");
            kumpulan[idx - 1] = '\0';
            idx--;
        }
        else if (inp == 27)
        {
            tam[0] = 27;
            tam[1] = '\0';
            return tam;
        }
        else
        {
            printf("\a");
        }
    }
    strcpy(tam, kumpulan);
    return tam;
}


char *vnamaupdate(char *tam)
{
    char inp;
    char kumpulan[20] = {};
    char idx = 0;

    while ((inp = getch()) != 13 || idx <= 5)
    {
        if (idx <= 20 && (((idx == 0 || kumpulan[idx - 1] == ' ') && (inp >= 'A' && inp <= 'Z')) ||
                          (idx > 0 && (inp >= 'a' && inp <= 'z') && (inp != kumpulan[idx - 1] || inp != kumpulan[idx - 2]) && kumpulan[idx - 1] != ' ') || idx > 2 && inp == 32 && kumpulan[idx - 1] != 32))
        {
            printf("%c", inp);
            kumpulan[idx] = inp;
            idx++;
        }
        else if (inp == 8 && idx != 0)
        {
            printf("\b \b");
            kumpulan[idx - 1] = '\0';
            idx--;
        }
        else if (inp == 27)
        {
            tam[0] = 27;
            tam[1] = '\0';
            return tam;
        }
        else
        {
            printf("\a");
        }
    }
    strcpy(tam, kumpulan);
    return tam;
}

///////////////////////////////////////////////////////////////// NO HP /////////////////////////////////////////////////////////////////////////
char *vnohp(char tamp[100])
{
    char inp;
    int idx = 0;

    while (1)
    {
        inp = getch();
        if (idx == 0 && inp == '0' || tamp[0] == '0' && idx == 1 && inp == '8' || idx > 1 && idx < 12 && angka && inp != ' ')
        {
            printf("%c", inp);
            tamp[idx++] = inp;
        }
        else if (inp == 8 && idx > 0)
        {
            printf("\b \b");
            tamp[--idx] = '\0';
        }
        else if (inp == '\r' && idx >= 11)
        {
            tamp[idx] = '\0';
            idx = 0;
            break;
        }
        else if (inp == 27)
        {
            tamp[0] = 27;
            tamp[1] = '\0';
            return tamp;
        }
    }
    return tamp;
}

//////////////////////////////////////////////////////////////// VALID GAJI /////////////////////////////////////////////////////////////////////
char *valid_gaji(int x, int y, int batas)
{
    static char money[20];

    int a = 0;
    money[a] = getch();

    while (money[a] != 13 || a == 0)
    {

        if (money[a] == 8)
        {
            a--;
            if (a < 0)
            {
                a = 0;
            }
            gotoxy(x + a, y);
            printf(" ");
        }
        else
        {
            if (money[a] >= '0' && money[a] <= '9')
            {
                if (money[0] == '0')
                {
                    gotoxy(x, y);
                    printf("Inputan Invalid ");
                    Sleep(500);
                    gotoxy(x, y);
                    printf("                ");
                }
                else
                {
                    gotoxy(x + a, y);
                    printf("%c", money[a]);
                    a++;
                }
            }
            else
            {
                if (money[a] == '\0')
                {
                }
                else
                {
                    a = 0;
                    gotoxy(x, y);
                    printf("Inputan Invalid ");
                    Sleep(500);
                    gotoxy(x, y);
                    printf("                ");
                }
            }
        }

        if (a >= batas + 1)
        {
            a = 0;
            gotoxy(x, y);
            printf("                 ");
            gotoxy(x, y);
            printf("Maksimal 15 digit");
            Sleep(500);
            gotoxy(x, y);
            printf("                 ");
        }

        gotoxy(x + a, y);
        money[a] = getch();

        if (money[a] == 15 && a < 7) // ENTER DAN
        {
            gotoxy(x, y);
            printf("Minimal 7 digit (1 juta)");
            Sleep(500);
            gotoxy(x, y);
            printf("                        ");
            Sleep(500);
            a = 0;
            money[a] = '\0';
        }

        if (money[a] != '\0')
        {
            gotoxy(x + a, y);
            printf("%c", money[a]);
        }

        if (money[a] == 27)
        {
            money[0] = 27;
            money[1] = '\0';
            return money;
        }
    }

    money[a] = '\0';
    return money;
}

//////////////////////////////////////////////////////////////// VALID LEMBUR ///////////////////////////////////////////////////////////////////
char *valid_lembur(int x, int y, int batas)
{
    static char lembur[20];
    int a = 0;
    gotoxy(x, y);

    while (1)
    {
        char ch = getch();
        if (ch >= '0' && ch <= '9')
        {
            if (a < batas)
            {
                if (a == 1 && lembur[0] == '0')
                {
                    gotoxy(x, y);
                    printf(" ");
                    a = 0;
                }
                lembur[a] = ch;
                gotoxy(x + a, y);
                printf("%c", ch);
                a++;
            }
        }
        else if (ch == 8)
        {
            if (a > 0)
            {
                a--;
                lembur[a] = '\0';
                gotoxy(x + a, y);
                printf(" ");
                gotoxy(x + a, y);
            }
        }
        else if (ch == 13)
        {
            if (a == 0)
            {
                lembur[0] = '0';
                lembur[1] = '\0';
                gotoxy(x, y);
                printf("0");
                break;
            }
            lembur[a] = '\0';
            break;
        }
        else if (ch == 27)
        {
            lembur[0] = 27;
            lembur[1] = '\0';
            return lembur;
        }
        else
        {
            printf("\a");
        }
    }
    return lembur;
}

///////////////////////////////////////////////////////////////// VALIDASI IURAN ////////////////////////////////////////////////////////////////
char *valid_iuran(int x, int y, int batas)
{
    static char iuran[20];

    int a = 0;

    gotoxy(x, y);
    iuran[a] = getch();

    while (iuran[a] != 13 || a == 0) // enter
    {

        if (iuran[a] == 8) // backspace
        {
            a--;
            if (a < 0)
            {
                a = 0;
            }
            gotoxy(x + a, y);
            printf(" ");
        }
        else
        {
            if (iuran[a] >= '0' && iuran[a] <= '9')
            {
                if (iuran[0] == '0')
                {
                    gotoxy(x, y);
                    printf(" Inputan Invalid ");
                    Sleep(500);
                    gotoxy(x, y);
                    printf("                 ");
                }
                else
                {
                    gotoxy(x + a, y);
                    printf("%c", iuran[a]);
                    a++;
                }
            }
            else
            {
                if (iuran[a] == '\0')
                {
                }
                else
                {
                    gotoxy(x, y);
                    printf(" Inputan Invalid ");
                    Sleep(500);
                    gotoxy(x, y);
                    printf("                 ");
                }
            }
        }

        gotoxy(x + a, y);
        iuran[a] = getch();

        if (iuran[a] == 13 && a < 5)
        {
            gotoxy(x, y);
            printf(" Minimal 5 digit (10 ribu)");
            Sleep(500);
            gotoxy(x, y);
            printf("                            ");
            Sleep(500);
            a = 0;
            iuran[a] = '\0';
        }

        if (iuran[a] == 27)
        {
            iuran[0] = 27;
            iuran[1] = '\0';
            return iuran;
        }

        if (iuran[a] != '\0')
        {
            gotoxy(x + a, y);
            printf("%c", iuran[a]);
        }
    }

    iuran[a] = '\0';
    return iuran;
}
////////////////////////////////////////////////////////////// NAMA WAJIB PAJAK ////////////////////////////////////////////////////////////////
char *val_nwp(char *nwp)
{
    char inp;
    char kumpulan[20] = {};
    char idx = 0;

    while ((inp = getch()) != 13 || idx <= 5)
    {
        if (idx <= 20 && (((idx == 0 || kumpulan[idx - 1] == ' ') && (inp >= 'A' && inp <= 'Z')) ||
                          (idx > 0 && (inp >= 'a' && inp <= 'z') && (inp != kumpulan[idx - 1] || inp != kumpulan[idx - 2]) && kumpulan[idx - 1] != ' ') || idx > 2 && inp == 32 && kumpulan[idx - 1] != 32))
        {
            printf("%c", inp);
            kumpulan[idx] = inp;
            idx++;
        }
        else if (inp == 8 && idx != 0)
        {
            printf("\b \b");
            kumpulan[idx - 1] = '\0';
            idx--;
        }
        else if (inp == 27)
        {
            nwp[0] = 27;
            nwp[1] = '\0';
            return nwp;
        }
        else
        {
            printf("\a");
        }
    }
    strcpy(nwp, kumpulan);
    return nwp;
}

/////////////////////////////////////////////////////////////////////// NPWP ///////////////////////////////////////////////////////////////////
char *valnpwp(int x, int y, int batas, char npwp[15])
{
    int a = 0;

    gotoxy(x, y);
    npwp[a] = getch();

    while (npwp[a] != 13 || a == 0)
    {

        if (npwp[a] == 8)
        {
            a--;
            if (a < 0)
            {
                a = 0;
            }
            gotoxy(x + a, y);
            printf(" ");
        }
        else
        {
            if (npwp[a] >= '0' && npwp[a] <= '9')
            {
                if (npwp[0] == '0' || (npwp[a] == npwp[a - 1] && npwp[a] == npwp[a - 2] && npwp[a] == npwp[a - 3] && npwp[a - 1] == npwp[a - 2]) && npwp[a - 1] == npwp[a - 3] && npwp[a - 2] == npwp[a - 3])
                {
                    gotoxy(x + a, y);
                    printf("Invalid ");
                    Sleep(500);
                    gotoxy(x + a, y);
                    printf("                ");
                }
                else
                {
                    gotoxy(x + a, y);
                    printf("%c", npwp[a]);
                    a++;
                }
            }
            else
            {
                if (npwp[a] == '\0')
                {
                }
                else
                {
                    gotoxy(x + a, y);
                    printf("Invalid ");
                    Sleep(500);
                    gotoxy(x + a, y);
                    printf("                ");
                }
            }
        }

        if (a > batas)
        {
            a = 0;
            gotoxy(x, y);
            printf("                                ");
            gotoxy(x, y);
            printf("JUMLAH NPWP 15 DIGIT");
            Sleep(500);
            gotoxy(x, y);
            printf("                                ");
        }

        gotoxy(x + a, y);
        npwp[a] = getch();

        if (npwp[a] == 13 && a < 15) // ENTER DAN
        {
            gotoxy(x, y);
            printf("JUMLAH NPWP 15 DIGIT");
            Sleep(500);
            gotoxy(x, y);
            printf("                          ");
            Sleep(500);
            a = 0;
            npwp[a] = '\0';
        }
        else if (npwp[a] == 27)
        {
            npwp[0] = 27;
            npwp[1] = '\0';
            return npwp;
        }

        if (npwp[a] != '\0')
        {
            gotoxy(x + a, y);
            printf("%c", npwp[a]);
        }
    }

    npwp[a] = '\0';
    return npwp;
}

///////////////////////////////////////////////////////////// PAJAK TERUTANG ////////////////////////////////////////////////////////////////////
char *valpajakterutang(int x, int y, int batas, char pajakterutang[20])
{
    int a = 0;

    gotoxy(x, y);
    pajakterutang[a] = getch();

    while (pajakterutang[a] != 13 || a == 0)
    {

        if (pajakterutang[a] == 8)
        {
            a--;
            if (a < 0)
            {
                a = 0;
            }
            gotoxy(x + a, y);
            printf(" ");
        }
        else
        {
            if (pajakterutang[a] >= '0' && pajakterutang[a] <= '9')
            {
                if (pajakterutang[0] == '0')
                {
                    gotoxy(x + a, y);
                    printf("     Invalid     ");
                    Sleep(500);
                    gotoxy(x + a, y);
                    printf("                ");
                }
                else
                {
                    gotoxy(x + a, y);
                    printf("%c", pajakterutang[a]);
                    a++;
                }
            }
            else
            {
                if (pajakterutang[a] == '\0')
                {
                }
                else
                {
                    a = 0;
                    gotoxy(x + a, y);
                    printf("    Invalid     ");
                    Sleep(500);
                    gotoxy(x + a, y);
                    printf("                ");
                }
            }
        }

        if (a > batas)
        {
            a = 0;
            gotoxy(x, y);
            printf("                              ");
            gotoxy(x, y);
            printf("Maksimal 13 digit");
            Sleep(500);
            gotoxy(x, y);
            printf("                              ");
        }

        gotoxy(x + a, y);
        pajakterutang[a] = getch();

        if (pajakterutang[a] == 13 && a < 4) // ENTER DAN
        {
            gotoxy(x, y);
            printf("Minimal 4 digit");
            Sleep(500);
            gotoxy(x, y);
            printf("                          ");
            Sleep(500);
            a = 0;
            pajakterutang[a] = '\0';
        }

        if (pajakterutang[a] == 27)
        {
            pajakterutang[0] = 27;
            pajakterutang[1] = '\0';
            return pajakterutang;
        }

        if (pajakterutang[a] != '\0')
        {
            gotoxy(x + a, y);
            printf("%c", pajakterutang[a]);
        }
    }

    pajakterutang[a] = '\0';
    return pajakterutang;
}

int valpilih_profil()
{
    char input;
    char tamp;
    int index = 0;
    while ((input = getch()) != 13 || index < 1)
    {
        if (index < 1 && (input >= '1' && input <= '4'))
        {
            printf("%c", input);
            tamp = input;
            index++;
        }
        else if (input == 8 && index != 0)
        {
            printf("\b \b");
            index--;
            tamp = 0;
        }
        else if (input == 27)
        {
            return -1;
        }
    }
    return tamp - '0';
}