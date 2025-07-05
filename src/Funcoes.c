#include "Funcoes.h"

SistemaArquivos *inicializar_sistema()
{
    SistemaArquivos *sistema = (SistemaArquivos *)malloc(sizeof(SistemaArquivos));
    sistema->raiz = criar_no("", 1);
    sistema->diretorio_atual = sistema->raiz;
    return sistema;
}

No *criar_no(const char *nome, int eh_diretorio)
{
    No *novo = (No *)malloc(sizeof(No));
    strcpy(novo->nome, nome);
    novo->eh_diretorio = eh_diretorio;
    novo->primeiro_filho = NULL;
    novo->proximo_irmao = NULL;
    novo->pai = NULL;
    return novo;
}

No *buscar_filho(No *pai, const char *nome)
{
    if (!pai || !pai->primeiro_filho)
        return NULL;

    No *atual = pai->primeiro_filho;
    while (atual)
    {
        if (strcmp(atual->nome, nome) == 0)
        {
            return atual;
        }
        atual = atual->proximo_irmao;
    }
    return NULL;
}

void adicionar_filho(No *pai, No *filho)
{
    filho->pai = pai;

    if (!pai->primeiro_filho)
    {
        pai->primeiro_filho = filho;
    }
    else
    {
        No *atual = pai->primeiro_filho;
        while (atual->proximo_irmao)
        {
            atual = atual->proximo_irmao;
        }
        atual->proximo_irmao = filho;
    }
}

void construir_arvore(SistemaArquivos *sistema, const char *caminho)
{
    char caminho_copia[512];
    strcpy(caminho_copia, caminho);

    No *atual = sistema->raiz;
    char *token = strtok(caminho_copia, "/");

    while (token)
    {
        No *filho = buscar_filho(atual, token);

        if (!filho)
        {
            // verifica se é o último token (pode ser arquivo)
            char *proximo_token = strtok(NULL, "/");
            int eh_diretorio = (proximo_token != NULL);

            // se não é o último, é definitivamente um diretório
            // se é o último, pode ser arquivo (sem extensão = diretório, com extensão = arquivo)
            if (!eh_diretorio)
            {
                eh_diretorio = (strchr(token, '.') == NULL);
            }

            filho = criar_no(token, eh_diretorio);
            adicionar_filho(atual, filho);

            // se havia um próximo token, continua a partir dele
            if (proximo_token)
            {
                token = proximo_token;
                atual = filho;
                continue;
            }
        }

        atual = filho;
        token = strtok(NULL, "/");
    }
}

// carrega o arquivo de entrada e constrói a árvore
void carregar_arquivo(SistemaArquivos *sistema, const char *nome_arquivo)
{
    FILE *arquivo = fopen(nome_arquivo, "r");
    if (!arquivo)
    {
        printf("Erro: Não foi possível abrir o arquivo %s\n", nome_arquivo);
        return;
    }

    char linha[512];
    while (fgets(linha, sizeof(linha), arquivo))
    {
        // remove quebra de linha
        linha[strcspn(linha, "\n")] = 0;

        if (strlen(linha) > 0)
        {
            construir_arvore(sistema, linha);
        }
    }

    fclose(arquivo);
    printf("Árvore de diretórios carregada com sucesso!\n");
}

// lista o conteúdo de um diretório
void listar_diretorio(No *diretorio)
{
    if (!diretorio || !diretorio->eh_diretorio)
    {
        printf("Erro: Não é um diretório válido\n");
        return;
    }

    if (!diretorio->primeiro_filho)
    {
        printf("Diretório vazio\n");
        return;
    }

    No *atual = diretorio->primeiro_filho;
    printf("Conteúdo do diretório:\n");
    while (atual)
    {
        if (atual->eh_diretorio)
        {
            printf("[DIR]  %s\n", atual->nome);
        }
        else
        {
            printf("[FILE] %s\n", atual->nome);
        }
        atual = atual->proximo_irmao;
    }
}

// navega para um diretório (comando cd)
No *navegar_cd(SistemaArquivos *sistema, const char *caminho)
{
    if (!caminho)
    {
        return sistema->diretorio_atual;
    }

    // se é "..", volta para o pai
    if (strcmp(caminho, "..") == 0)
    {
        if (sistema->diretorio_atual->pai)
        {
            sistema->diretorio_atual = sistema->diretorio_atual->pai;
        }
        return sistema->diretorio_atual;
    }

    // se é "/", vai para a raiz
    if (strcmp(caminho, "/") == 0)
    {
        sistema->diretorio_atual = sistema->raiz;
        return sistema->diretorio_atual;
    }

    // busca o diretório filho
    No *destino = buscar_filho(sistema->diretorio_atual, caminho);
    if (destino && destino->eh_diretorio)
    {
        sistema->diretorio_atual = destino;
        return destino;
    }
    else
    {
        // tenta sugerir diretórios similares
        sugerir_diretorios(sistema->diretorio_atual, caminho);
        return NULL;
    }
}

