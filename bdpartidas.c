#include "bdpartidas.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

BDPartidas* bdpartidas_criar() {
    BDPartidas* bdp = (BDPartidas*) malloc(sizeof(BDPartidas));
    if (bdp != NULL) {
        bdp->num_partidas = 0;
    }
    return bdp;
}

void bdpartidas_liberar(BDPartidas* bdp) {
    free(bdp);
}

int bdpartidas_carregar(BDPartidas* bdp, const char* arquivo_csv) {
    FILE* f = fopen(arquivo_csv, "r");
    if (f == NULL) {
        // Campeonato Vazio
        if (strcmp(arquivo_csv, "partidas_vazio.csv") == 0) {
             printf("Aviso: Arquivo '%s' nao encontrado ou vazio.\n", arquivo_csv);
             bdp->num_partidas = 0;
             return 0;
        }
        perror("Erro ao abrir arquivo de partidas");
        return 1;
    }

    char buffer[100];
    int id, t1_id, t2_id, g1, g2;

    // Ignora cabecalho
    fgets(buffer, 100, f);

    // Le cada linha do arquivo
    while (bdp->num_partidas < MAX_PARTIDAS && 
           fgets(buffer, 100, f) != NULL) {
        
        // Pega as variaveis
        if (sscanf(buffer, "%d,%d,%d,%d,%d", &id, &t1_id, &t2_id, &g1, &g2) == 5) {
            Partida* p = &bdp->partidas[bdp->num_partidas];
            p->id = id;
            p->time1_id = t1_id;
            p->time2_id = t2_id;
            p->gols_time1 = g1;
            p->gols_time2 = g2;
            
            bdp->num_partidas++;
        }
    }

    fclose(f);
    return 0;
}

Partida* bdpartidas_buscar_por_indice(BDPartidas* bdp, int index) {
    if (index >= 0 && index < bdp->num_partidas) {
        return &bdp->partidas[index];
    }
    return NULL;
}

int bdpartidas_get_num_partidas(BDPartidas* bdp) {
    return bdp->num_partidas;
}