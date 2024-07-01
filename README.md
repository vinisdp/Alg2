# Programas em C: Encontrar Menor Caminho em Labirinto e Análise de Palavras e Etiquetas em Arquivos de Texto

## Programa 1: Encontrar Menor Caminho em Labirinto

### Descrição

Este programa em C foi desenvolvido para encontrar o menor caminho em um labirinto representado por uma matriz. Utiliza o algoritmo A* (A-star) para calcular o caminho ótimo do ponto de início ao ponto de saída, considerando obstáculos e custos de movimentação.

#### Funcionalidades

- Leitura de parâmetros de entrada de um arquivo.
- Identificação de ponto de início, pontos de saída e áreas livres no labirinto.
- Cálculo do caminho mais curto usando o algoritmo A*.
- Saída do caminho encontrado e custo total.

#### Requisitos do Sistema

Para compilar e executar este programa, são necessários GCC e as seguintes bibliotecas padrão:

- `stdio.h`
- `stdlib.h`
- `string.h`

#### Compilação e Execução

Para compilar o programa, utilize o seguinte comando:

```bash
gcc programa_labirinto.c -o programa_labirinto
