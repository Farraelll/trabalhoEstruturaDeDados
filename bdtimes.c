#include "bdtimes.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

BDTimes* bdtimes_criar() {
    BDTimes* bdt = (BDTimes*) malloc(sizeof(BDTimes));
    if (bdt != NULL) {
        bdt->inicio = NULL;
        bdt->num_times = 0;
    }
    return bdt;
}

void bdtimes_liberar(BDTimes* bdt) {
    NodeTime* atual = bdt->inicio;
    while (atual != NULL) {
        NodeTime* prox = atual->prox;
        free(atual);
        atual = prox;
    }
    free(bdt);
}

int bdtimes_carregar(BDTimes* bdt, const char* arquivo_csv) {
    FILE* f = fopen(arquivo_csv, "r");
    if (f == NULL) return 1;

    char buffer[100];
    int id;
    char nome[50];

    fgets(buffer, 100, f); // Ignora header

    // Leitura e insercao na lista
    while (fgets(buffer, 100, f) != NULL) {
        if (sscanf(buffer, "%d,%49[^\n]", &id, nome) == 2) {
            // Remove \r se existir
            nome[strcspn(nome, "\r")] = 0;

            NodeTime* novo = (NodeTime*) malloc(sizeof(NodeTime));
            time_inicializar(&novo->time, id, nome);
            novo->prox = NULL;

            // Insere no final para manter ordem de leitura
            if (bdt->inicio == NULL) {
                bdt->inicio = novo;
            } else {
                NodeTime* aux = bdt->inicio;
                while (aux->prox != NULL) aux = aux->prox;
                aux->prox = novo;
            }
            bdt->num_times++;
        }
    }
    fclose(f);
    return 0;
}

void bdtimes_zerar_tudo(BDTimes* bdt) {
    NodeTime* atual = bdt->inicio;
    while (atual != NULL) {
        time_zerar(&atual->time);
        atual = atual->prox;
    }
}

void bdtimes_processar_partida(BDTimes* bdt, int id_time1, int gols_time1, int id_time2, int gols_time2) {
    Time* t1 = bdtimes_buscar_por_id(bdt, id_time1);
    Time* t2 = bdtimes_buscar_por_id(bdt, id_time2);
    if (t1 && t2) {
        time_processar_partida(t1, gols_time1, gols_time2);
        time_processar_partida(t2, gols_time2, gols_time1);
    }
}

Time* bdtimes_buscar_por_id(BDTimes* bdt, int id) {
    NodeTime* atual = bdt->inicio;
    while (atual != NULL) {
        if (atual->time.id == id) return &atual->time;
        atual = atual->prox;
    }
    return NULL;
}

// Troca os dados de dois nos
void swap_times(NodeTime* a, NodeTime* b) {
    Time temp = a->time;
    a->time = b->time;
    b->time = temp;
}

void bdtimes_ordenar_classificacao(BDTimes* bdt) {
    if (bdt->inicio == NULL) return;
    
    int trocou;
    NodeTime *ptr1;
    NodeTime *lptr = NULL;

    do {
        trocou = 0;
        ptr1 = bdt->inicio;

        while (ptr1->prox != lptr) {
            Time* t1 = &ptr1->time;
            Time* t2 = &ptr1->prox->time;

            int pg1 = time_get_pg(t1);
            int pg2 = time_get_pg(t2);
            int s1 = time_get_s(t1);
            int s2 = time_get_s(t2);

            // Criterios: PG desc, V desc, Saldo desc
            int deve_trocar = 0;
            if (pg1 < pg2) deve_trocar = 1;
            else if (pg1 == pg2) {
                if (t1->v < t2->v) deve_trocar = 1;
                else if (t1->v == t2->v) {
                    if (s1 < s2) deve_trocar = 1;
                }
            }

            if (deve_trocar) {
                swap_times(ptr1, ptr1->prox);
                trocou = 1;
            }
            ptr1 = ptr1->prox;
        }
        lptr = ptr1;
    } while (trocou);
}

void bdtimes_imprimir_todos(BDTimes* bdt) {
    NodeTime* atual = bdt->inicio;
    while (atual != NULL) {
        time_imprimir_estatisticas(&atual->time);
        atual = atual->prox;
    }
}