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

// Libera a memoria alocada para o BDPartidas
void bdpartidas_liberar(BDPartidas* bdp);

// Carrega as partidas do arquivo partidas.csv 
// Retorna 0 em sucesso, 1 em falha.

int bdpartidas_carregar(BDPartidas* bdp, const char* arquivo_csv);

// Retorna um ponteiro para uma partida, usando o indice
Partida* bdpartidas_buscar_por_indice(BDPartidas* bdp, int index);

// Retorna o numero total de partidas carregadas
int bdpartidas_get_num_partidas(BDPartidas* bdp);

#endif