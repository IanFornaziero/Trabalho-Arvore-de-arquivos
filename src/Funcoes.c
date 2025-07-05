#include "Funcoes.h"

SistemaArquivos *inicializarSistema()
{
    SistemaArquivos *sistema = (SistemaArquivos *)malloc(sizeof(SistemaArquivos));
    sistema->raiz = criarNo("", 1);
    sistema->diretorioAtual = sistema->raiz;
    return sistema;
}

No *criarNo(const char *nome, int ehDiretorio)
{
    No *novo = (No *)malloc(sizeof(No));
    strcpy(novo->nome, nome);
    novo->ehDiretorio = ehDiretorio;
    novo->primeiroFilho = NULL;
    novo->proximoIrmao = NULL;
    novo->pai = NULL;
    return novo;
}

No *buscarFilho(No *pai, const char *nome)
{
    if (!pai || !pai->primeiroFilho)
        return NULL;

    No *atual = pai->primeiroFilho;
    while (atual)
    {
        if (strcmp(atual->nome, nome) == 0)
        {
            return atual;
        }
        atual = atual->proximoIrmao;
    }
    return NULL;
}

void adicionarFilho(No *pai, No *filho)
{
    filho->pai = pai;

    if (!pai->primeiroFilho)
    {
        pai->primeiroFilho = filho;
    }
    else
    {
        No *atual = pai->primeiroFilho;
        while (atual->proximoIrmao)
        {
            atual = atual->proximoIrmao;
        }
        atual->proximoIrmao = filho;
    }
}

void construirArvore(SistemaArquivos *sistema, const char *caminho)
{
    char caminhoCopia[512];
    strcpy(caminhoCopia, caminho);

    No *atual = sistema->raiz;
    char *token = strtok(caminhoCopia, "/");

    while (token)
    {
        No *filho = buscarFilho(atual, token);

        if (!filho)
        {
            // verifica se e o ultimo token (pode ser arquivo)
            char *proximoToken = strtok(NULL, "/");
            int ehDiretorio = (proximoToken != NULL);

            // se nao e o ultimo, e definitivamente um diretorio
            // se e o ultimo, pode ser arquivo (sem extensao = diretorio, com extensao = arquivo)
            if (!ehDiretorio)
            {
                ehDiretorio = (strchr(token, '.') == NULL);
            }

            filho = criarNo(token, ehDiretorio);
            adicionarFilho(atual, filho);

            // se havia um proximo token, continua a partir dele
            if (proximoToken)
            {
                token = proximoToken;
                atual = filho;
                continue;
            }
        }

        atual = filho;
        token = strtok(NULL, "/");
    }
}

// carrega o arquivo de entrada e constroi a arvore
void carregarArquivo(SistemaArquivos *sistema, const char *nomeArquivo)
{
    FILE *arquivo = fopen(nomeArquivo, "r");
    if (!arquivo)
    {
        printf("Erro: Nao foi possivel abrir o arquivo %s\n", nomeArquivo);
        return;
    }

    char linha[512];
    while (fgets(linha, sizeof(linha), arquivo))
    {
        // remove quebra de linha
        linha[strcspn(linha, "\n")] = 0;

        if (strlen(linha) > 0)
        {
            construirArvore(sistema, linha);
        }
    }

    fclose(arquivo);
    printf("Arvore de diretorios carregada com sucesso!\n");
}

// lista o conteudo de um diretorio
void List(No *diretorio)
{
    if (!diretorio || !diretorio->ehDiretorio)
    {
        printf("Erro: Nao e um diretorio valido\n");
        return;
    }

    if (!diretorio->primeiroFilho)
    {
        printf("Diretorio vazio\n");
        return;
    }

    No *atual = diretorio->primeiroFilho;
    printf("Conteudo do diretorio:\n");
    while (atual)
    {
        if (atual->ehDiretorio)
        {
            printf("[DIR]  %s\n", atual->nome);
        }
        else
        {
            printf("[FILE] %s\n", atual->nome);
        }
        atual = atual->proximoIrmao;
    }
}

// navega para um diretorio (comando cd)
No *Cd(SistemaArquivos *sistema, const char *caminho)
{
    if (!caminho)
    {
        return sistema->diretorioAtual;
    }

    // se e "..", volta para o pai
    if (strcmp(caminho, "..") == 0)
    {
        if (sistema->diretorioAtual->pai)
        {
            sistema->diretorioAtual = sistema->diretorioAtual->pai;
        }
        return sistema->diretorioAtual;
    }

    // se e "/", vai para a raiz
    if (strcmp(caminho, "/") == 0)
    {
        sistema->diretorioAtual = sistema->raiz;
        return sistema->diretorioAtual;
    }

    // busca o diretorio filho
    No *destino = buscarFilho(sistema->diretorioAtual, caminho);
    if (destino && destino->ehDiretorio)
    {
        sistema->diretorioAtual = destino;
        return destino;
    }
    else
    {
        // tenta sugerir diretorios similares
        sugerirDiretorios(sistema->diretorioAtual, caminho);
        return NULL;
    }
}

