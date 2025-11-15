#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define HASH_SIZE 20

/* ============================
      ESTRUTURAS DO PROGRAMA
   ============================ */

/* Sala da mansão (árvore binária) */
typedef struct Sala {
    char nome[30];
    struct Sala *esq;
    struct Sala *dir;
} Sala;

/* Nó da BST de pistas */
typedef struct PistaNode {
    char pista[50];
    struct PistaNode *esq, *dir;
} PistaNode;

/* Estrutura da tabela hash (lista encadeada) */
typedef struct HashNode {
    char pista[50];
    char suspeito[30];
    struct HashNode *prox;
} HashNode;

/* Tabela hash */
HashNode* tabelaHash[HASH_SIZE];

/* ============================
      FUNÇÕES AUXILIARES
   ============================ */

/* Hash simples pela soma dos caracteres */
int hash(const char *str) {
    int soma = 0;
    for (int i = 0; str[i]; i++)
        soma += str[i];
    return soma % HASH_SIZE;
}

/* Cria uma sala dinamicamente */
Sala* criarSala(char nome[]) {
    Sala *nova = (Sala*) malloc(sizeof(Sala));
    strcpy(nova->nome, nome);
    nova->esq = nova->dir = NULL;
    return nova;
}

/* ============================
      FUNÇÕES DA TABELA HASH
   ============================ */

/* Insere pista → suspeito na tabela hash */
void inserirNaHash(char *pista, char *suspeito) {
    int index = hash(pista);

    HashNode *novo = (HashNode*) malloc(sizeof(HashNode));
    strcpy(novo->pista, pista);
    strcpy(novo->suspeito, suspeito);
    novo->prox = tabelaHash[index];
    tabelaHash[index] = novo;
}

/* Retorna suspeito associado à pista */
char* encontrarSuspeito(char *pista) {
    int index = hash(pista);
    HashNode *aux = tabelaHash[index];

    while (aux != NULL) {
        if (strcmp(aux->pista, pista) == 0)
            return aux->suspeito;
        aux = aux->prox;
    }
    return NULL;
}

/* ============================
      FUNÇÕES DA BST DAS PISTAS
   ============================ */

/* Insere pista na BST */
PistaNode* inserirPista(PistaNode* raiz, char *pista) {
    if (raiz == NULL) {
        PistaNode *novo = (PistaNode*) malloc(sizeof(PistaNode));
        strcpy(novo->pista, pista);
        novo->esq = novo->dir = NULL;
        return novo;
    }

    if (strcmp(pista, raiz->pista) < 0)
        raiz->esq = inserirPista(raiz->esq, pista);
    else if (strcmp(pista, raiz->pista) > 0)
        raiz->dir = inserirPista(raiz->dir, pista);

    return raiz;
}

/* Exibe pistas em ordem alfabética */
void listarPistas(PistaNode *raiz) {
    if (raiz == NULL) return;
    listarPistas(raiz->esq);
    printf("- %s\n", raiz->pista);
    listarPistas(raiz->dir);
}

/* Conta quantas pistas apontam para um suspeito */
int contarPistasPorSuspeito(PistaNode *raiz, char *suspeito) {
    if (!raiz) return 0;

    int count = contarPistasPorSuspeito(raiz->esq, suspeito);

    char *s = encontrarSuspeito(raiz->pista);
    if (s != NULL && strcmp(s, suspeito) == 0)
        count++;

    return count + contarPistasPorSuspeito(raiz->dir, suspeito);
}

/* ============================
   SISTEMA DE PISTAS POR SALA
   ============================ */

/* Retorna a pista associada a um cômodo */
char* pistaPorSala(char *sala) {
    if (strcmp(sala, "Hall") == 0) return "Chave dourada";
    if (strcmp(sala, "Biblioteca") == 0) return "Livro rasgado";
    if (strcmp(sala, "Escritorio") == 0) return "Documento adulterado";
    if (strcmp(sala, "Adega") == 0) return "Taca quebrada";
    if (strcmp(sala, "Cozinha") == 0) return "Faca suja";
    if (strcmp(sala, "Jardim") == 0) return "Pegadas recentes";
    if (strcmp(sala, "Porao") == 0) return "Luvas queimadas";
    return NULL;
}

