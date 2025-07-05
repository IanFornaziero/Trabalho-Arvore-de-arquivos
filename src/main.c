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

    SistemaArquivos *sistema = inicializar_sistema();

    // carrega o arquivo de entrada
    printf("=== SIMULADOR DE LINHA DE COMANDO ===\n");
    printf("Carregando estrutura de diretórios...\n");
    carregar_arquivo(sistema, "data/in.txt");

    printf("\nDigite 'help' para ver os comandos disponíveis.\n");

    do
    {
        // mostra o prompt com o diretório atual
        printf("\n%s> ", obter_caminho_atual(sistema->diretorio_atual));

        fgets(entrada, sizeof(entrada), stdin);
        entrada[strcspn(entrada, "\n")] = 0; // apaga o \n

        if (strlen(entrada) == 0)
            continue; // ignora entrada vazia

        comando = strtok(entrada, " ");
        argumento = strtok(NULL, "");

        if (strcmp(comando, "cd") == 0)
        {
            navegar_cd(sistema, argumento);
        }
        else if (strcmp(comando, "search") == 0)
        {
            if (argumento)
            {
                printf("Buscando por '%s'...\n", argumento);
                buscar_arquivo(sistema->raiz, argumento, "");
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
                remover_arquivo(sistema, argumento);
            }
            else
            {
                printf("Uso: rm <nome_do_arquivo>\n");
            }
        }
        else if (strcmp(comando, "list") == 0)
        {
            listar_diretorio(sistema->diretorio_atual);
        }
        else if (strcmp(comando, "mkdir") == 0)
        {
            if (argumento)
            {
                criar_diretorio(sistema, argumento);
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
            mostrar_ajuda();
        }
        else if (strcmp(comando, "exit") == 0)
        {
            printf("Saindo do programa...\n");
            continuar = 0;
        }
        else
        {
            printf("Comando '%s' não reconhecido. Digite 'help' para ver os comandos disponíveis.\n", comando);
        }

    } while (continuar);

    // libera a memória
    liberar_sistema(sistema);
    return 0;
}
