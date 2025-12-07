#ifndef BDPARTIDAS_H
#define BDPARTIDAS_H

#include "partida.h"

// No da lista encadeada de partidas
typedef struct NodePartida {
    Partida partida;
    struct NodePartida* prox;
} NodePartida;

typedef struct {
    NodePartida* inicio;
    int num_partidas;
} BDPartidas;

BDPartidas* bdpartidas_criar();
void bdpartidas_liberar(BDPartidas* bdp);
int bdpartidas_carregar(BDPartidas* bdp, const char* arquivo_csv);

// Novas funcoes para Parte II
void bdpartidas_inserir(BDPartidas* bdp, int t1, int t2, int g1, int g2);
int bdpartidas_remover(BDPartidas* bdp, int id);
Partida* bdpartidas_buscar_por_id(BDPartidas* bdp, int id);
int bdpartidas_atualizar(BDPartidas* bdp, int id, int g1, int g2);

// Iterador para o main (substitui o loop por indice)
NodePartida* bdpartidas_get_inicio(BDPartidas* bdp);

#endif