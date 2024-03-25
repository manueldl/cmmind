#include <stdio.h>
#include <string.h>  /* strcmp() and others */
#include <stdlib.h>
#include <time.h>    /* rand(), sleep() */

#define PROGRAM     "CMind"
#define EXECUTABLE  "cmind"
#define DESCRIPTION "Mastermind®-like console game."
#define VERSION     "0.20"
#define URL         "https://github.com/mdomlop/cmind"
#define LICENSE     "GPLv3+"
#define AUTHOR      "Manuel Domínguez López"
#define NICK        "mdomlop"
#define MAIL        "zqbzybc@tznvy.pbz"


#ifdef _WIN32
#define RED     ""
#define GREEN   ""
#define YELLOW  ""
#define BLUE    ""
#define MAGENTA ""
#define CYAN    ""
#define RESET   ""
#else
/* ANSI COLORS: */
#define RED     "\x1b[31m"
#define GREEN   "\x1b[32m"
#define YELLOW  "\x1b[33m"
#define BLUE    "\x1b[34m"
#define MAGENTA "\x1b[35m"
#define CYAN    "\x1b[36m"
#define RESET   "\x1b[0m"
#endif



int ngames    = 0;  /* Number of completed games */
int victories = 0;  /* Number of winned games */
int defeats   = 0;  /* Number of losed games */

int maxtries = 10;   /* Number of attempts */
int secretc  = 4;   /* Secret code size */
int colorsc  = 6;   /* Number of colors */

char input[secretc + 1];

int wingame;        /* Boolean, if win the game */
long tries;         /* Number of remaining attempts */

char secret[secretc + 1]; /* The +1 is for '\0' */
char colors[colorsc + 1]; /* Valid 'colors' */


unsigned int sleep(unsigned int seconds);

void replay(int fail);
void stat(void);
void finish(void);
void exitgame(void);


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


void start_banner(void)
{
    printf( "\n"
            RESET "Valid characteres are a, b, c, d, e, f\n"
            "--------------------------------------\n\n"
            "                                      \n"

            RESET "%ld) " YELLOW "CPU: " RESET "****\n", tries);
}


void gensecret (void)
/* Generate the secret code */
{
	int i;
    char aux[colorsc];

    /* Generate the colors string */
    for (i = 0; i < colorsc; i++)
        colors[i] = i + 97; /* Start with ASCII 'a' */
    /*colors[i] = '\0';  *//* Close the string */

    /* Generate the aux array */
    for (i = 0; i < colorsc; i++)
    {
		int j;
        j = rand() % (colorsc);  /* random pick from colors */
        aux[i] = colors[j];
    }

    /* Generate the secret array */
    for (i = 0; i < secretc; i++)
    {
        secret[i] = aux[i];
    }
}


void getinput(void)
{
    printf(BLUE "You: " RESET);
    if (scanf("%5s", input) != 1) exit(0);  /* Exit game on Ctrl+D */
	while (getchar() != '\n');  /* clean buffer*/

    while (strlen(input) != secretc)
    {
        printf("%ld) " RED "CPU: "
                RESET "Sorry. I want only 4 letters.\n"
				" Please, try again.\n", tries);

        printf(MAGENTA "You: " RESET);
		if (scanf("%5s", input) != 1) exit(0);  /* Exit game on Ctrl+D */
		while (getchar() != '\n');  /* clean */
    };
}


void help(void)
{
    puts(MAGENTA "CPU: " GREEN "Guess the secret code!!!" RESET);
}


