#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include "nomes.c"
#include "cliente.c"

#include <limits.h>
#include <dirent.h>
#include <string.h>

typedef struct vetor {
    TCliente *cli;
    FILE *f;
} TVet;

typedef struct vetorNomes {
    char *nome;
} TVetNomes;

TNomes *gera_nomes_particoes(int num){
    TVetNomes nomes[num];
    TNomes* lista_nomes_particoes = NULL;
    for (int i = num-1; i >= 0; i--) {
        nomes[i].nome = (char *) malloc(sizeof(char[10]));
        char num_particao[10];
        strcpy(nomes[i].nome, "p");
        sprintf(num_particao, "%d", i+1);
        strcat(nomes[i].nome, num_particao);
        strcat(nomes[i].nome, ".txt");
        lista_nomes_particoes = insere_inicio(lista_nomes_particoes, nomes[i].nome);
    }
    return lista_nomes_particoes;
}

char *gera_nome_particao(int num_part) {
    char *nome_particao = (char *) malloc(sizeof(char[10]));
    char num_particao[10];

    strcpy(nome_particao, "p");
    sprintf(num_particao, "%d", num_part);
    strcat(nome_particao, num_particao);
    strcat(nome_particao, ".txt");

    return nome_particao;
}

void imprime_arquivo(char *name) {
    FILE *arq; //declara ponteiro para arquivo
    // abre arquivo para leitura
    arq = fopen(name, "r");
    if (arq != NULL) {// checa se não deu erro na abertura do arquivo
        char linha[150];
        fgets (linha, 150, arq);
        while (!feof(arq)) {//testa se chegou ao final do arquivo
            printf("%s", linha);
            fgets (linha, 150, arq);
        }
        fclose(arq); //fecha arquivo
    } else printf("Erro ao abrir arquivo\n");
}

typedef struct vetaux{
    TVet *info;
    struct vetaux *prox;
} Tvetaux;

int intercalacao_otima(char *nome_arquivo_saida, int num_p, TNomes *nome_particoes, int f) {
    int num_particoes = num_p;
    while (num_p > 1) {
        Tvetaux *vetaux = NULL;
        for (int i = 0; i < (f - 1); i++) {
            if(num_particoes < f - 1 && num_particoes > 1) {
                while (){
                vetaux->info->f = fopen(nome_particoes[i].nome, "r");
                vetaux->info->cli = ler_cliente(vetaux->info->f);
              }
            }
            else{
                vet[i].f = fopen(nome_particoes[i].nome, "r");
                vet[i].cli = ler_cliente(vet[i].f);
                }
        }
    }

    return 0;
}

int main() {
    int num_particoes;
    int f;
    int num_particoes_intermediarias;
    TNomes *nomes_particoes = NULL;
    //le numero de particoes a serem intercaladas
    scanf("%d", &num_particoes);
    nomes_particoes = gera_nomes_particoes(num_particoes);
    //le f (número total de arquivos a serem manipulados ao mesmo tempo)
    //lembrar que o método usa f-1 arquivos para leitura, e 1 arquivo para saída
    scanf("%d", &f);
    num_particoes_intermediarias = intercalacao_otima("saida.txt", num_particoes, nomes_particoes, f);
    printf("%d\n", num_particoes_intermediarias);
    printf("*** saida.txt\n");
    if (num_particoes_intermediarias > 0)
        imprime_arquivo("saida.txt");
}