// sugere diretórios com base no prefixo digitado
void sugerir_diretorios(No *diretorio_atual, const char *prefixo)
{
    if (!diretorio_atual || !prefixo)
        return;

    int encontrou_sugestoes = 0;
    int len_prefixo = strlen(prefixo);

    // percorre todos os filhos do diretório atual
    No *filho = diretorio_atual->primeiro_filho;

    while (filho)
    {
        // verifica se é um diretório e se o nome começa com o prefixo
        if (filho->eh_diretorio && strncmp(filho->nome, prefixo, len_prefixo) == 0)
        {
            if (!encontrou_sugestoes)
            {
                printf("Diretório '%s' não encontrado. Você quis dizer:\n", prefixo);
                encontrou_sugestoes = 1;
            }
            printf("  - %s\n", filho->nome);
        }
        filho = filho->proximo_irmao;
    }

    if (!encontrou_sugestoes)
    {
        printf("Diretório '%s' não encontrado\n", prefixo);
    }
}

// busca recursivamente por um arquivo ou diretório
void buscar_arquivo(No *raiz, const char *nome, const char *caminho_atual)
{
    if (!raiz)
        return;

    // verifica se o nome atual corresponde ao que estamos buscando
    if (strlen(raiz->nome) > 0 && strcmp(raiz->nome, nome) == 0)
    {
        if (strlen(caminho_atual) > 0)
        {
            printf("Encontrado: %s/%s\n", caminho_atual, raiz->nome);
        }
        else
        {
            printf("Encontrado: %s\n", raiz->nome);
        }
    }

    // busca nos filhos se for um diretório
    if (raiz->eh_diretorio && raiz->primeiro_filho)
    {
        char novo_caminho[512];
        if (strlen(raiz->nome) > 0)
        {
            if (strlen(caminho_atual) > 0)
            {
                sprintf(novo_caminho, "%s/%s", caminho_atual, raiz->nome);
            }
            else
            {
                strcpy(novo_caminho, raiz->nome);
            }
        }
        else
        {
            strcpy(novo_caminho, caminho_atual);
        }

        No *filho = raiz->primeiro_filho;
        while (filho)
        {
            buscar_arquivo(filho, nome, novo_caminho);
            filho = filho->proximo_irmao;
        }
    }
}

// remove um arquivo ou diretório
void remover_arquivo(SistemaArquivos *sistema, const char *nome)
{
    No *pai = sistema->diretorio_atual;
    No *alvo = buscar_filho(pai, nome);

    if (!alvo)
    {
        printf("Erro: '%s' não encontrado\n", nome);
        return;
    }

    // remove da lista de filhos
    if (pai->primeiro_filho == alvo)
    {
        pai->primeiro_filho = alvo->proximo_irmao;
    }
    else
    {
        No *anterior = pai->primeiro_filho;
        while (anterior && anterior->proximo_irmao != alvo)
        {
            anterior = anterior->proximo_irmao;
        }
        if (anterior)
        {
            anterior->proximo_irmao = alvo->proximo_irmao;
        }
    }

    // libera memória recursivamente se for diretório
    liberar_arvore(alvo);
    printf("'%s' removido com sucesso\n", nome);
}

void criar_diretorio(SistemaArquivos *sistema, const char *nome)
{
    if (buscar_filho(sistema->diretorio_atual, nome))
    {
        printf("Erro: '%s' já existe\n", nome);
        return;
    }

    No *novo_dir = criar_no(nome, 1);
    adicionar_filho(sistema->diretorio_atual, novo_dir);
    printf("Diretório '%s' criado com sucesso\n", nome);
}

void mostrar_ajuda()
{
    printf("\t=== COMANDOS DISPONÍVEIS ===\n");
    printf("cd <diretorio>  - Navega para o diretório especificado.\n");
    printf("cd ..           - Volta para o diretório pai.\n");
    printf("cd /            - Vai para o diretório raiz.\n");
    printf("list            - Lista o conteúdo do diretório atual.\n");
    printf("search <nome>   - Busca por arquivo ou diretório.\n");
    printf("mkdir <nome>    - Cria um novo diretório.\n");
    printf("rm <nome>       - Remove arquivo ou diretório.\n");
    printf("clear           - Limpa a tela.\n");
    printf("help            - Mostra esta ajuda.\n");
    printf("exit            - Sai do programa.\n");
    printf("\t=============================\n\n");
}

char *obter_caminho_atual(No *diretorio)
{
    static char caminho[512];
    caminho[0] = '\0';

    if (!diretorio || !diretorio->pai)
    {
        strcpy(caminho, "/");
        return caminho;
    }

    char temp[512];
    strcpy(temp, obter_caminho_atual(diretorio->pai));

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

void liberar_arvore(No *no)
{
    if (!no)
        return;

    No *filho = no->primeiro_filho;
    while (filho)
    {
        No *proximo = filho->proximo_irmao;
        liberar_arvore(filho);
        filho = proximo;
    }

    free(no);
}

void liberar_sistema(SistemaArquivos *sistema)
{
    if (sistema)
    {
        liberar_arvore(sistema->raiz);
        free(sistema);
    }
}
