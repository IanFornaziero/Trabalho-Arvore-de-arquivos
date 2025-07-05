#ifndef FUNCOES_H
#define FUNCOES_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// representa um no da arvore (arquivo ou diretorio)
typedef struct No
{
    char nome[256];
    int ehDiretorio;          // 1 se for diretorio, 0 se for arquivo
    struct No *primeiroFilho; // primeiro filho (subdiretorio ou arquivo)
    struct No *proximoIrmao;  // proximo irmao no mesmo nivel
    struct No *pai;
} No;

typedef struct
{
    No *raiz;           // No raiz da arvore
    No *diretorioAtual; // Diretorio atual onde o usuario esta
} SistemaArquivos;

SistemaArquivos *inicializarSistema();
void liberarArvore(No *no);
void liberarSistema(SistemaArquivos *sistema);
No *criarNo(const char *nome, int ehDiretorio);
No *buscarFilho(No *pai, const char *nome);
void adicionarFilho(No *pai, No *filho);
void construirArvore(SistemaArquivos *sistema, const char *caminho);
void carregarArquivo(SistemaArquivos *sistema, const char *nomeArquivo);
void listarDiretorio(No *diretorio);
No *navegarCd(SistemaArquivos *sistema, const char *caminho);
void buscarArquivo(No *raiz, const char *nome, const char *caminhoAtual);
void removerArquivo(SistemaArquivos *sistema, const char *nome);
void criarDiretorio(SistemaArquivos *sistema, const char *nome);
void mostrarAjuda();
char *obterCaminhoAtual(No *diretorio);
void sugerirDiretorios(No *diretorioAtual, const char *prefixo);

#endif
