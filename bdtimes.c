#include "bdtimes.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Inicializa a lista de times
BDTimes* bdtimes_criar() {
    BDTimes* bdt = (BDTimes*) malloc(sizeof(BDTimes));
    if (bdt != NULL) {
        bdt->num_times = 0;
    }
    return bdt;
}

// Libera a lista de times
void bdtimes_liberar(BDTimes* bdt) {
    free(bdt);
}

// Carrega os Times
int bdtimes_carregar(BDTimes* bdt, const char* arquivo_csv) {
    FILE* f = fopen(arquivo_csv, "r");
    if (f == NULL) {
        perror("Erro ao abrir times.csv");
        return 1;
    }

    char buffer[100];
    int id;
    char nome[50];

    // Ignora cabecalho
    fgets(buffer, 100, f); 

    // Le cada linha do arquivo
    while (fgets(buffer, 100, f) != NULL) {
        if (sscanf(buffer, "%d,%49[^\n]", &id, nome) == 2) {
            if (id >= 0 && id < NUM_TIMES) {
                time_inicializar(&bdt->times[id], id, nome);
                bdt->num_times++;
            }
        }
    }

    fclose(f);
    
    // Valida se carregou o numero esperado de times
    if (bdt->num_times != NUM_TIMES) {
        fprintf(stderr, "Erro: Esperava %d times, mas carregou %d\n", NUM_TIMES, bdt->num_times);
        return 1;
    }
    
    return 0;
}

// Processa e carrega as estatisticas
void bdtimes_processar_partida(BDTimes* bdt, int id_time1, int gols_time1, int id_time2, int gols_time2) {
    // Busca os times pelo ID
    Time* time1 = bdtimes_buscar_por_id(bdt, id_time1);
    Time* time2 = bdtimes_buscar_por_id(bdt, id_time2);

    if (time1 != NULL && time2 != NULL) {
        // Atualiza estatisticas de cada time
        time_processar_partida(time1, gols_time1, gols_time2);
        time_processar_partida(time2, gols_time2, gols_time1);
    }
}

// Busca Times por Id
Time* bdtimes_buscar_por_id(BDTimes* bdt, int id) {
    if (id >= 0 && id < bdt->num_times) {
        return &bdt->times[id];
    }
    return NULL;
}

// Busca times por indice
Time* bdtimes_buscar_por_indice(BDTimes* bdt, int index) {
    if (index >= 0 && index < bdt->num_times) {
        return &bdt->times[index];
    }
    return NULL;
}

// Retorna o numero de times
int bdtimes_get_num_times(BDTimes* bdt) {
    return bdt->num_times;
}