// sugere diretorios com base no prefixo digitado
void sugerirDiretorios(No *diretorioAtual, const char *prefixo)
{
    if (!diretorioAtual || !prefixo)
        return;

    int encontrouSugestoes = 0;
    int lenPrefixo = strlen(prefixo);

    // percorre todos os filhos do diretorio atual
    No *filho = diretorioAtual->primeiroFilho;

    while (filho)
    {
        // verifica se e um diretorio e se o nome comeca com o prefixo
        if (filho->ehDiretorio && strncmp(filho->nome, prefixo, lenPrefixo) == 0)
        {
            if (!encontrouSugestoes)
            {
                printf("Diretorio '%s' nao encontrado. Voce quis dizer:\n", prefixo);
                encontrouSugestoes = 1;
            }
            printf("  - %s\n", filho->nome);
        }
        filho = filho->proximoIrmao;
    }

    if (!encontrouSugestoes)
    {
        printf("Diretorio '%s' nao encontrado\n", prefixo);
    }
}

// busca recursivamente por um arquivo ou diretorio
void Search(No *raiz, const char *nome, const char *caminhoAtual)
{
    if (!raiz)
        return;

    // verifica se o nome atual corresponde ao que estamos buscando
    if (strlen(raiz->nome) > 0 && strcmp(raiz->nome, nome) == 0)
    {
        if (strlen(caminhoAtual) > 0)
        {
            printf("Encontrado: %s/%s\n", caminhoAtual, raiz->nome);
        }
        else
        {
            printf("Encontrado: %s\n", raiz->nome);
        }
    }

    // busca nos filhos se for um diretorio
    if (raiz->ehDiretorio && raiz->primeiroFilho)
    {
        char novoCaminho[512];
        if (strlen(raiz->nome) > 0)
        {
            if (strlen(caminhoAtual) > 0)
            {
                sprintf(novoCaminho, "%s/%s", caminhoAtual, raiz->nome);
            }
            else
            {
                strcpy(novoCaminho, raiz->nome);
            }
        }
        else
        {
            strcpy(novoCaminho, caminhoAtual);
        }

        No *filho = raiz->primeiroFilho;
        while (filho)
        {
            buscarArquivo(filho, nome, novoCaminho);
            filho = filho->proximoIrmao;
        }
    }
}

// remove um arquivo ou diretorio
void Rm(SistemaArquivos *sistema, const char *nome)
{
    No *pai = sistema->diretorioAtual;
    No *alvo = buscarFilho(pai, nome);

    if (!alvo)
    {
        printf("Erro: '%s' nao encontrado\n", nome);
        return;
    }

    // remove da lista de filhos
    if (pai->primeiroFilho == alvo)
    {
        pai->primeiroFilho = alvo->proximoIrmao;
    }
    else
    {
        No *anterior = pai->primeiroFilho;
        while (anterior && anterior->proximoIrmao != alvo)
        {
            anterior = anterior->proximoIrmao;
        }
        if (anterior)
        {
            anterior->proximoIrmao = alvo->proximoIrmao;
        }
    }

    // libera memoria recursivamente se for diretorio
    liberarArvore(alvo);
    printf("'%s' removido com sucesso\n", nome);
}

void Mkdir(SistemaArquivos *sistema, const char *nome)
{
    if (buscarFilho(sistema->diretorioAtual, nome))
    {
        printf("Erro: '%s' ja existe\n", nome);
        return;
    }

    No *novoDir = criarNo(nome, 1);
    adicionarFilho(sistema->diretorioAtual, novoDir);
    printf("Diretorio '%s' criado com sucesso\n", nome);
}

void Help()
{
    printf("\t=== COMANDOS DISPONIVEIS ===\n");
    printf("cd <diretorio>  - Navega para o diretorio especificado.\n");
    printf("cd ..           - Volta para o diretorio pai.\n");
    printf("cd /            - Vai para o diretorio raiz.\n");
    printf("list            - Lista o conteudo do diretorio atual.\n");
    printf("search <nome>   - Busca por arquivo ou diretorio.\n");
    printf("mkdir <nome>    - Cria um novo diretorio.\n");
    printf("rm <nome>       - Remove arquivo ou diretorio.\n");
    printf("clear           - Limpa a tela.\n");
    printf("help            - Mostra esta ajuda.\n");
    printf("exit            - Sai do programa.\n");
    printf("\t=============================\n\n");
}

void Clear()
{
#ifdef _WIN32
    system("cls");
#elif __APPLE__ || __linux__
    system("clear");
#else
    printf("Não foi possível limpar a tela neste sistema.\n");
#endif
}

char *obterCaminhoAtual(No *diretorio)
{
    static char caminho[512];
    caminho[0] = '\0';

    if (!diretorio || !diretorio->pai)
    {
        strcpy(caminho, "/");
        return caminho;
    }

    char temp[512];
    strcpy(temp, obterCaminhoAtual(diretorio->pai));

    if (strlen(diretorio->nome) > 0)
    {
        if (strcmp(temp, "/") != 0)
        {
            strcat(temp, "/");
        }
        strcat(temp, diretorio->nome);
    }

    strcpy(caminho, temp);
    return caminho;
}

void liberarArvore(No *no)
{
    if (!no)
        return;

    No *filho = no->primeiroFilho;
    while (filho)
    {
        No *proximo = filho->proximoIrmao;
        liberarArvore(filho);
        filho = proximo;
    }

    free(no);
}

void liberarSistema(SistemaArquivos *sistema)
{
    if (sistema)
    {
        liberarArvore(sistema->raiz);
        free(sistema);
    }
}