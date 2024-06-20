#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/*Trabalho desenvolvido pelos alunos:
Vinicius Silva de Paula, RGA:2017.0743.043-8
Julio Lossavaro, RGA:2017.0743.029-4*/

struct cel
{
    int frequencia;
    char palavra[100];
    struct cel *prox;
};

typedef struct cel celula;

/*Função para alocar cada celula do nó, nela é feito o malloc na celula, apos isso é verificado
se a celula foi realmente alocada e por fim coloca a palavra em seu devido lugar, faz a frequencia
ter 1 e o novo->prox aponta pra nulo - O(1).*/
celula *aloca(char palavra[])
{
    celula *novo;
    novo = (celula *)malloc(sizeof(celula));
    if (novo == NULL)
    {
        fprintf(stderr, "Erro ao alocar memória!\n");
        exit(1);
    }
    strcpy(novo->palavra, palavra);
    novo->frequencia = 1;
    novo->prox = NULL;
    return novo;
}

/*Metodo utilizado para inserção de elementos nas listas, nesse metodo é verificação se a lista ta vazia,
se estiver ele insere no novo elemento na lista, caso não esteja vazia, ele percorre a lista até achar a
posição que o novo elemento será inserido, após isso ele verifica se o a posição da lista é menor, maior
ou igual em ordem lexicografica, quando for maior ele coloca no fim da lista, quando for menor
ele faz o anterior apontar para o novo e o novo aponta para a posição atual da lista - O(n).*/
void insere(celula **lst, char palavra[])
{
    celula *pt, *novo, *anterior = NULL;
    novo = aloca(palavra);
    if (*lst == NULL)
    {
        *lst = novo;
    }
    else
    {
        pt = *lst;
        while (pt->prox != NULL && (strcmp(novo->palavra, pt->palavra) > 0))
        {
            anterior = pt;
            pt = pt->prox;
        }
        if (strcmp(novo->palavra, pt->palavra) == 0)
        {
            pt->frequencia = pt->frequencia + 1;

            free(novo);
        }
        else if (strcmp(novo->palavra, pt->palavra) > 0)
        {
            pt->prox = novo;
        }
        else
        {
            novo->prox = pt;
            if (anterior != NULL)
            {
                anterior->prox = novo;
            }
            else
            {
                *lst = novo;
            }
        }
    }
}

/*Metodo de leitura de arquivo - neste metodo é lido palavra e etiquetas usando o fscanf, 
apos isso é utilizada a função strtok para separar etiqueta de palavra e os dois são salvos
em uma matriz de 2 por 100, é verificado com strtok tambem os caracteres que não podem ser 
contabilizados pelo programa, para isso é utilizado um indice que verificar se entrou no 
laço do strtok pois se aquele caractere estiver sozinho logo no primeiro token o ponteiro
será nulo,  quando isso acontecer significa que devera ser ignorada a palavra e sua tag 
- O(n.m)*/
void le_arquivo(FILE *arq, celula **lista1, celula **lista2)
{
    char palavra[100], *pt, mat[2][100];
    int i;

    while (!feof(arq))
    {
        i = 0;
        fscanf(arq, "%s", palavra);
        pt = strtok(palavra, " /");
        while (pt)
        {
            strcpy(mat[i], pt);
            pt = strtok(NULL, " /");
            i++;
        }
        strcpy(palavra, mat[0]);
        i = 0;
        pt = strtok(mat[0], ".,\'\"$!?:)(#");
        while (pt)
        {
            pt = strtok(NULL, ".,\'\"$!?:)(#");
            i++;
        }
        if (i >= 1)
        {
                for (i = 0; i < strlen(palavra); i++)
                {
                    palavra[i] = tolower(palavra[i]);
                }
                insere(lista1, palavra);
                strcpy(palavra, mat[1]);
                insere(lista2, palavra);
        }
    }

    fclose(arq);
}
/*Metodo que percorre as listas e libera as celulas da lista - O(n).*/
void libera_lista(celula **lst)
{
    celula *anterior;
    while (*lst != NULL)
    {
        anterior = *lst;
        *lst = (*lst)->prox;
        free(anterior);
    }
}

/*Metodo para percorrer a lista e printar os elementos, nela é verificado se a posição na lista é nula,
enquanto não for continua percorrendo, caso a proxima posição for nula ele não pula a linha - O(n).*/
void mostra_lista(FILE *arqsaida, celula *lista)
{
    celula *pt;
    pt = lista;
    while (pt != NULL)
    {
        fprintf(arqsaida, "%s, %d", pt->palavra, pt->frequencia);
        if (pt->prox != NULL)
        {
            fprintf(arqsaida, "\n");
        }
        pt = pt->prox;
    }
}
/*Metodo que percorre a lista de palavras contando o total de palavras que é o valor total de todas
as frequencias de palavras e o total de palavras distintas que é o total de palavras inseridas na lista -
O(n).*/
void conta_palavras(celula *lista, int *conta_palavra, int *conta_palavra_sem_repeticoes)
{
    celula *pt;
    pt = lista;
    while (pt != NULL)
    {
        *conta_palavra = (*conta_palavra) + pt->frequencia;
        (*conta_palavra_sem_repeticoes)++;
        pt = pt->prox;
    }
}
/*Metodo para gerar o arquivo de saida do algoritmo - O(2n+m).*/
void salva_arquivo(FILE *arqsaida, celula *lista_palavras, celula *lista_tags)
{
    int conta_palavra = 0, conta_palavra_sem_repeticoes = 0;
    conta_palavras(lista_palavras, &conta_palavra, &conta_palavra_sem_repeticoes);
    fprintf(arqsaida, "TOTAL DE PALAVRAS, %d\n", conta_palavra);
    fprintf(arqsaida, "TOTAL DE PALAVRAS DISTINTAS, %d\n\n", conta_palavra_sem_repeticoes);
    fprintf(arqsaida, "PoS, FREQ\n");
    mostra_lista(arqsaida, lista_tags);
    fprintf(arqsaida, "\n\nPALAVRA, FREQ\n");
    mostra_lista(arqsaida, lista_palavras);
    fclose(arqsaida);
}

int main(int argc, char *argv[])
{
    celula *lista_palavras = NULL, *lista_tags = NULL;

    char *nomearq;
    FILE *arqentrada, *arqsaida;
    if (argc == 2)
    {
        arqentrada = fopen(argv[1], "r");
        if (arqentrada != NULL)
        {
            nomearq = strtok(argv[1], ".");
            strcat(nomearq, ".csv");
            le_arquivo(arqentrada, &lista_palavras, &lista_tags);
            arqsaida = fopen(nomearq, "w");
            salva_arquivo(arqsaida, lista_palavras, lista_tags);
            libera_lista(&lista_palavras);
            libera_lista(&lista_tags);
        }
        else
        {
            printf("Arquivo não encontrado\n");
        }
    }
    else
    {
        printf("Modo de uso: ./<nome_prog> <nome_arquivo.pos>");
    }
    return 0;
} 