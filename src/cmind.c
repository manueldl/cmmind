#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define VERSION "0.10b"

/* ANSI COLORS: */

#define RED     "\x1b[31m"
#define GREEN   "\x1b[32m"
#define YELLOW  "\x1b[33m"
#define BLUE    "\x1b[34m"
#define MAGENTA "\x1b[35m"
#define CYAN    "\x1b[36m"
#define RESET   "\x1b[0m"

#define COLORSC 6 /* Number of colors */
#define SECRETC 4 /* Secret code size */
#define MAXTRIES 10 /* Number of attempts */

int wingame;
size_t tries;
int ngames = 0;
int endgame = 1;

char *input;
char *playagain;

char secret[SECRETC + 1]; /* The +1 is for \0 */
char colors[] = "abcdef"; /* Valid 'colors' */
unsigned short i, j; /* Counters */

unsigned int sleep(unsigned int seconds);
void replay(void);


void banner(void)
{
    puts( GREEN"\n\n"
            " ********************************* \n"
            "*" RESET "                 _           _   " GREEN "*\n"
            "*" RESET "                (_)         | |  " GREEN "*\n"
            "*" RESET "   ___ _ __ ___  _ _ __   __| |  " GREEN "*\n"
            "*" RESET "  / __| '_ ` _ \\| | '_ \\ / _` |" GREEN "  *\n"
            "*" RESET " | (__| | | | | | | | | | (_| |  " GREEN "*\n"
            "*" RESET "  \\___|_| |_| |_|_|_| |_|\\__,_|" GREEN "  *\n"
            "*" RESET "                                 " GREEN "*\n"
            " ********************************* \n");
}


void start(void)
{
    printf(
            "\n"
            RESET "Valid characteres are a, b, c, d, e, f\n"
            "--------------------------------------\n\n"
            "                                      \n"

            RESET "%ld) " YELLOW "CPU: " RESET "****\n", tries);
}


void gensecret (void)
/* Generate the secret code */
{
    char aux[COLORSC];

    for (i = 0; i < COLORSC; i++)
    {
        j = rand() % (COLORSC);
        aux[i] = colors[j];
    }

    for (i = 0; i < SECRETC; i++)
    {
        secret[i] = aux[i];
    }
    /* strcpy(secret, "cebb"); */
}


int getinput(void)
{
        int retval;
        printf(BLUE "You: " RESET);
        retval = scanf("%s", input);

        if (retval == -1) return 0;

        while (
                (strlen(input) < SECRETC) ||
                (strlen(input) > SECRETC)
                )
        {
            printf("%ld) " RED "CPU: "
                    RESET "Sorry. I want only 4 letters.\n"
                    "Try again:\n", tries);

            free(input);
            input = (char*)malloc(SECRETC + 1);

            printf(MAGENTA "You: " RESET);
            retval = scanf("%s", input);
            if (retval == -1) return 0;
        };


        return 1;
}


void help(void)
{
    puts(MAGENTA "CPU: " GREEN "Guess the secret code!!!" RESET);
}


void info(void)
{
	printf(MAGENTA "CPU: " RESET "This is CMind® version %s by mdomlop.\n\n\n",
		   VERSION);

	puts("Copyright: 2020 Manuel Domínguez López <mdomlop@gmail.com>\n\n"
	"License: GPL-3+\n\n"
	" This program is free software: you can redistribute it and/or modify\n"
	" it under the terms of the GNU General Public License as published by\n"
	" the Free Software Foundation, either version 3 of the License, or\n"
	" (at your option) any later version.\n");

	puts(" This package is distributed in the hope that it will be useful,\n"
	" but WITHOUT ANY WARRANTY; without even the implied warranty of\n"
	" MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the\n"
	" GNU General Public License for more details.\n");

	puts(" You should have received a copy of the GNU General Public License\n"
	" along with this program. If not, see"
	" <https://www.gnu.org/licenses/>.\n");

	puts(" On Debian systems, the complete text of the GNU General\n"
	" Public License version 3 can be found in"
	" \"/usr/share/common-licenses/GPL-3\".\n\n"
	" On Arch systems, the complete text of the GNU General\n"
	" Public License version 3 can be found in"
	" \"/usr/share/licenses/common/GPL3/license.txt\".\n\n"
	" You can find the latest version of the source code at:\n"
	" <https://github.com/mdomlop/cmind>.\n"
	RESET);
}


