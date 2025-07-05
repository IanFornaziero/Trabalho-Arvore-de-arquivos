/*
 *Alunos:
 *      Ian Batista Fornaziero RA: 2677210
 *      Gabriel Augusto Dupim RA: 2651408
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Funcoes.h"

int main(void)
{
    char entrada[256];
    char *comando;
    char *argumento;
    int continuar = 1;

    SistemaArquivos *sistema = inicializarSistema();

    // carrega o arquivo de entrada
    printf("\tSimulador de Diretorio em Linha de Comando\n");
    printf("Carregando estrutura de diretorios...\n");
    carregarArquivo(sistema, "data/in.txt");

    printf("\nDigite 'help' para ver os comandos disponiveis.\n");

    do
    {
        // mostra o prompt com o diretorio atual
        printf("\n%s > ", obterCaminhoAtual(sistema->diretorioAtual));

        fgets(entrada, sizeof(entrada), stdin);
        entrada[strcspn(entrada, "\n")] = 0; // apaga o \n

        if (strlen(entrada) == 0)
            continue; // ignora entrada vazia

        comando = strtok(entrada, " ");
        argumento = strtok(NULL, "");

        if (strcmp(comando, "cd") == 0)
        {
            navegarCd(sistema, argumento);
        }
        else if (strcmp(comando, "search") == 0)
        {
            if (argumento)
            {
                printf("Buscando por '%s'...\n", argumento);
                buscarArquivo(sistema->raiz, argumento, "");
            }
            else
            {
                printf("Uso: search <nome_do_arquivo>\n");
            }
        }
        else if (strcmp(comando, "rm") == 0)
        {
            if (argumento)
            {
                removerArquivo(sistema, argumento);
            }
            else
            {
                printf("Uso: rm <nome_do_arquivo>\n");
            }
        }
        else if (strcmp(comando, "list") == 0)
        {
            listarDiretorio(sistema->diretorioAtual);
        }
        else if (strcmp(comando, "mkdir") == 0)
        {
            if (argumento)
            {
                criarDiretorio(sistema, argumento);
            }
            else
            {
                printf("Uso: mkdir <nome_do_diretorio>\n");
            }
        }
        else if (strcmp(comando, "clear") == 0)
        {
            system("cls");
        }
        else if (strcmp(comando, "help") == 0)
        {
            mostrarAjuda();
        }
        else if (strcmp(comando, "exit") == 0)
        {
            printf("Saindo do programa...\n");
            continuar = 0;
        }
        else
        {
            printf("Comando '%s' nao reconhecido. Digite 'help' para ver os comandos disponiveis.\n", comando);
        }

    } while (continuar);

    // libera a memoria
    liberarSistema(sistema);
    return 0;
}
