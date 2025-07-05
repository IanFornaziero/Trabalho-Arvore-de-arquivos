/*
 *Alunos:
 *      Ian Batista Fornaziero RA: 2677210
 *      Gabriel Augusto Dupim RA: 2651408
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// #include "Funcoes.h"

//esse commit foi com o CLION

int main(void)
{
    char entrada[20];
    char *comando;
    char *argumento;
    int continuar = 1;

    do {

        fgets(entrada, sizeof(entrada), stdin);
        entrada[strcspn(entrada, "\n")] = 0; //apaga o \n

        comando = strtok(entrada, " ");
        argumento = strtok(NULL, "");

        if(strcmp(comando, "cd") == 0)
        { //aqui vai a função cd
            printf("usado comando %s com o diretorio %s\n",comando, argumento);
        }
        else if(strcmp(comando,"search") == 0)
        { //aqui vai a função search
            printf("usado comando search\n");
        }
        else if(strcmp(comando,"rm") == 0)
        { //aqui vai a função rm
            printf("usado comando rm\n");
        }
        else if(strcmp(comando,"list") == 0)
        { //aqui vai a função list
            printf("usado comando list\n");
        }
        else if(strcmp(comando,"mkdir") == 0)
        { //aqui vai a função mkdir
            printf("Usado comando mkdir\n");
        }
        else if(strcmp(comando,"clear") == 0)
        { //não sei se ele quer que a gente faça um clear do zero ou só chame a função clear
            system("cls");
        }
        else if(strcmp(comando,"help") == 0)
        { //aqui vai a função help
            printf("usado comando help\n");
        }
        else if(strcmp(comando, "exit") == 0)
        {
            printf("Usado comando exit\n");
            continuar = 0;
        }
        else
        {
            printf("comando %s nao reconhecido\n", comando);
        }

    } while(continuar);
    return 0;
}
