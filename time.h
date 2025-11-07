#ifndef TIME_H
#define TIME_H

// Define a estrutura do time

typedef struct {
    int id;
    char nome[50];
    int v;
    int e;
    int d;
    int gm;
    int gs;
} Time;

// Prototipos das Funcoes
void time_inicializar(Time* time, int id, const char* nome);
void time_processar_partida(Time* time, int gols_feitos, int gols_sofridos);
int time_get_pg(Time* time);
int time_get_s(Time* time);
void time_imprimir_estatisticas(Time* time);

#endif
