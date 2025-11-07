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
void bdtimes_liberar(BDTimes* bdt);
int bdtimes_carregar(BDTimes* bdt, const char* arquivo_csv);
void bdtimes_processar_partida(BDTimes* bdt, int id_time1, int gols_time1, int id_time2, int gols_time2);
Time* bdtimes_buscar_por_id(BDTimes* bdt, int id);
Time* bdtimes_buscar_por_indice(BDTimes* bdt, int index);
int bdtimes_get_num_times(BDTimes* bdt);

#endif
