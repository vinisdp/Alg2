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
    char direcao;
} node;

/*FunÃƒÂ§ÃƒÂ£o para fazer a leitura das informaÃƒÂ§ÃƒÂµes da arena, ela recebe por
parametro as variaveis e o arquivo de entrada, apos isso ÃƒÂ© criada uma
variavel para armazenar as informaÃƒÂ§ÃƒÂµes que nÃƒÂ£o sÃƒÂ£o uteis para o programa
e a leitura ÃƒÂ© feita utilizando o fscanf*/
void leitura_parametro(char *nomeInstancia, int *orcamento, int *saida, char *dimensao, FILE *entrada)
{
    char lixo[30];
    fscanf(entrada, "%s", lixo);
    fscanf(entrada, "%s", nomeInstancia);

    fscanf(entrada, "%s", lixo);
    fscanf(entrada, "%d", orcamento);

    fscanf(entrada, "%s", lixo);
    fscanf(entrada, "%d", saida);

    fscanf(entrada, "%s", lixo);
    fscanf(entrada, "%s", dimensao);
}

int calcula_saida(int linha, int coluna, int finalx, int finaly)
{

    int sub1 = 0,
        sub2 = 0, tempo;

    if (finalx > linha)
    {
        sub1 = finalx - linha;
        if (finaly > coluna)
        {
            sub2 = finaly - coluna;
        }
        else
        {
            sub2 = coluna - finaly;
        }
    }
    else
    {
        sub1 = linha - finalx;
        if (finaly > coluna)
        {
            sub2 = finaly - coluna;
        }
        else
        {
            sub2 = coluna - finaly;
        }
    }
    tempo = sub1 + sub2;
    return tempo;
}
/*FunÃƒÂ§ÃƒÂ£o para alocar os vetores que serÃƒÂ£o utilizados para
coluna da matriz da arena nela ÃƒÂ© recebido quantidade de linhas,
colunas e a propria matriz e aloca os vetores com o tamanho das
colunas em cada linha do vertor que armazena os endereÃƒÂ§os*/
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
/*FunÃƒÂ§ÃƒÂ£o para liberar a memoria alocada para matriz
arena utilizando a funÃƒÂ§ÃƒÂ£o arena*/
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
/*FunÃƒÂ§ÃƒÂ£o para a leitura das linhas da arena
de dentro do arquivo usando o fscanf*/
void leitura_arena(char **Arena, FILE *entrada, int linha)
{
    int y;

    for (y = 0; y < linha; y++)
    {
        fscanf(entrada, " %[^\n]", Arena[y]);
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
/*Nessa funÃƒÂ§ÃƒÂ£o ÃƒÂ© utilizado o strtok para separar a quantidade
de linhas e colunas essa funÃƒÂ§ÃƒÂ£o permite que escolhemos caracteres
chaves para que haja a separaÃƒÂ§ÃƒÂ£o das duas partes da string
alem disso tambÃƒÂ©m foi utilizado o atoi*/
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

int isWalkable(char **arena, int l, int c)
{
    int r;
    if (arena[l][c] == '0' || arena[l][c] == '1' || arena[l][c] == '2' || arena[l][c] == '3' || arena[l][c] == '4' || arena[l][c] == '5' || arena[l][c] == '6' || arena[l][c] == '7' || arena[l][c] == '8' || arena[l][c] == '9' || arena[l][c] == ' ' || arena[l][c] == '*')
    {
        r = 1;
    }
    else
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
            no[i][j].direcao = 'z';
        }
    }
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

int encontraMenorCaminho(node **matriz, pontos *ponto, int linha, int coluna, int iniciox, int inicioy, int finalx, int finaly, int nsaida, FILE *arqsaida, int orcamento)
{
    int i, j;
    int dx, dy, count, flag, conta_vizinho_andavel;

    int linhaatual = iniciox;
    int colunaatual = inicioy;
    int menorf, flag2;

    matriz[linhaatual][colunaatual].listaAberta = 1;
    matriz[linhaatual][colunaatual].listaFechada = 0;
    flag2 = 0;

    while (linhaatual != finalx || colunaatual != finaly)
    {
        conta_vizinho_andavel = 0;
        /*percorre toda a matriz em busca do menor F, e em seguida o passa co mo o nÃƒÂ³ atual*/
        menorf = 1000;
        flag = 0;
        for (i = 0; i < linha; i++)
        {
            for (j = 0; j < coluna; j++)
            {
                if (matriz[i][j].listaAberta == 1)
                {
                    flag = 1;
                    if (matriz[i][j].f < menorf)
                    {
                        linhaatual = i;
                        colunaatual = j;
                        menorf = matriz[linhaatual][colunaatual].f;
                    }
                }
            }
        }

        if (linhaatual == finalx && colunaatual == finaly)
        {
            flag2 = 1;
        }
        if (flag == 0 || flag2 == 1)
        {
            break;
        }
        else
        {
            matriz[linhaatual][colunaatual].listaAberta = 0;
            matriz[linhaatual][colunaatual].listaFechada = 1;

            for (dx = -1; dx <= 1; dx++)
            {
                for (dy = -1; dy <= 1; dy++)
                {

                    if (dx != 0 || dy != 0)
                        if ((dx == 0 || dy == 0))
                        {

                            if ((linhaatual + dx) < linha && (colunaatual + dy) < coluna && linhaatual + dx > -1 && colunaatual + dy > -1)
                            {

                                if (matriz[linhaatual + dx][colunaatual + dy].andavel == 1 && matriz[linhaatual + dx][colunaatual + dy].listaFechada == 0)
                                {
                                    conta_vizinho_andavel++;
                                    /*se nao estiver na lista aberta*/
                                    if (matriz[linhaatual + dx][colunaatual + dy].listaAberta == 0)
                                    {

                                        /*adicionando a lista aberta*/
                                        matriz[linhaatual + dx][colunaatual + dy].listaAberta = 1;
                                        matriz[linhaatual + dx][colunaatual + dy].listaFechada = 0;
                                        /*fazendo o no atual seu parente*/
                                        matriz[linhaatual + dx][colunaatual + dy].parenteLinha = linhaatual;
                                        matriz[linhaatual + dx][colunaatual + dy].parenteColuna = colunaatual;
                                        /*acumulando o G*/
                                        matriz[linhaatual + dx][colunaatual + dy].g = matriz[linhaatual][colunaatual].g + 1;
                                        /*calculando o H*/
                                        /*Metodo manhattan*/
                                        matriz[linhaatual + dx][colunaatual + dy].h = calcula_saida((linhaatual + dx), (colunaatual + dy), finalx, finaly);
                                        matriz[linhaatual + dx][colunaatual + dy].f = matriz[linhaatual + dx][colunaatual + dy].g + matriz[linhaatual + dx][colunaatual + dy].h;
                                    }
                                }
                            }

                        } /*se for parede*/
                }         /* fim while*/
            }
        }

        if (conta_vizinho_andavel == 0 && flag == 0)
        {
            fprintf(arqsaida, "SaÃ­da %d: NÃ£o alcanÃ§ada!\n", nsaida);
            count = -1;
        }
    }

    linhaatual = finalx;
    colunaatual = finaly;

    if (flag2 == 1)
    {
        count = 0;
        ponto[count].x = linhaatual;
        ponto[count].y = colunaatual;

        while (linhaatual != iniciox || colunaatual != inicioy)
        {
            count++;
            if ((linhaatual - matriz[linhaatual][colunaatual].parenteLinha) == 0 && colunaatual - matriz[linhaatual][colunaatual].parenteColuna == 1)
            {
                matriz[linhaatual][colunaatual].direcao = 'd';
            }
            else if ((linhaatual - matriz[linhaatual][colunaatual].parenteLinha) == 0 && colunaatual - matriz[linhaatual][colunaatual].parenteColuna == -1)
            {
                matriz[linhaatual][colunaatual].direcao = 'e';
            }

            else if ((linhaatual - matriz[linhaatual][colunaatual].parenteLinha) == -1 && colunaatual - matriz[linhaatual][colunaatual].parenteColuna == 0)
            {
                matriz[linhaatual][colunaatual].direcao = 'c';
            }

            else if ((linhaatual - matriz[linhaatual][colunaatual].parenteLinha) == 1 && colunaatual - matriz[linhaatual][colunaatual].parenteColuna == 0)
            {
                matriz[linhaatual][colunaatual].direcao = 'b';
            }

            ponto[count].x = matriz[linhaatual][colunaatual].parenteLinha;
            ponto[count].y = matriz[linhaatual][colunaatual].parenteColuna;

            linhaatual = ponto[count].x;
            colunaatual = ponto[count].y;
        }

        if (count > orcamento)
        {

            fprintf(arqsaida, "SaÃ­da %d: NÃ£o alcanÃ§ada!\n", nsaida);
            count = -1;
        }
        else
        {
            j = count - 1;
            fprintf(arqsaida, "SaÃ­da %d: %d segundos-> ", nsaida, count);
            while (j != -1)
            {
                fprintf(arqsaida, "%c", matriz[ponto[j].x][ponto[j].y].direcao);
                j--;
            }
            fprintf(arqsaida, "\n");
        }
    }
    else
    {
        fprintf(arqsaida, "SaÃ­da %d: NÃ£o alcanÃ§ada!\n", nsaida);
        count = -1;
    }
    return count;
}

int main(int argc, char *argv[])
{
    char **arena, nome_instancia[50] = "", dimensao[25], caminho[25], nomearquivo[60] = "";
    int saida, orcamento, linha, coluna, i, j, num_livre = 0, count = 0, saidas_alcancadas = 0, menor_tempo = 10000, menor_saida;

    pontos saidas[10], inicio, *menorCaminho;
    node **no;
    FILE *entrada, *arqsaida;

    if (argc == 2)
    {
        entrada = fopen(argv[1], "r");
        leitura_parametro(nome_instancia, &orcamento, &saida, dimensao, entrada);
        separa_dimencao(&linha, &coluna, dimensao);
        strcpy(nomearquivo, nome_instancia);
        strcat(nomearquivo, ".out");
        arqsaida = fopen(nomearquivo, "w");

        arena = (char **)malloc(linha * sizeof(char *));
        /*VerificaÃƒÂ§ÃƒÂ£o da alocaÃƒÂ§ÃƒÂ£o*/
        if (arena != NULL)
        {

            /*aloca_arena(arena, coluna, linha);*/
            for (i = 0; i < linha; i++)
            {
                arena[i] = (char *)malloc(coluna * sizeof(char));
                if (arena[i] == NULL)
                {
                    printf("Memoria insuficiente.\n");
                    exit(1);
                }
            }
            leitura_arena(arena, entrada, linha);
            fclose(entrada);
            identifica_saidas(arena, linha, coluna, saidas, &inicio, &num_livre);
        }
        else
        {
            printf("Memoria insuficiente.\n");
            exit(1);
        }

        if (saida == 0)
        {
            printf("InstÃ¢ncia %s\n", nome_instancia);
            printf("OrÃ§amento disponÃ­vel: %d segundos\n\n", orcamento);
            printf("Sem saÃ­da!\n\n");
            printf("Arquivo %s.out gerado com sucesso!", nome_instancia);

            fprintf(arqsaida, "PosiÃ§Ã£o inicial[x,y]: [%d,%d]\n", inicio.x, inicio.y);

            fprintf(arqsaida, "OrÃ§amento disponÃ­vel: %d segundos\n\n", orcamento);
            fprintf(arqsaida, "Arena sem saÃ­da!\n");
            fclose(arqsaida);
        }
        else
        {

            /*AlocaÃƒÂ§ÃƒÂ£o das linhas da matriz*/
            no = (node **)malloc(linha * sizeof(node *));
            if (no == NULL)
            {
                printf("Memoria insuficiente.\n");
                exit(1);
            }
            for (i = 0; i < linha; i++)
            {
                no[i] = (node *)malloc(coluna * sizeof(node));
                if (no[i] == NULL)
                {
                    printf("Memoria insuficiente.\n");
                    exit(1);
                }
            }
            /*aloca_no(no, linha, coluna);*/
            fprintf(arqsaida, "PosiÃ§Ã£o inicial[x,y]: [%d,%d]\n", inicio.x, inicio.y);

            fprintf(arqsaida, "OrÃ§amento disponÃ­vel: %d segundos\n\n", orcamento);

            printf("InstÃ¢ncia %s\n", nome_instancia);
            printf("OrÃ§amento disponÃ­vel: %d segundos\n", orcamento);
            menorCaminho = (pontos *)malloc(((linha * coluna)) * sizeof(pontos));

            for (i = 0; i < saida; i++)
            {
                if (menorCaminho == NULL)
                {
                    printf("falha ao alocar memoria para lista do menor caminho");
                    exit(1);
                }
                iniciaNode(no, arena, linha, coluna);

                count = encontraMenorCaminho(no, menorCaminho, linha, coluna, inicio.x, inicio.y, saidas[i].x, saidas[i].y, i, arqsaida, orcamento);
                if (count != -1)
                {

                    saidas_alcancadas++;

                    if (menor_tempo > count)
                    {

                        menor_tempo = count;
                        menor_saida = i;

                        j = 0;
                        count--;
                        while (count != -1)
                        {
                            caminho[j] = no[menorCaminho[count].x][menorCaminho[count].y].direcao;
                            j++;
                            count--;
                        }
                    }
                }
            }

            free(menorCaminho);
            if (saidas_alcancadas > 0)
            {
                printf("Quantidade de saÃ­das alcanÃ§adas: %d\n", saidas_alcancadas);
                printf("SaÃ­da(s) mais prÃ³xima(s): %d segundo(s)\n\n", menor_tempo);
                printf("SaÃ­da %d:", menor_saida);
                for (j = 0; j < menor_tempo; j++)
                {
                    printf("%c", caminho[j]);
                }
                printf("\n\n");
                printf("Arquivo %s.out gerado com sucesso!\n", nome_instancia);
            }else{
                printf("\nNenhuma saída alcançada\n\n");
                printf("Arquivo %s.out gerado com sucesso!\n", nome_instancia);
            }

            fclose(arqsaida);
        }

        /*LiberaÃƒÂ§ao do espaÃƒÂ§o alocado*/
        libera_arena(arena, linha);
        liberaNo(no, linha);
    }
    else
    {
        printf("Exemplo de entrada .\\programa arquivo.dat");
    }
    return 0;
}