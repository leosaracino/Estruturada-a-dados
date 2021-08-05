#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define TAM_NOME 100

typedef struct Departamento {
    int cod_dept;
    int sala;
    char nome[TAM_NOME];
} TDepartamento;

typedef struct Funcionario {
    int cod_func;
    int cod_dept;
    char nome[TAM_NOME];
} TFuncionario;

TFuncionario *le_funcionario(FILE *in)
{
    TFuncionario *func = (TFuncionario *) malloc(sizeof(TFuncionario));
    char linha[150];
    if (fgets (linha, 150, in) == NULL ) {
        free(func);
        return NULL;
    }

    char delimitador[] = ";";
    char *ptr;
    int cod;

    //quebra a linha
    ptr = strtok(linha, delimitador);
    cod = atoi(ptr);
    func->cod_func = cod;
    ptr = strtok(NULL, delimitador);
    cod = atoi(ptr);
    func->cod_dept = cod;
    ptr = strtok(NULL, delimitador);
    strcpy(func->nome, ptr);

    return func;
}

TDepartamento *le_departamento(FILE *in)
{
    TDepartamento *dept = (TDepartamento *) malloc(sizeof(TDepartamento));
    char linha[150];
    if (fgets (linha, 150, in) == NULL ) {
        free(dept);
        return NULL;
    }

    char delimitador[] = ";";
    char *ptr;
    int cod, sala;

    //quebra a linha
    ptr = strtok(linha, delimitador);
    cod = atoi(ptr);
    dept->cod_dept = cod;
    ptr = strtok(NULL, delimitador);
    sala = atoi(ptr);
    dept->sala = sala;
    ptr = strtok(NULL, delimitador);
    strcpy(dept->nome, ptr);

    return dept;
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

void leftOuterJoin(char *nome_arq_dept, char *nome_arq_funcionarios, char *nome_arq_join) {
    FILE *arq_dept, *arq_func, *arq_join;
    arq_dept = fopen(nome_arq_dept, "r");
    arq_func = fopen(nome_arq_funcionarios, "r");
    arq_join = fopen(nome_arq_join, "w");
    int i = 0;
    if (arq_dept != NULL && arq_func != NULL && arq_join != NULL) {
        TFuncionario *func;
        TDepartamento *dept;
        while ((dept = le_departamento(arq_dept)) != NULL) {
            i = 0;
            fseek(arq_func, 0, SEEK_SET);
            while ((func = le_funcionario(arq_func)) != NULL) {
                if (func->cod_dept == dept->cod_dept) {
                    i = 1;
                    fprintf(arq_join, "%d;%d;%s;%d;%s;\n",func->cod_dept, dept->sala,dept->nome, func->cod_func, func->nome);
                }
            }
            if (i == 0) {
                fprintf(arq_join, "%d;%d;%s;%d;;\n",dept->cod_dept, dept->sala,dept->nome, 0);
            }
        }
        fclose(arq_dept);
        fclose(arq_func);
        fclose(arq_join);
    } else printf("Erro ao abrir arquivo\n");
}

int main() {
    leftOuterJoin("departamentos.txt", "funcionarios.txt", "join.txt");
    imprime_arquivo("join.txt");
}