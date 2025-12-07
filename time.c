#include "time.h"
#include <stdio.h>
#include <string.h>

void time_inicializar(Time* time, int id, const char* nome) {
    time->id = id;
    strncpy(time->nome, nome, 50);
    time->nome[49] = '\0';
    time_zerar(time);
}

void time_zerar(Time* time) {
    time->v = 0;
    time->e = 0;
    time->d = 0;
    time->gm = 0;
    time->gs = 0;
}

void time_processar_partida(Time* time, int gols_feitos, int gols_sofridos) {
    if (gols_feitos > gols_sofridos) {
        time->v++;
    } else if (gols_feitos == gols_sofridos) {
        time->e++;
    } else {
        time->d++;
    }
    time->gm += gols_feitos;
    time->gs += gols_sofridos;
}

int time_get_pg(Time* time) {
    return (3 * time->v) + time->e;
}

int time_get_s(Time* time) {
    return time->gm - time->gs;
}

void time_imprimir_estatisticas(Time* time) {
    printf("%-2d %-12s %-3d %-3d %-3d %-3d %-3d %-3d %-3d\n",
           time->id, time->nome, time->v, time->e, time->d,
           time->gm, time->gs, time_get_s(time), time_get_pg(time));
}