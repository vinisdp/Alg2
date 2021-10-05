#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct
{
    int x, y;
} pontos;

typedef struct 
{
    int andavel;
    int listaAberta;
    int listaFechada;
    int g;
    int h;
    int f;
    int parenteLinha;
    int parenteColuna;
} node;


/*Função para fazer a leitura das informações da arena, ela recebe por
parametro as variaveis e o arquivo de entrada, apos isso é criada uma
variavel para armazenar as informações que não são uteis para o programa
e a leitura é feita utilizando o fscanf*/
void leitura_parametro(char *nomeInstancia, int *orcamento, int *saida, char *dimensao, FILE *entrada)
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
    fscanf(entrada, "%s", dimensao);
}

void calcula_saida(pontos saidas[10], int saida, pontos *inicio, int tempo[10])
{
    int i, sub1 = 0, sub2 = 0;
    for (i = 0; i < saida; i++)
    {
        if (saidas[i].x > inicio->x)
        {
            sub1 = saidas[i].x - inicio->x;
            if (saidas[i].y > inicio->y)
            {
                sub2 = saidas[i].y - inicio->y;
            }
            else
            {
                sub2 = inicio->y - saidas[i].y;
            }
        }
        else
        {
            sub1 = inicio->x - saidas[i].x;
            if (saidas[i].y > inicio->y)
            {
                sub2 = saidas[i].y - inicio->y;
            }
            else
            {
                sub2 = inicio->y - saidas[i].y;
            }
        }
        tempo[i] = sub1 + sub2;
    }
}
/*Função para alocar os vetores que serão utilizados para
coluna da matriz da arena nela é recebido quantidade de linhas,
colunas e a propria matriz e aloca os vetores com o tamanho das
colunas em cada linha do vertor que armazena os endereços*/
void aloca_arena(char **arena, int coluna, int linha)
{
    int i;
    /*Pedi memoria ao sistema*/
    for (i = 0; i < linha; i++)
    {
        arena[i] = (char *)malloc(coluna * sizeof(char));
        if (arena[i] == NULL)
        {
            printf("Memoria insuficiente.\n");
            exit(1);
        }
    }
}
/*Função para liberar a memoria alocada para matriz
arena utilizando a função arena*/
void libera_arena(char **Arena, int linha)
{
    int y;
    for (y = 0; y < linha; y++)
    {
        free(Arena[y]);
    }
    free(Arena);
}
/*Função para a leitura das linhas da arena
de dentro do arquivo usando o fscanf*/
void leitura_arena(char **Arena, FILE *entrada, int linha)
{
    int y;

    for (y = 0; y < linha; y++)
    {
        fscanf(entrada, " %[^\n]", Arena[y]);
    }
}
/*Função para imprimir a arena*/
void mostrar_arena(char **arena, int linha)
{
    int i;
    for (i = 0; i < linha; i++)
    {
        printf("%s\n", arena[i]);
    }
}
void mapeia_campo_livre(char **arena, int linha, int coluna, pontos *livre)
{
    int l, c, auxl = 0;
    for (l = 0; l < linha; l++)
    {
        for (c = 0; c < coluna; c++)
        {
            if (arena[l][c] == ' ')
            {
                livre[auxl].x = l;
                livre[auxl].y = c;
                auxl++;
            }
        }
    }
}
void identifica_saidas(char **arena, int linha, int coluna, pontos saidas[10], pontos *inicio, int *num_livre)
{
    int l, c, auxl = 0;
    for (l = 0; l < linha; l++)
    {
        for (c = 0; c < coluna; c++)
        {
            if (arena[l][c] == '0' || arena[l][c] == '1' || arena[l][c] == '2' || arena[l][c] == '3' || arena[l][c] == '4' || arena[l][c] == '5' || arena[l][c] == '8' || arena[l][c] == '9')
            {
                saidas[auxl].x = l;
                saidas[auxl].y = c;
                auxl++;
            }
            else if (arena[l][c] == '*')
            {
                inicio->x = l;
                inicio->y = c;
            }
            else if (arena[l][c] == ' ')
            {
                *num_livre = *num_livre + 1;
            }
        }
    }
}
/*Nessa função é utilizado o strtok para separar a quantidade
de linhas e colunas essa função permite que escolhemos caracteres
chaves para que haja a separação das duas partes da string
alem disso também foi utilizado o atoi*/
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

