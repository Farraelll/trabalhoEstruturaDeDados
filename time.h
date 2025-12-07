#ifndef TIME_H
#define TIME_H

typedef struct {
    int id;
    char nome[50];
    int v, e, d;
    int gm, gs;
} Time;

void time_inicializar(Time* time, int id, const char* nome);
void time_zerar(Time* time); // Nova função
void time_processar_partida(Time* time, int gols_feitos, int gols_sofridos);
int time_get_pg(Time* time);
int time_get_s(Time* time);
void time_imprimir_estatisticas(Time* time);

#endif