int hacks(const char * hack)
{
    if(!strcmp(hack, "show"))
    {
        printf( MAGENTA "CPU: "
                RESET "%s\n", secret);
        tries++;
        return 1;
    }
    else if(!strcmp(hack, "iwin"))
    {
        wingame = 1;
        return 1;
    }
    else if(!strcmp(hack, "lose"))
    {
        tries = 1;
        return 1;
    }
    else if(!strcmp(hack, "logo"))
    {
        tries++;
        banner();
        return 1;
    }
    else if(!strcmp(hack, "help"))
    {
        tries++;
        help();
        return 1;
    }
    else if(!strcmp(hack, "info"))
    {
        tries++;
        banner();
        sleep(1);
        info();
        return 1;
    }
    else if(!strcmp(hack, "more"))
    {
        tries += 2;
        printf(MAGENTA "CPU: " RESET "You have now %ld tries.\n ", tries -1);
        return 1;
    }
    else if(!strcmp(hack, "less"))
    {
        tries++ ;
        puts(MAGENTA "CPU: " RESET "Dumb, play the fucking game.\n");
        return 1;
    }
    else if(!strcmp(hack, "time"))
    {
        tries++ ;
        puts(MAGENTA "CPU: " RESET "It's soon. You can finish this game.\n");
        return 1;
    }
    else if(!strcmp(hack, "wool"))
    {
        tries++ ;
        puts(MAGENTA "CPU: " RESET "WoOLolOo.\n");
        return 1;
    }
    else if(!strcmp(hack, "hool"))
    {
        tries++ ;
        puts(MAGENTA "CPU: " RESET "HoOLolOo.\n");
        return 1;
    }
    else if(!strcmp(hack, "exit"))
    {
        puts(MAGENTA "CPU: " RESET "Quitting.\n");
        exit(0);
    }
    else if(!strcmp(hack, "quit"))
    {
        puts(MAGENTA "CPU: " RESET "Exiting.\n");
        exit(0);
    }
    return 0;
}


int validate(const char * s)
{
    int i, j;
    char v[] = "abcdef";
    int vsz = sizeof(v) - 1;
    int ret = 0;

    for (i=0; i<SECRETC; i++)
    {
        ret = 0;
        for (j=0; j < vsz ; j++)
        {
            if(s[i] == v[j]) break;
            else ret++;
        }

        if(ret == vsz )
        {
            tries++ ;
            printf(RED "CPU: " RESET "Invalid character: " RED "%c"
			RESET ".\n", s[i]);
            return 1;
        }
    }


    return 0;
}


int compare(void)
{
    int i, j;
    int okpos = 0;
    int okcol = 0;
    char code[] = "    ";
    char input_aux[] =  "xxxx";  /* marks */
    char secret_aux[] =  "xxxx";  /* marks */

    if(hacks(input) == 1)
        return 0;

    if(validate(input) == 1) return 0;

    for (i=0; i < SECRETC; i++)  /* Find ! */
    {
        if(input[i] == secret[i])
        {
		/* printf("Found POS: %d %d\n", i, okpos); */
            okpos++;
            input_aux[i] = '-';
            secret_aux[i] = '-';
            continue;
        }
    }

    for (i=0; i < SECRETC; i++)  /* Find ? */
    {
		if (input_aux[i] == '-') continue;
        for (j=0; j < SECRETC; j++)
        {
			if (secret_aux[j] == '-') continue;
            if(input[i] == secret[j])
            {
                okcol++;
                input_aux[i] = '-';
                secret_aux[j] = '-';
                break;
            }
        }
    }


    for (i=0; i < okpos; i++)
	{
		/* printf("POS: %d ! %d\n", i, okpos); */
		code[i] = '!';
	}
    for (i=okpos; i < okcol + okpos; i++)
	{
		/* printf("COL: %d ? %d\n", i, okcol); */
		code[i] = '?';
	}
    printf("%ld) " YELLOW "CPU: "
            RESET "%s\n", tries - 1, code);

    if(okpos == SECRETC) wingame = 1;

    return 0;
}


void play(void)
{

    tries = MAXTRIES;
    banner();
    start();
    gensecret();
    wingame = 0;
    ngames++;

    while(tries)
    {

        if (!getinput()) exit(0);  /* Exit on Ctrl+D */
        compare();
        if (wingame) break;
        tries--;
    }
    replay();
}


void exitgame(void)
{
    puts(RESET "Thank you for playing cmind.");
}


void replay(void)
{
    char answer[100];

    if (wingame)
    printf(YELLOW "\nCPU: "
            RESET "You win! The secret was "
            CYAN "%s\n", secret);
    else
    printf(YELLOW "\nCPU: "
            RESET "You lose! The secret was "
            CYAN "%s\n", secret);


    puts(RESET "\nDo you want to play again? (y/N).");

    do
    {
    scanf("%s", answer);
    } while(
            ((answer[0] != 'y') && (answer[1] != '\0')) ||
            ((answer[0] != 'n') && (answer[1] != '\0'))
            );

    if(answer[0] == 'y')
    play();
    else if(answer[0] == 'n')
    exitgame();
    else puts("NOOOooo");
}


int main(void)
{
    input = (char*)malloc(SECRETC + 1);

    srand(time(NULL));
    ngames++;

    play();

    printf(RESET);
    getchar();
    return 0;
}
