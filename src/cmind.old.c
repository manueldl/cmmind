#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>

#define randomize ( srand(time(NULL) + getpid()) )
#define random(limit) ( rand() % (limit) )

#define COLORSC 6 /* El tamaño de la gama */
#define SECRETC 4 /* El tamaño del código */
#define MAXTRIES 10 /* El número de intentos */

const char okposition[] = "!"; /* El carácter de posición correcta */
const char okcolor[] = "?"; /* El caracter de color correcto */
const char okwhite[] = " "; /* Espacio de relleno */
const char mark = '|'; /* El cursor */

char colors[] = "abcdef"; /* El código de 'colores' válido */
char tryhist[MAXTRIES][SECRETC + 1]; /* Historial de intentos */
char resulthist[MAXTRIES][SECRETC + 1]; /* Historial de resultados */

char secret[SECRETC + 1]; /* El +1 es para el \0 */
char try[SECRETC + 1];
char result[SECRETC + 1];

unsigned short i, j; /* Contadores */
unsigned short endgame;
unsigned short tries;

char user[100] = ""; /* Se rellenará luego con el nombre del jugador.
¿Es el modo correcto de hacerlo? ¿No sobreescribirá memoria? */


int nanosleep(const struct timespec *req, struct timespec *rem);

void clearscreen (void)
{
    printf( "\033[2J" ); /* Esto no parece que sea portable */
}


void gensecret ( void )
/* Genera el código secreto */
{
    char aux[COLORSC];

    for (i = 0; i < COLORSC; i++)
    {
        j = random(COLORSC);
        aux[i] = colors[j];
    }

    for (i = 0; i < SECRETC; i++)
    {
        secret[i] = aux[i];
    }
}


void gettry(void)
/* Toma y valida el intento del jugador */
{
    if (tries != MAXTRIES)
    {
        do
        {
            printf ("   ---------\n  %s: ", user);
            scanf("%s", tryhist[tries] );
        } while ( strlen(tryhist[tries]) != SECRETC ); /* Comprueba que
        el número de caracteres del intento sea igual que el del
        código secreto antes de seguir */

        strcpy(try, tryhist[tries]); /* Guardamos una copia */
        tries++;
    }
    else
    {
        endgame = 1;
    }
}


void head (int switcher)
/* El encabezado. Sólo muestra el código secreto si se ha acabado el
juego, ya sea por victoria o derrota. Esta condición viene evaluada
desde el exterior en el argumento, que funciona como un interruptor */
{
    char shadow[SECRETC + 1] = "****";

    if ( switcher == 0 ) /* El juego no ha acabado */
    {
        printf("\n  cpu:  %s\n", shadow);
    }
    else
    {
        printf("\n  cpu:  %s\n", secret);
    }

    puts("   ---------");
}


void scoreboard (void)
/* El marcador de resultados */
{
    j = 0;

    for (i = 0; i < MAXTRIES; i++)
    {
        if (j < COLORSC)
        /* Imprime el lado izquiedo con la gama en vertical */
        {
            printf ("%c", colors[j]);
            j++;
        }
        else
        {
            printf(" ");
        }
/*---------------------------------------------------------------------------*/
        if ( i < tries )
        /* Imprime el lado derecho con los resultados */
        {
            printf("  %s %s\n", resulthist[i], tryhist[i]);
        }
        else if ( i == tries )
        {
            printf(" %cxxxx ----\n", mark);
        }
        else
        {
            printf("  xxxx ----\n");
        }
    }
}


void compare ( void )
/* FIXME: Puede que sobrescriba memoria. Probar en otras plataformas. */
/* Devuelve los resultados de la comparación */
{
    int okp = 0; /* Posiciones correctas (número de ?s) */
    int okc = 0; /* Colores correctos (número de !s) */
    int okt = 0; /* Espacios blancos correctos (número de ' 's) */

    char saux[SECRETC + 1]; /* Se hacen unas copias de secret y try */
    char taux[SECRETC + 1]; /* porque sus valores se van a modificar */

    strcpy(saux, secret);
    strcpy(taux, try);

    /* Comprobamos los !s */
    for ( i = 0; i < SECRETC; i++ )
    {
        if ( saux[i] == '-') { continue;}
        if ( taux[i] == saux[i] )
        {
            saux[i] = '-';
            taux[i] = '-';
            okp++;
        }
    }

    /* Comprobamos los ?s */
    for ( i = 0; i < SECRETC; i++ )
    {
        for (j = 0; j < SECRETC; j++)
        {
            if ( taux[i] == '-') { continue;}
            if ( saux[j] == '-') { continue;}
            if ( taux[i] == saux[j] )
            {
                saux[j] = '-';
                taux[i] = '-';
                okc++;
            }
        }
    }

    /* Comprobamos los ' 's */
    okt = SECRETC - okp - okc;

    strcpy(result, ""); /* Vaciamos la cadena */
    for ( i = 0; i < okp; i++ ) strcat(result, okposition);
    for ( i = 0; i < okc; i++ ) strcat(result, okcolor);
    for ( i = 0; i < okt; i++ ) strcat(result, okwhite);

    strcpy(resulthist[tries - 1], result); /* tries - 1 porque ya se
    incrementó en la anterior llamada a gettry() */

    if (okp == SECRETC) /* Se ha ganado el juego */
    {
        clearscreen();
        head(1);
        scoreboard();
        printf("\n\n¡Has ganado!\n");
        endgame = 1;
    }
    else if ( tries == MAXTRIES ) /* Se ha perdido */
    {
        clearscreen();
        head(1);
        scoreboard();
        printf("\n\nLo siento, has perdido.\n");
        endgame = 1;
    }
}


void newgame(void)
/* Contadores a 0 */
{
    gensecret();

    endgame = 0;
    tries = 0;
}


void play (void)
/* La partida entra en un bucle condicionado por endgame */
{
    while (endgame == 0)
    {
        clearscreen();
        head(0);
        scoreboard();
        gettry();
        compare();
    }
}


void ossleep(int sec, int nsec)
/* Función sleep con precisión de milisegundos */
{
    struct timespec delay;
    delay.tv_sec = sec;
    delay.tv_nsec = nsec * 1000000;
    nanosleep(&delay, NULL);
}


void exitgame(void)
/* Se ejecuta cuando el jugador no quiere echar otra partida */
{
    const char banner[] = "\n\nGracias por jugar.\n\n:-)\n";

    for ( i = 0; i < strlen(banner); i++ ) /* Un adornito */
    {
        putchar(banner[i]);
        fflush(stdout);
        ossleep(0, 200);
    }

    ossleep(0, 200 * 3);
}


void replay(void)
/* Interactúa con el jugador para evaluar si se quiere jugar otra vez */
{

    char playmore[3]; /* letra + \n + \0 */
    char s[] = "s";
    char n[] = "n";

    do
    {
        printf("¿Quieres volver a jugar? (s/n): ");

        scanf(" %s", playmore);

        if ( strcmp(playmore, s) == 0 ) { newgame(); play(); }
        else if ( strcmp(playmore, n) == 0 ) { exitgame(); }
        else { printf("\nPor favor, contesta Sí o No.\n"); }

    } while( strcmp(playmore, n) != 0);
}


void getuser (void)
/* Detección del entorno donde se ejecuta el programa */
{
    strcpy( user, getenv("USER") );

    if ( strlen(user) == 0) /* Por si acaso */
    {
        strcpy( user, "Jugador" );
    }
}


int main ( void )
{
    getuser();

    randomize;

    newgame();
    play();
    replay();

    return 0;
}