void info(void)
{
	printf(MAGENTA "CPU: " RESET "This is %s® version %s by %s.\n\n\n",
		   PROGRAM, VERSION, NICK);

	printf("Copyright: 2020 %s <%s@gmail.com>\n\n"
	"License: GPL-3+\n\n", AUTHOR, NICK);

	puts(" This program is free software: you can"
	" redistribute it and/or modify\n"
	" it under the terms of the GNU General Public License as published by\n"
	" the Free Software Foundation, either version 3 of the License, or\n"
	" (at your option) any later version.\n");

	puts(" This package is distributed in the hope that it will be useful,\n"
	" but WITHOUT ANY WARRANTY; without even the implied warranty of\n"
	" MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the\n"
	" GNU General Public License for more details.\n");

	puts(" You should have received a copy of the"
	" GNU General Public License\n"
	" along with this program. If not, see"
	" <https://www.gnu.org/licenses/>.\n");

	puts(" On Debian systems, the complete text of the GNU General\n"
	" Public License version 3 can be found in"
	" \"/usr/share/common-licenses/GPL-3\".\n\n"
	" On Arch systems, the complete text of the GNU General\n"
	" Public License version 3 can be found in"
	" \"/usr/share/licenses/common/GPL3/license.txt\".\n\n");

	printf(" You can find the latest version of the source code at:\n"
	" <%s>.\n" RESET, URL);
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
    else if(!strcmp(hack, "stat"))
    {
        tries++;
        stat();
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
        printf(MAGENTA "CPU: " RESET "Now you have %ld tries.\n", tries -1);
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
        puts(MAGENTA "CPU: " RESET "It's soon."
				" You can still finish this game.\n");
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
    else if(!strcmp(hack, "unix"))
    {
        tries++;
        puts(MAGENTA "CPU: " RESET "It's a UNIX system! I know this!\n");
        return 1;
    }
    else if(!strcmp(hack, "exit"))
    {
        puts(MAGENTA "CPU: " RESET "Quitting.\n");
        exitgame();
    }
    else if(!strcmp(hack, "quit"))
    {
        puts(MAGENTA "CPU: " RESET "Exiting.\n");
        exitgame();
    }
    return 0;
}


int validate(const char * s)
{
    int i;
    char v[] = "abcdef";
    int vsz = sizeof(v) - 1;

    for (i=0; i<secretc; i++)
    {
		int j, ret = 0;
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

    for (i=0; i < secretc; i++)  /* Find ! */
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

    for (i=0; i < secretc; i++)  /* Find ? */
    {
		if (input_aux[i] == '-') continue;
        for (j=0; j < secretc; j++)
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

    if(okpos == secretc) wingame = 1;

    return 0;
}


void stat(void)
{
    printf(RESET "\nGames: %d\nVictories: %d\nDefeats: %d\nAverage %d %%\n",
            ngames - 1, victories, defeats,
            ngames > 1 ? victories * 100 / (ngames - 1) : 0); /* No x/0 */
}


void play(void)
{
    tries = maxtries;
    wingame = 0;

    banner();
    start_banner();
    gensecret();
    ngames++;  /* For stat function */

    while(tries)
    {
        getinput();
        compare();
        if (wingame) break;
        tries--;
    }

	finish();
	stat();
	replay(0);
}


void exitgame(void)
{
    puts(RESET " Thank you for playing cmind.");
    printf(RESET);  /* Normal colors for terminal on exit. */
	/*free(input); */
	exit(0);
}


void finish(void)
{
    if (wingame)
    {
        victories++;
        printf(YELLOW "\nCPU: "
                RESET "You WIN! The secret was "
                CYAN "%s\n", secret);
    }
    else
    {
        defeats++;
        printf(YELLOW "\nCPU: "
                RESET "You LOSE! The secret was "
                CYAN "%s\n", secret);
    }
}


void replay(int fail)
{
	if (!fail)
		puts(YELLOW "\nCPU: " RESET "Do you want to play again? (Y/N).");

    printf(BLUE "You: " RESET);


    if (scanf("%2s", input) != 1) exit(1);
	while (getchar() != '\n');  /* clean */

    while ( (strlen(input) != 1) )
	{
        printf(RED "CPU: " RESET "Please. Answer 'y' or 'n'.\n");
		puts(RESET " Do you want to play again? (Y/N).");

        printf(MAGENTA "You: " RESET);
		if (scanf("%2s", input) != 1) exit(1);
		while (getchar() != '\n');  /* clean */
    };


	switch (input[0])
	{
		case 'y':
		case 'Y':
			printf(GREEN "CPU: " RESET "OK. Let's play again!\n");
			play();
			break;
		case 'n':
		case 'N':
			printf(YELLOW "CPU: " RESET "Bye.\n");
			exitgame();
			break;
		default:
			printf(RED "CPU: " RESET "Please. Answer 'y' or 'n'.\n");
			puts(RESET " Do you want to play again? (Y/N).");
			replay(1);
			puts(RED "Aquí hay un error" RESET);
			abort();
	}
}


int main(void)
{
    /*input = (char*)malloc(secretc + 2);*/

    srand(time(NULL));

    play();

    printf(RESET);  /* Normal colors for terminal on exit. */
	/*free(input);*/

    return 0;
}
