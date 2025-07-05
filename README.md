# Simulador de Sistema de Arquivos

Um simulador de linha de comando que implementa um sistema de arquivos hierárquico em C, permitindo navegação e manipulação de diretórios e arquivos.

## Como Compilar

```bash
gcc -o programa src/main.c src/Funcoes.c
```

## Como Executar

```bash
./programa
```

## Comandos Disponíveis

### Navegação
- `cd <diretorio>` - Navega para o diretório especificado
- `cd ..` - Volta para o diretório pai
- `cd /` - Vai para o diretório raiz

### Visualização
- `list` - Lista o conteúdo do diretório atual
- `type <nome>` - Lista o conteúdo de um diretório específico
- `search <nome>` - Busca por arquivo ou diretório em toda a árvore

### Manipulação
- `mkdir <nome>` - Cria um novo diretório
- `echo. <nome>` - Cria um novo arquivo
- `rm <nome>` - Remove arquivo ou diretório

### Utilidades
- `clear` - Limpa a tela
- `help` - Mostra os comandos disponíveis
- `exit` - Sai do programa

## Estrutura de Dados

O sistema utiliza uma árvore n-ária implementada com a técnica "First Child, Next Sibling":
- Cada nó representa um arquivo ou diretório
- Ponteiros para primeiro filho, próximo irmão e pai
- Navegação eficiente e uso otimizado de memória

## Exemplo de Uso

```
/ > list
Conteudo do diretorio:
[DIR]  Arquivos e Programas
[DIR]  Meus Documentos
[DIR]  Meus Downloads

/ > cd Meus Documentos
/Meus Documentos > list
Conteudo do diretorio:
[FILE] apresentacao.ppt
[FILE] relatorio.doc
[DIR]  fontes
[DIR]  imagens

/Meus Documentos > search main.c
Buscando por 'main.c'...
Encontrado: Meus Documentos/fontes/main.c
```

## Autores

- Ian Batista Fornaziero - RA: 2677210
- Gabriel Augusto Dupim - RA: 2651408
