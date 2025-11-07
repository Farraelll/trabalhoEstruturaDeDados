#ifndef BDTIMES_H
#define BDTIMES_H
#include "time.h"

#define NUM_TIMES 10

// Estrutura para as informacoes dos times
typedef struct {
    Time times[NUM_TIMES];
    int num_times;
} BDTimes;

// Prototipos das Funcoes


BDTimes* bdtimes_criar();

// Libera a memoria alocada para o BDTimes
void bdtimes_liberar(BDTimes* bdt);

// Carrega os times do arquivo times.csv
int bdtimes_carregar(BDTimes* bdt, const char* arquivo_csv);

// Processa uma partida, atualizando as estatisticas dos dois times envolvidos
void bdtimes_processar_partida(BDTimes* bdt, int id_time1, int gols_time1, int id_time2, int gols_time2);

// Retorna um ponteiro para um time, dado seu ID
// Retorna NULL se nao encontrado

Time* bdtimes_buscar_por_id(BDTimes* bdt, int id);

// Retorna um ponteiro para um time, usando o indice
Time* bdtimes_buscar_por_indice(BDTimes* bdt, int index);

// Retorna o numero de times
int bdtimes_get_num_times(BDTimes* bdt);

#endif