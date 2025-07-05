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
void List(No *diretorio);
No *Cd(SistemaArquivos *sistema, const char *caminho);
void Search(No *raiz, const char *nome, const char *caminhoAtual);
void Rm(SistemaArquivos *sistema, const char *nome);
void Mkdir(SistemaArquivos *sistema, const char *nome);
void Help();
void Clear();
char *obterCaminhoAtual(No *diretorio);
void sugerirDiretorios(No *diretorioAtual, const char *prefixo);

#endif
