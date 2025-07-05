#ifndef FUNCOES_H
#define FUNCOES_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// representa um nó da árvore (arquivo ou diretório)
typedef struct No
{
    char nome[256];
    int eh_diretorio;          // 1 se for diretório, 0 se for arquivo
    struct No *primeiro_filho; // primeiro filho (subdiretório ou arquivo)
    struct No *proximo_irmao;  // próximo irmão no mesmo nível
    struct No *pai;
} No;

typedef struct
{
    No *raiz;
    No *diretorio_atual;
} SistemaArquivos;

SistemaArquivos *inicializar_sistema();
void liberar_arvore(No *no);
void liberar_sistema(SistemaArquivos *sistema);
No *criar_no(const char *nome, int eh_diretorio);
No *buscar_filho(No *pai, const char *nome);
void adicionar_filho(No *pai, No *filho);
void construir_arvore(SistemaArquivos *sistema, const char *caminho);
void carregar_arquivo(SistemaArquivos *sistema, const char *nome_arquivo);
void listar_diretorio(No *diretorio);
No *navegar_cd(SistemaArquivos *sistema, const char *caminho);
void buscar_arquivo(No *raiz, const char *nome, const char *caminho_atual);
void remover_arquivo(SistemaArquivos *sistema, const char *nome);
void criar_diretorio(SistemaArquivos *sistema, const char *nome);
void mostrar_ajuda();
char *obter_caminho_atual(No *diretorio);
void sugerir_diretorios(No *diretorio_atual, const char *prefixo);

#endif