int heuristic(int linha1, int coluna1, int linha2, int coluna2)
{
    int linhah = (abs(linha2 - linha1) * 10);
    int colunah = (abs(coluna2 - coluna1) * 10);
    return max(linhah, colunah); //ALTERAR PARA linhah + COLUNA, já que não andamos na diagonal
}

int isWalkable(char **matriz, int i, int j)
{
    if (matriz[i][j] == ' ')
        return TRUE;

    else if (matriz[i][j] == '#')
        return FALSE;
}

node **iniciaNode(char **matriz, int linha, int coluna)
{
    node **no;
    int i, j;
    for (i = 0; i < linha; i++)
    {
        for (j = 0; j < coluna; j++)
        {
            no[i][j].andavel = isWalkable(matriz, i, j);
            no[i][j].listaAberta = FALSE;
            no[i][j].listaFechada = FALSE;
            no[i][j].g = 0;
            no[i][j].h = 0;
            no[i][j].f = 0;
            no[i][j].parenteLinha = NULL;
            no[i][j].parenteColuna = NULL;
        }
    }
    return no;
}

void encontraMenorCaminho(node **matriz ,int iniciox, int inicioy, int finalx, int finaly)
{

    int x, y;
    int dx, dy;
    int linhaatual = iniciox;
    int colunaatual = inicioy;
    int menorf = 1000;

    while (linhaatual != alvoL && colunaatual != alvoC)
    {
        for (i = 0; i < linha; i++)
        {
            for (j = 0; j < coluna; j++)
            {
                if (no[i][j].listaAberta == 1)
                    if (no[i][j].f < menorF)
                    {
                        linhaatual = i;
                        colunaatual = j;
                        menorF = no[linhaatual][colunaatual].f;
                    }
            }
        }

        matriz[linhaatual][colunaatual].listaAberta = FALSE;
        matriz[linhaatual][colunaatual] = TRUE;

        for (dlinha = -1; dlinha <= 1; dlinha++)
        {
            for (dcoluna = -1; dcoluna <= 1; dcoluna++)
            {
                if (no[linhaatual + dlinha][colunaatual + dcoluna].andavel && no[linhaatual + dlinha][colunaatual + dcoluna].listaFechada)
            }
        }
    }
}

int main(int argc, char *argv[])
{
    char **arena, nome_instancia[50], dimensao[25];
    int saida, orcamento, linha, coluna, i, tempo[10], num_livre = 0;
    pontos saidas[10], inicio, *livre, pilha[20];
    FILE *entrada;
    if (argc == 2)
    {
        entrada = fopen(argv[1], "r");
        leitura_parametro(nome_instancia, &orcamento, &saida, dimensao, entrada);
        separa_dimencao(&linha, &coluna, dimensao);
        /*Alocação das linhas da matriz*/
        arena = (char **)malloc(linha * sizeof(char *));
        /*Verificação da alocação*/
        if (arena != NULL)
        {
            aloca_arena(arena, coluna, linha);
            leitura_arena(arena, entrada, linha);
            fclose(entrada);
            mostrar_arena(arena, linha);
            identifica_saidas(arena, linha, coluna, saidas, &inicio, &num_livre);
            livre = (pontos *)malloc(num_livre * sizeof(pontos));
            mapeia_campo_livre(arena, linha, coluna, livre);
            for (i = 0; i < saida; i++)
            {
                printf("Saidas:%d %d\n", saidas[i].x, saidas[i].y);
            }
            printf("inicio:%d %d\n", inicio.x, inicio.y);
            calcula_saida(saidas, saida, &inicio, tempo);
            for (i = 0; i < saida; i++)
            {
                printf("%d \n", tempo[i]);
            }
            for (i = 0; i < num_livre; i++)
            {
                printf("Livre:%d %d \n", livre[i].x, livre[i].y);
            }
        }
        else
        {
            printf("Memoria insuficiente.\n");
            exit(1);
        }
        /*Liberaçao do espaço alocado*/
        libera_arena(arena, linha);
        free(livre);
    }
    else
    {
        printf("Exemplo de entrada .\\programa arquivo.dat");
    }
    return 0;
}