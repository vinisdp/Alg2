# Programa de Encontrar Menor Caminho em Labirinto

Este programa em C foi desenvolvido para encontrar o menor caminho em um labirinto representado por uma matriz. Ele utiliza o algoritmo A* (A-star) para encontrar o caminho ótimo do ponto de início ao ponto de saída, considerando obstáculos e custos de movimentação.

## Funcionalidades

- Leitura de parâmetros de entrada a partir de um arquivo.
- Identificação do ponto de início, pontos de saída e áreas livres no labirinto.
- Cálculo do caminho mais curto usando o algoritmo A*.
- Saída do caminho encontrado e custo total.

## Requisitos do Sistema

Para compilar e executar este programa, você precisa ter um ambiente de desenvolvimento C configurado, como GCC, e as seguintes bibliotecas padrão:

- `stdio.h`
- `stdlib.h`
- `string.h`

## Compilação e Execução

Para compilar o programa, execute o seguinte comando:

```bash
gcc programa.c -o programa
-----------------------------------------------------------------------------------------------------------------------------------------------------

# Análise de Palavras e Etiquetas em Arquivos de Texto

Este programa em C realiza a análise de palavras e etiquetas em arquivos de texto, contabilizando frequências de ocorrência e gerando um arquivo CSV como saída. Ele foi desenvolvido como parte de um trabalho acadêmico pelos alunos Vinicius Silva de Paula e Julio Lossavaro.

## Funcionalidades

### 1. Estruturas de Dados Utilizadas

O programa utiliza uma estrutura de lista encadeada para armazenar as palavras e suas frequências, bem como as etiquetas (tags) associadas.

- **Estrutura `celula`**:
  - `int frequencia`: Armazena a frequência de ocorrência da palavra.
  - `char palavra[100]`: Armazena a palavra encontrada no arquivo.
  - `struct cel *prox`: Ponteiro para a próxima célula na lista.

### 2. Funções Principais

#### Função `aloca`

- **Descrição**: Aloca dinamicamente uma nova célula (`struct cel`) para armazenar uma palavra.
- **Uso**: Utilizada para criar novas células que serão inseridas na lista encadeada.

#### Função `insere`

- **Descrição**: Insere uma palavra na lista encadeada de acordo com sua ordem lexicográfica. Se a palavra já existe na lista, incrementa sua frequência.
- **Uso**: Utilizada para inserir palavras nas listas de palavras e etiquetas durante a leitura do arquivo.

#### Função `le_arquivo`

- **Descrição**: Realiza a leitura do arquivo de entrada, separa palavras e etiquetas utilizando `strtok`, e insere essas informações nas listas encadeadas.
- **Uso**: Utilizada para processar o arquivo de entrada e alimentar as estruturas de dados do programa.

#### Função `mostra_lista`

- **Descrição**: Percorre a lista encadeada e imprime suas células no arquivo de saída CSV.
- **Uso**: Utilizada para gerar a seção de palavras e etiquetas no arquivo de saída.

#### Função `conta_palavras`

- **Descrição**: Percorre a lista encadeada e conta o número total de palavras e o número de palavras distintas (sem repetições).
- **Uso**: Utilizada para calcular estatísticas sobre o conteúdo do arquivo processado.

#### Função `salva_arquivo`

- **Descrição**: Gera o arquivo de saída CSV com informações sobre as palavras e etiquetas encontradas no arquivo de entrada.
- **Uso**: Utilizada para escrever as estatísticas e as listas de palavras e etiquetas no arquivo de saída.

### 3. Compilação e Execução

Para compilar e executar o programa, utilize os seguintes passos:

```bash
gcc -o analise_palavras main.c
./analise_palavras arquivo_de_entrada.pos
