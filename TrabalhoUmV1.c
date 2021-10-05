#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct
{
    int x, y;
} pontos;
typedef struct
{
    pontos coordenada;
    char direcao;
    int custo;
} caminho;
void empilha(caminho *pilha, int *topo, int l, int c, char direcao, int num_livre)
{
    if (*topo < num_livre)
    {
        pilha[*topo].coordenada.x = l;
        pilha[*topo].coordenada.y = c;
        pilha[*topo].direcao = direcao;
        if(*topo>0){
            pilha[*topo].custo = pilha[*topo - 1].custo + 1;
        }else{
            pilha[*topo].custo = 0;
        }
        
        *topo++;
    }
    else
        printf("Pilha cheia!");
}
caminho desempilha(caminho *pilha, int *topo)
{
    if (*topo > 0)
    {
        *topo--;
        return pilha[*topo];
    }
    else
        printf("Pilha vazia!");
}
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
int busca_visitado(caminho *visitado, int l, int c, int qtd){
    int i;
    while(i<qtd){
        if(l == visitado[i].coordenada.x && c == visitado[i].coordenada.y){
            return 1;
        }
        i++;
    }
    return 0;
}
void mapeia_campo_livre(char **arena, int linha, int coluna, pontos saida, caminho *livre, pontos inicio, caminho *visitado, int *topo1, int *topo2, int num_livre)
{
    int l = inicio.x, c = inicio.y,aux=0;
    caminho desempilhado;
    empilha(livre,topo1,inicio.x,inicio.y,' ',num_livre);
    while (l != saida.x && c != saida.y)
    {
        if (arena[l][c + 1] == ' ' && busca_visitado(visitado,l,c+1,aux)!=1)
        {
            c++;
            empilha(livre, topo1, l, c, 'd',num_livre);
        }
        else if (arena[l][c - 1] == ' ' && busca_visitado(visitado,l,c-1,aux)!=1)
        {
            c--;
            empilha(livre, topo1, l, c, 'e',num_livre);
        }else if (arena[l+1][c] == ' ' && busca_visitado(visitado,l+1,c,aux)!=1){
            l++;
            empilha(livre, topo1, l, c, 'b',num_livre);
        }else if (arena[l-1][c] == ' ' && busca_visitado(visitado,l-1,c,aux)!=1){
            l--;
            empilha(livre, topo1, l, c, 'c',num_livre);
        }else{
            visitado[aux] = desempilha(livre,topo1);
            aux++;
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
int main(int argc, char *argv[])
{
    char **arena, nome_instancia[50], dimensao[25];
    int saida, orcamento, linha, coluna, i, tempo[10], num_livre = 0, topo1, topo2;
    pontos saidas[10], inicio;
    caminho *livre, *visitado;
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
            visitado = (pontos *)malloc(num_livre * sizeof(pontos));
            calcula_saida(saidas, saida, &inicio, tempo);
            for (i = 0; i < saida; i++)
            {
                mapeia_campo_livre(arena, linha, coluna, saidas[i], livre, inicio, visitado, topo1, topo2, num_livre);
            }
            printf("inicio:%d %d\n", inicio.x, inicio.y);

            for (i = 0; i < num_livre; i++)
            {
                printf("Livre:%d %d \n", livre[i].coordenada.x, livre[i].coordenada.y);
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