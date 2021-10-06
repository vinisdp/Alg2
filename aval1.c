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

int calcula_saida(pontos coordenadaSaida, int linha, int coluna)
{
    int sub1 = 0, sub2 = 0, tempo;
    if (coordenadaSaida.x > linha)
    {
        sub1 = coordenadaSaida.x - linha;
        if (coordenadaSaida.y > coluna)
        {
            sub2 = coordenadaSaida.y - coluna;
        }
        else
        {
            sub2 = coluna - coordenadaSaida.y;
        }
    }
    else
    {
        sub1 = linha - coordenadaSaida.x;
        if (coordenadaSaida.y > coluna)
        {
            sub2 = coordenadaSaida.y - coluna;
        }
        else
        {
            sub2 = coluna - coordenadaSaida.y;
        }
    }
    tempo = sub1 + sub2;
    return tempo;
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
void liberaNo(node **no, int linha)
{
    int y;
    for (y = 0; y < linha; y++)
    {
        free(no[y]);
    }
    free(no);
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

int isWalkable(char **matriz, int i, int j)
{
    int r;
    if (matriz[i][j] == ' ')
    {
        r = 1;
    }
    else if (matriz[i][j] == '#')
    {
        r = 0;
    }
    return r;
}

void iniciaNode(node **no, char **matriz, int linha, int coluna)
{
    int i, j;
    for (i = 0; i < linha; i++)
    {
        for (j = 0; j < coluna; j++)
        {
            no[i][j].andavel = isWalkable(matriz, i, j);
            no[i][j].listaAberta = 0;
            no[i][j].listaFechada = 0;
            no[i][j].g = 0;
            no[i][j].h = 0;
            no[i][j].f = 0;
            no[i][j].parenteLinha = -1;
            no[i][j].parenteColuna = -1;
        }
    }
}
pontos encontraParente(node **no, int linhaAtual, int colunaAtual, pontos saida)
{
    pontos coordenada;
    int dLinha, dColuna, linhaParente, colunaParente,menorF=0;
    coordenada.x=linhaAtual;
    coordenada.y=colunaAtual;
    for (dLinha = -1; dLinha <= 1; dLinha++)
    {
        for (dColuna = -1; dColuna <= 1; dColuna++)
        {
            linhaParente = linhaAtual - dLinha;
            colunaParente = colunaAtual - dColuna;
            if (dLinha != dColuna && no[linhaParente][colunaParente].andavel == 1)
            {
                no[linhaParente][colunaParente].parenteLinha = linhaAtual;
                no[linhaParente][colunaParente].parenteColuna = colunaAtual;
                no[linhaParente][colunaParente].g = no[linhaAtual][colunaAtual].g + 1;
                no[linhaParente][colunaParente].h = calcula_saida(saida,linhaParente,colunaParente);
                no[linhaParente][colunaParente].f = no[linhaParente][colunaParente].g + no[linhaParente][colunaParente].h;
                if(menorF>no[linhaParente][colunaParente].f){
                    menorF=no[linhaParente][colunaParente].f;
                    coordenada.x=linhaParente;
                    coordenada.y=colunaParente;
                }
            }
        }
    }
    printf("No:\nAndavel:%d \nCoordenadas:%d,%d\nG:%d\nH:%d\nF:%d\n", no[coordenada.x][coordenada.y].andavel, no[coordenada.x][coordenada.y].parenteLinha, no[coordenada.x][coordenada.y].parenteColuna,no[coordenada.x][coordenada.y].g,no[coordenada.x][coordenada.y].h,no[coordenada.x][coordenada.y].f);
    return coordenada;
}

void aloca_no(node **no, int coluna, int linha)
{
    int i;
    /*Pedi memoria ao sistema*/
    for (i = 0; i < linha; i++)
    {
        no[i] = (node *)malloc(coluna * sizeof(node));
        if (no[i] == NULL)
        {
            printf("Memoria insuficiente.\n");
            exit(1);
        }
    }
}
void procuraCaminho(node **no, pontos inicio, pontos coordenadaSaida,int saida){
    pontos coordenadaAtual;
    coordenadaAtual=inicio;
    while(coordenadaAtual.x!= coordenadaSaida.x && coordenadaAtual.y!= coordenadaSaida.y){
        coordenadaAtual=encontraParente(no,coordenadaAtual.x,coordenadaAtual.y,coordenadaSaida);
        if(no[coordenadaAtual.x][coordenadaAtual.y].h==1){
            coordenadaAtual=coordenadaSaida;
        }
    }
}
int main(int argc, char *argv[])
{
    char **arena, nome_instancia[50], dimensao[25];
    int saida, orcamento, linha, coluna, i, num_livre = 0;
    pontos saidas[10], inicio;
    node **no;
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
            printf("Alocando arena.\n");
            aloca_arena(arena, coluna, linha);
            printf("Lendo arena.\n");
            leitura_arena(arena, entrada, linha);
            printf("Fechando Arquivo.\n");
            fclose(entrada);
            printf("Mostrando arena.\n");
            mostrar_arena(arena, linha);
            printf("Identificando saida.\n");
            identifica_saidas(arena, linha, coluna, saidas, &inicio, &num_livre);
            printf("Alocando no.\n");
            no = (node **)malloc(linha * sizeof(node *));
            printf("Alocando no!");
            if (no == NULL)
            {
                printf("Memoria insuficiente.\n");
                exit(1);
            }
            printf("Alocando no!\n");
            aloca_no(no, linha, coluna);
            printf("Iniciando no!\n");
            iniciaNode(no, arena, linha, coluna);
            printf("inicio:%d %d\n", inicio.x, inicio.y);
            for(i=0;i<saida;i++){
                procuraCaminho(no,inicio,saidas[i],saida);
            }
            for (i = 0; i < saida; i++)
            {
                printf("Saidas:%d %d\n", saidas[i].x, saidas[i].y);
            }
            printf("inicio:%d %d\n", inicio.x, inicio.y);
        }
        else
        {
            printf("Memoria insuficiente.\n");
            exit(1);
        }
        /*Liberaçao do espaço alocado*/
        libera_arena(arena, linha);
        liberaNo(no,linha);
    }
    else
    {
        printf("Exemplo de entrada .\\programa arquivo.dat");
    }
    return 0;
}