#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct No {
    int m; //quantidade de chaves armazenadas no n�
    struct No *pai;
    int *s; //array de chaves
    struct No **p; //ponteiro para array de ponteiros para os filhos
} TNo;

typedef struct lista{
    int info;
    struct lista *prox;
} TLista;

TLista* insere_ordenado (TLista* li, int i) {
    TLista* novo;
    TLista* ant = NULL; /* ponteiro para elemento anterior */
    TLista* p = li; /* ponteiro para percorrer a lista */
    /* procura posi��o de inser��o */
    while (p != NULL && p->info < i) {
        ant = p;
        p = p->prox;
    }
    /* cria novo elemento */
    novo = (TLista*) malloc(sizeof(TLista));
    novo->info = i;
    /* encadeia elemento */
    if (ant == NULL) { /* insere elemento no in�cio */
        novo->prox = li; li = novo;
    }
    else { /* insere elemento no meio da lista */
        novo->prox = ant->prox;
        ant->prox = novo;
    }
    return li;
}

void imprime_lista(TLista *li) {
    TLista* p;
    for (p = li; p != NULL; p = p->prox)
        printf("%d ", p->info);
}

TNo *cria(int d) {
    TNo *novo = (TNo *) malloc(sizeof(TNo));
    novo->m = 0;
    novo->pai = NULL;
    novo->s = (int *) malloc(sizeof(int *) * (d * 2));
    novo->p = (TNo **) malloc(sizeof(TNo *) * (d * 2) + 1);
    for (int i = 0; i < (d * 2 + 1); i++) {
        novo->p[i] = NULL;
    }
    return novo;
}

TNo *libera(TNo *a, int d) {
    if (a != NULL) {
        for (int i = 0; i <= d * 2 + 1; i++) {
            libera(a->p[i], d);
        }
        free(a->s);
        free(a->p);
        free(a);
    }
    return NULL;
}

/*
 * A impressao da �rvore � feita como se ela estivesse na vertical ao inv�s da horizontal, portanto cada n� � impresso na vertical,
 * com a menor chave do n� na parte inferior do n�
 */
void imprime_arvore(TNo *a, int nivel) {
    if (a != NULL) {
        char indent[10] = "";
        for (int i = 0; i < nivel; i++) {
            strcat(indent, "\t");
        }

        for (int i = a->m; i > 0; i--) {
            imprime_arvore(a->p[i], nivel + 1);
            printf("%s", indent);
            printf("%5d\n", a->s[i - 1]);
        }
        imprime_arvore(a->p[0], nivel + 1);
    }
}

void imprime_no(TNo *a) {
    if (a != NULL) {
        for (int i = 0; i < a->m; i++) {
            printf("%5d", a->s[i]);
        }
        printf("\n");
    }
}

/*
 * Busca bin�ria da posi��o em que a chave deveria estar dentro do n�.
 */
int posicao(int chave, TNo *no) {
    int inicio = 0;
    int fim = no->m;
    int pos = (fim + inicio) / 2;
    while (pos != no->m && chave != no->s[pos] && inicio < fim) {
        if (chave > no->s[pos]) {
            inicio = pos + 1;
        } else {
            fim = pos;
        }
        pos = (fim + inicio) / 2;
    }
    return pos;
}

/*
 * Busca retorna ponteiro para o n� onde a chave est�, ou onde ela deveria estar (se chegar numa folha e n�o encontrar a chave)
 */
TNo *busca(TNo *no, int chave) {
    int pos = posicao(chave, no);
    if (no->p[pos] == NULL || (pos < no->m && chave == no->s[pos])) {
        return no;
    } else {
        return busca(no->p[pos], chave);
    }
}

void particiona(TNo *P, int d, int pos, int chave, TNo *pt);

/*
 * Insere a chave e o seu repectivo ponteiro da direita (para Q) numa posi��o espec�fica de um n� espec�fico da �rvore
 * d � a ordem da arvore
 * Caso a posi��o n�o seja informada (-1) o algoritmo busca pela posi��o correta
 * Retorna ponteiro para a raiz da �rvore
 */
void insere(TNo *no, int d, int pos, int chave, TNo *pt) {
    if (pos == -1) {
        pos = posicao(chave, no);
    }

    if (no->m < 2 * d) { // Tem espa�o no n�, ent�o pode inserir diretamente
        for (int i = no->m; i > pos; i--) { // Abre espa�o para a inser��o
            no->s[i] = no->s[i - 1];
            no->p[i + 1] = no->p[i];
        }
        no->s[pos] = chave;
        no->p[pos + 1] = pt;
        if (pt != NULL) {
            pt->pai = no;
        }
        no->m++;
    } else { // N� est� cheio -- � necess�rio particionar
        particiona(no, d, pos, chave, pt);
    }
}

