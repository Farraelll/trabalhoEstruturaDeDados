#include "bdpartidas.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

BDPartidas* bdpartidas_criar() {
    BDPartidas* bdp = (BDPartidas*) malloc(sizeof(BDPartidas));
    if (bdp) {
        bdp->inicio = NULL;
        bdp->num_partidas = 0;
    }
    return bdp;
}

void bdpartidas_liberar(BDPartidas* bdp) {
    NodePartida* atual = bdp->inicio;
    while (atual != NULL) {
        NodePartida* prox = atual->prox;
        free(atual);
        atual = prox;
    }
    free(bdp);
}

// Funcao auxiliar para pegar o maior ID e gerar o proximo
int gerar_id(BDPartidas* bdp) {
    int max = -1;
    NodePartida* atual = bdp->inicio;
    while (atual) {
        if (atual->partida.id > max) max = atual->partida.id;
        atual = atual->prox;
    }
    return max + 1;
}

void adicionar_na_lista(BDPartidas* bdp, Partida p) {
    NodePartida* novo = (NodePartida*) malloc(sizeof(NodePartida));
    novo->partida = p;
    novo->prox = NULL;

    if (bdp->inicio == NULL) {
        bdp->inicio = novo;
    } else {
        NodePartida* aux = bdp->inicio;
        while (aux->prox != NULL) aux = aux->prox;
        aux->prox = novo;
    }
    bdp->num_partidas++;
}

int bdpartidas_carregar(BDPartidas* bdp, const char* arquivo_csv) {
    FILE* f = fopen(arquivo_csv, "r");
    if (f == NULL) {
        // Se nao existe (vazio), apenas retorna sem erro critico para permitir insercoes
        if (strstr(arquivo_csv, "vazio") != NULL) return 0;
        return 1;
    }

    char buffer[100];
    int id, t1, t2, g1, g2;
    fgets(buffer, 100, f); 

    while (fgets(buffer, 100, f) != NULL) {
        if (sscanf(buffer, "%d,%d,%d,%d,%d", &id, &t1, &t2, &g1, &g2) == 5) {
            Partida p = {id, t1, t2, g1, g2};
            adicionar_na_lista(bdp, p);
        }
    }
    fclose(f);
    return 0;
}

void bdpartidas_inserir(BDPartidas* bdp, int t1, int t2, int g1, int g2) {
    Partida p;
    p.id = gerar_id(bdp);
    p.time1_id = t1;
    p.time2_id = t2;
    p.gols_time1 = g1;
    p.gols_time2 = g2;
    adicionar_na_lista(bdp, p);
}

int bdpartidas_remover(BDPartidas* bdp, int id) {
    NodePartida *atual = bdp->inicio, *ant = NULL;
    
    while (atual != NULL && atual->partida.id != id) {
        ant = atual;
        atual = atual->prox;
    }

    if (atual == NULL) return 0; // Nao achou

    if (ant == NULL) {
        bdp->inicio = atual->prox;
    } else {
        ant->prox = atual->prox;
    }
    free(atual);
    bdp->num_partidas--;
    return 1;
}

Partida* bdpartidas_buscar_por_id(BDPartidas* bdp, int id) {
    NodePartida* atual = bdp->inicio;
    while (atual != NULL) {
        if (atual->partida.id == id) return &atual->partida;
        atual = atual->prox;
    }
    return NULL;
}

int bdpartidas_atualizar(BDPartidas* bdp, int id, int g1, int g2) {
    Partida* p = bdpartidas_buscar_por_id(bdp, id);
    if (p) {
        p->gols_time1 = g1;
        p->gols_time2 = g2;
        return 1;
    }
    return 0;
}

NodePartida* bdpartidas_get_inicio(BDPartidas* bdp) {
    return bdp->inicio;
}