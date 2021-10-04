#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Função que lê paramestros e atribui aos ponteiros
void leitura_parametro(char *nomeInstancia, int *orcamento, int *saida, char *dimencao, FILE *entrada)
{
    char lixo[30];
    fscanf(entrada, "%s", lixo);
    fscanf(entrada, "%s", nomeInstancia);

    printf("%s\n", nomeInstancia);

    fscanf(entrada, "%s", lixo);
    fscanf(entrada, "%d", orcamento);

    printf("%d\n", *orcamento);

    fscanf(entrada, "%s", lixo);
    fscanf(entrada, "%d", saida);

    printf("%d\n", *saida);

    fscanf(entrada, "%s", lixo);
    fscanf(entrada, "%s", dimencao);
}

void aloca_arena(char **arena, int coluna)
{
    int i;
    /*Pedi memoria ao sistema*/
    for (i = 0; i < 6; i++)
    {
        arena[i] = (char *)malloc(coluna * sizeof(char));
        if (arena[i] == NULL)
        {
            printf("Memoria insuficiente.\n");
            exit(1);
        }
    }
}

void libera_arena(char **Arena)
{
    int y;
    for (y = 0; y < 6; y++)
    {
        free(Arena[y]);
    }
    free(Arena);
}

void leitura_arena(char **Arena, FILE *entrada)
{
    int y;

    for (y = 0; y < 6; y++)
    {
        fscanf(entrada, " %[^\n]", Arena[y]);
    }
}

void mostrar_arena(char **arena)
{
    int i;
    for (i = 0; i < 6; i++)
    {
        printf("%s\n", arena[i]);
    }
}

void separa_dimencao(int *linha, int *coluna, char *dimencao)
{
    char *pt;
    int cont = 0;
    pt = strtok(dimencao, "xX ");
    while (pt != NULL)
    {
        if (cont == 0)
        {
            *linha = atoi(pt);
        }
        else
        {
            *coluna = atoi(pt);
        }

        pt = strtok(NULL, "xX ");
        cont = cont + 1;
    }
}

int main(int argc, char *argv[])
{
    char **arena, nome_instancia[50], dimencao[25];

    int saida, orcamento, linha, coluna;

    FILE *entrada;
    if (argc - 1 == 1)
    {
        entrada = fopen(argv[1], "r");

        leitura_parametro(nome_instancia, &orcamento, &saida, dimencao, entrada);

        separa_dimencao(&linha, &coluna, dimencao);
        /*Alocação das linhas da matriz*/
        arena = (char **)malloc(linha * sizeof(char *));
        /*Verificação da alocação*/
        if (arena != NULL)
        {
            aloca_arena(arena, coluna);
            leitura_arena(arena, entrada);
            fclose(entrada);
            mostrar_arena(arena);
        }
        else
        {
            printf("Memoria insuficiente.\n");
            exit(1);
        }
        /*Liberaçao do espaço alocado*/
        libera_arena(arena);
    }
    else
    {
        printf("Exemplo de entrada .\\programa arquivo.dat");
    }

    return 0;
}