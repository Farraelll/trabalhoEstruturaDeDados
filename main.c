#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <strings.h>
#include "bdtimes.h"
#include "bdpartidas.h"

// Funcao auxiliar para recalcular tudo
void recalcular_geral(BDTimes* bdt, BDPartidas* bdp) {
    bdtimes_zerar_tudo(bdt); // Zera V, E, D, GM, GS de todos os times
    
    NodePartida* p_node = bdpartidas_get_inicio(bdp);
    while (p_node != NULL) {
        Partida p = p_node->partida;
        bdtimes_processar_partida(bdt, p.time1_id, p.gols_time1, p.time2_id, p.gols_time2);
        p_node = p_node->prox;
    }
}

void ler_string(char *str, int tamanho) {
    fgets(str, tamanho, stdin);
    size_t len = strlen(str);
    if (len > 0 && str[len - 1] == '\n') str[len - 1] = '\0';
}

void exibir_menu() {
    printf("\n--- Sistema de Gerenciamento de Partidas ---\n");
    printf("1 - Consultar time\n");
    printf("2 - Consultar partidas\n");
    printf("3 - Atualizar partida\n");
    printf("4 - Remover partida\n");
    printf("5 - Inserir partida\n");
    printf("6 - Imprimir tabela de classificacao\n");
    printf("Q - Sair\n");
    printf("Escolha uma opcao: ");
}

// Funcionalidades
void do_consultar_time(BDTimes *bdt) {
    char prefixo[50];
    printf("Digite o nome ou prefixo do time: ");
    ler_string(prefixo, 50);

    // Iteracao na lista de times
    printf("\n%-4s %-15s %-3s %-3s %-3s %-3s %-3s %-3s %-3s\n", 
           "ID", "Time", "V", "E", "D", "GM", "GS", "S", "PG");
    
    NodeTime* atual = bdt->inicio;
    int achou = 0;
    while(atual) {
        if (strncasecmp(atual->time.nome, prefixo, strlen(prefixo)) == 0) {
            time_imprimir_estatisticas(&atual->time);
            achou = 1;
        }
        atual = atual->prox;
    }
    if (!achou) printf("Nenhum time encontrado.\n");
}

void do_consultar_partidas(BDPartidas *bdp, BDTimes *bdt) {
    int modo;
    printf("1-Mandante, 2-Visitante, 3-Ambos: ");
    scanf("%d", &modo);
    getchar();

    char nome[50];
    printf("Digite o nome do time: ");
    ler_string(nome, 50);

    printf("\n%-4s %-15s %-15s %-7s %-7s\n", "ID", "Time1", "Time2", "Placar1", "Placar2");

    NodePartida* atual = bdpartidas_get_inicio(bdp);
    while(atual) {
        Time* t1 = bdtimes_buscar_por_id(bdt, atual->partida.time1_id);
        Time* t2 = bdtimes_buscar_por_id(bdt, atual->partida.time2_id);
        
        int match = 0;
        if (t1 && t2) {
            int m1 = (strncasecmp(t1->nome, nome, strlen(nome)) == 0);
            int m2 = (strncasecmp(t2->nome, nome, strlen(nome)) == 0);
            
            if (modo == 1 && m1) match = 1;
            else if (modo == 2 && m2) match = 1;
            else if (modo == 3 && (m1 || m2)) match = 1;
        }

        if (match) {
            printf("%-4d %-15s %-15s %-7d %-7d\n", 
                   atual->partida.id, t1->nome, t2->nome, 
                   atual->partida.gols_time1, atual->partida.gols_time2);
        }
        atual = atual->prox;
    }
}

void do_atualizar_partida(BDPartidas* bdp, BDTimes* bdt) {
    do_consultar_partidas(bdp, bdt); // Mostra para ajudar a escolher ID
    int id;
    printf("Digite o ID para atualizar: ");
    scanf("%d", &id);
    
    Partida* p = bdpartidas_buscar_por_id(bdp, id);
    if (!p) { printf("Nao encontrado.\n"); return; }
    
    printf("Placar Atual: %d x %d. Novos valores (g1 g2): ", p->gols_time1, p->gols_time2);
    int g1, g2;
    scanf("%d %d", &g1, &g2);
    
    char conf;
    printf("Confirma? (S/N): ");
    scanf(" %c", &conf);
    if (conf == 'S' || conf == 's') {
        bdpartidas_atualizar(bdp, id, g1, g2);
        recalcular_geral(bdt, bdp);
        printf("Atualizado!\n");
    }
}

void do_remover_partida(BDPartidas* bdp, BDTimes* bdt) {
    int id;
    printf("ID para remover: ");
    scanf("%d", &id);
    
    Partida* p = bdpartidas_buscar_por_id(bdp, id);
    if(p) {
         printf("Remover partida ID %d? (S/N): ", id);
         char conf;
         scanf(" %c", &conf);
         if (conf == 'S' || conf == 's') {
             bdpartidas_remover(bdp, id);
             recalcular_geral(bdt, bdp);
             printf("Removido!\n");
         }
    } else {
        printf("ID nao encontrado.\n");
    }
}

void do_inserir_partida(BDPartidas* bdp, BDTimes* bdt) {
    int t1, t2, g1, g2;
    printf("ID Time 1, ID Time 2, Gols 1, Gols 2: ");
    scanf("%d %d %d %d", &t1, &t2, &g1, &g2);
    
    // Valida IDs
    if (!bdtimes_buscar_por_id(bdt, t1) || !bdtimes_buscar_por_id(bdt, t2)) {
        printf("Times invalidos.\n");
        return;
    }

    printf("Confirma insercao? (S/N): ");
    char conf;
    scanf(" %c", &conf);
    if(conf == 'S' || conf == 's') {
        bdpartidas_inserir(bdp, t1, t2, g1, g2);
        recalcular_geral(bdt, bdp);
        printf("Inserido com sucesso!\n");
    }
}

void do_imprimir_classificacao(BDTimes* bdt) {
    bdtimes_ordenar_classificacao(bdt);
    printf("\n--- Classificacao ---\n");
    printf("%-4s %-15s %-3s %-3s %-3s %-3s %-3s %-3s %-3s\n", 
           "ID", "Time", "V", "E", "D", "GM", "GS", "S", "PG");
    bdtimes_imprimir_todos(bdt);
}

int main() {
    BDTimes *bdt = bdtimes_criar();
    BDPartidas *bdp = bdpartidas_criar();

    if (bdtimes_carregar(bdt, "times.csv") != 0) {
        printf("Erro carregando times.csv\n");
        return 1;
    }
    // Pode falhar se arquivo nao existir (cenario vazio), mas bdpartidas lida com isso
    bdpartidas_carregar(bdp, "partidas.csv");

    // Calculo inicial
    recalcular_geral(bdt, bdp);

    char opcao = ' ';
    do {
        exibir_menu();
        scanf(" %c", &opcao);
        getchar(); // Limpa buffer
        switch (toupper(opcao)) {
            case '1':
            do_consultar_time(bdt);
            break;
            case '2': 
            do_consultar_partidas(bdp, bdt);
            break;
            case '3':
            do_atualizar_partida(bdp, bdt);
            break;
            case '4':
            do_remover_partida(bdp, bdt);
            break;
            case '5':
            do_inserir_partida(bdp, bdt);
            break;
            case '6':
            do_imprimir_classificacao(bdt);
            break;
            case 'Q':
            printf("Saindo...\n");
            break;
            default:
            printf("Opcao invalida.\n");
        }
    } while (toupper(opcao) != 'Q');

    bdtimes_liberar(bdt);
    bdpartidas_liberar(bdp);
    return 0;
}