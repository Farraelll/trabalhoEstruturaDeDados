#ifndef BDPARTIDAS_H
#define BDPARTIDAS_H

#include "partida.h"

// Tamanho pre-definido para o vetor
#define MAX_PARTIDAS 100

// Estrutura para informacoes sobre as partidas
typedef struct {
    Partida partidas[MAX_PARTIDAS];
    int num_partidas;
} BDPartidas;

// Prototipos das Funcoes
BDPartidas* bdpartidas_criar();
void bdpartidas_liberar(BDPartidas* bdp);
int bdpartidas_carregar(BDPartidas* bdp, const char* arquivo_csv);
Partida* bdpartidas_buscar_por_indice(BDPartidas* bdp, int index);
int bdpartidas_get_num_partidas(BDPartidas* bdp);

#endif
