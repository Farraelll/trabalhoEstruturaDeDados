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

// Declara time_inicializar
void time_inicializar(Time* time, int id, const char* nome);

// Declara time_processar_partida
void time_processar_partida(Time* time, int gols_feitos, int gols_sofridos);

// Declara time_get_pg
int time_get_pg(Time* time);

// Declara time_get_s
int time_get_s(Time* time);

// Declara time_imprimir_estatisticas
void time_imprimir_estatisticas(Time* time);

#endif