/* Retorna suspeito associado à pista fixa */
char* suspeitoPorPista(char *pista) {
    if (strcmp(pista, "Chave dourada") == 0) return "Elisa";
    if (strcmp(pista, "Livro rasgado") == 0) return "Mortimer";
    if (strcmp(pista, "Documento adulterado") == 0) return "Clara";
    if (strcmp(pista, "Taca quebrada") == 0) return "Elisa";
    if (strcmp(pista, "Faca suja") == 0) return "Mortimer";
    if (strcmp(pista, "Pegadas recentes") == 0) return "Clara";
    if (strcmp(pista, "Luvas queimadas") == 0) return "Mortimer";
    return NULL;
}

/* ============================
      EXPLORAÇÃO DA MANSÃO
   ============================ */

/* Explora salas e coleta pistas */
void explorarSalas(Sala *atual, PistaNode **arvorePistas) {
    if (!atual) return;

    char opc;

    while (1) {
        printf("\nVocê está em: %s\n", atual->nome);

        /* Coletar pista da sala */
        char *pista = pistaPorSala(atual->nome);
        if (pista != NULL) {
            printf("Você encontrou a pista: %s\n", pista);

            /* Inserir pista na BST */
            *arvorePistas = inserirPista(*arvorePistas, pista);

            /* Inserir na hash */
            inserirNaHash(pista, suspeitoPorPista(pista));
        }

        printf("Mover para (e) esquerda, (d) direita ou (s) sair: ");
        scanf(" %c", &opc);

        if (opc == 'e') {
            if (atual->esq) atual = atual->esq;
            else printf("Não há sala à esquerda.\n");
        } 
        else if (opc == 'd') {
            if (atual->dir) atual = atual->dir;
            else printf("Não há sala à direita.\n");
        }
        else if (opc == 's') {
            printf("Fim da exploração.\n");
            break;
        }
        else {
            printf("Opção inválida.\n");
        }
    }
}

/* ============================
         JULGAMENTO FINAL
   ============================ */

void verificarSuspeitoFinal(PistaNode *arvorePistas) {
    char acusado[30];

    printf("\n=== JULGAMENTO FINAL ===\n");
    printf("Pistas coletadas:\n\n");
    listarPistas(arvorePistas);

    printf("\nQuem você acusa? ");
    scanf(" %s", acusado);

    int total = contarPistasPorSuspeito(arvorePistas, acusado);

    printf("\nTotal de pistas apontando para %s: %d\n", acusado, total);

    if (total >= 2)
        printf("\n✔ Acusação válida! %s é o verdadeiro culpado!\n", acusado);
    else
        printf("\n✘ Acusação insuficiente! %s é inocente...\n", acusado);
}

/* ============================
              MAIN
   ============================ */

int main() {
    /* Construção da mansão (árvore binária fixa) */
    Sala *hall = criarSala("Hall");
    Sala *biblioteca = criarSala("Biblioteca");
    Sala *cozinha = criarSala("Cozinha");
    Sala *adega = criarSala("Adega");
    Sala *escritorio = criarSala("Escritorio");
    Sala *jardim = criarSala("Jardim");
    Sala *porao = criarSala("Porao");

    hall->esq = biblioteca;
    hall->dir = cozinha;

    biblioteca->esq = adega;
    biblioteca->dir = escritorio;

    cozinha->esq = jardim;
    cozinha->dir = porao;

    /* Arvore de pistas vazia */
    PistaNode *arvorePistas = NULL;

    /* Exploração */
    explorarSalas(hall, &arvorePistas);

    /* Julgamento final */
    verificarSuspeitoFinal(arvorePistas);

    return 0;
}