/*
 * Insere a chave numa folha da �rvore
 * raiz � ponteiro para a raiz da �rvore
 * d � a ordem da arvore
 * Retorna ponteiro para a raiz da �rvore
 */
TNo *insere_folha(TNo *raiz, int d, int chave) {
    if (raiz != NULL) {
        TNo *no = busca(raiz, chave);
        int pos = posicao(chave, no);
        if (pos == no->m || chave != no->s[pos]) { // chave n�o existe no n�
            insere(no, d, pos, chave, NULL);
            if (raiz->pai != NULL) { // Inser��o riou uma nova raiz
                raiz = raiz->pai;
            }
        }
    } else {
        raiz = cria(d);
        raiz->s[0] = chave;
        raiz->m = 1;
    }
    return raiz;
}

/*
 * Particiona o n� P para adicionar a chave e o ponteiro pt
 * Ap�s o final da execu��o dessa fun��o, a chave ter� sido inserida no local correto
 * e um novo n� Q ter� sido criado como resultado do particionamento
 * Uma nova raiz W pode ser criada, se a raiz atual estiver sendo particionada
 */
void particiona(TNo *P, int d, int pos, int chave, TNo *pt) {
    // Prepara Q
    TNo *Q = cria(d);
    int i = d * 2 - 1;
    int j = d - 1;
    while (j >= 0) {
        if (j != pos - d - 1) { // Move chave de P para Q
            Q->s[j] = P->s[i];
            Q->p[j + 1] = P->p[i + 1];
            i--;
        } else { // Insere chave em Q
            Q->s[j] = chave;
            Q->p[j + 1] = pt;
        }
        if (Q->p[j + 1] != NULL) {
            Q->p[j + 1]->pai = Q;
        }
        j--;
    }
    Q->m = d;

    // Processa P
    if (pos <= d) {
        P->m = d;
        insere(P, d, pos, chave, pt); // inser��o vai fazer o n�mero de chaves aumentar para d+1, e isso � tratado no final dessa fun��o
    }

    // Processa W
    if (P->pai == NULL) { // Caso o n� seja raiz, cria o n� superior
        TNo *W = cria(d);
        W->p[0] = P;
        P->pai = W;
    }
    // Chave d de P sobe para o pai W
    insere(P->pai, d, -1, P->s[d], Q);
    // Ajusta o ponteiro da esquerda de Q (recebe ponteiro da direita da chave que subiu para o pai W)
    Q->p[0] = P->p[d+1];
    // A inser��o da chave d em W fez o n�mero de chaves de P diminuir para D
    P->m = d;
}

TLista *recursiva(TNo *raiz, int k, TLista *lista) {
    if (raiz != NULL) {
        int i;
        for (i = 0; i < raiz->m; i++) {
            if (raiz->s[i] < k) {  
                lista = insere_ordenado(lista, raiz->s[i]);
            }   
             lista = recursiva(raiz->p[i], k, lista);
        }
        lista = recursiva(raiz->p[i], k, lista);
    }
    return lista;
}

TLista *menor_k(TNo *raiz, int k) {
    TLista *lista = NULL;
    lista = recursiva(raiz, k, lista);
    return lista;
}

int main(int argc, char *argv[]) {
    TNo *raiz = NULL;

    int d;
    int k;
    //le ordem da arvore
    scanf("%d", &d);

    char l[1000];
    char delimitador[] = "-";
    char *ptr;
    int valor;

    /* l� valores para criar a arvore
     * valores devem ser informados separados por tra�o
     * exemplo: 1-3-5-2-7-9-21-6 */
    scanf("%s", l);
    //quebra a string de entrada
    ptr = strtok(l, delimitador);
    while(ptr != NULL) {
        valor = atoi(ptr);
        //printf("\n\nInserindo %d\n", valor);
        raiz = insere_folha(raiz, d, valor);
        //imprime_arvore(raiz, 0);
        ptr = strtok(NULL, delimitador);
    }
    scanf("%d", &k);
    //imprime_arvore(raiz, 0);

    TLista *lista = menor_k(raiz, k);
    imprime_lista(lista);
}