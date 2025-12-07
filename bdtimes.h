#ifndef BDTIMES_H
#define BDTIMES_H
#include "time.h"

// No da lista encadeada de times
typedef struct NodeTime {
    Time time;
    struct NodeTime* prox;
} NodeTime;

// TAD BDTimes agora gerencia a cabeca da lista
typedef struct {
    NodeTime* inicio;
    int num_times;
} BDTimes;

BDTimes* bdtimes_criar();
void bdtimes_liberar(BDTimes* bdt);
int bdtimes_carregar(BDTimes* bdt, const char* arquivo_csv);
void bdtimes_zerar_tudo(BDTimes* bdt);
void bdtimes_processar_partida(BDTimes* bdt, int id_time1, int gols_time1, int id_time2, int gols_time2);

Time* bdtimes_buscar_por_id(BDTimes* bdt, int id);
Time* bdtimes_buscar_por_nome(BDTimes* bdt, const char* prefixo); // Novo Helper

void bdtimes_ordenar_classificacao(BDTimes* bdt); // Para opcao 6
void bdtimes_imprimir_todos(BDTimes* bdt); // Helper para iteracao

#endif