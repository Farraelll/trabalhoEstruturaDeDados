#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <strings.h>
#include "bdtimes.h"
#include "bdpartidas.h"

// Prototipos das Funcoes de Menu

void exibir_menu();
void do_consultar_time(BDTimes *bdt);                      // Funcionalidade 1
void do_consultar_partidas(BDPartidas *bdp, BDTimes *bdt); // Funcionalidade 2
void do_imprimir_tabela(BDTimes *bdt);                     // Funcionalidade 6

void ler_string(char *str, int tamanho)
{
    fgets(str, tamanho, stdin);
    // Remove o '\n' do final
    size_t len = strlen(str);
    if (len > 0 && str[len - 1] == '\n')
    {
        str[len - 1] = '\0';
    }
    else
    {
        int c;
        while ((c = getchar()) != '\n' && c != EOF)
            ;
    }
}

// Funcao Principal

int main()
{
    // Cria os TADs
    BDTimes *bdt = bdtimes_criar();
    BDPartidas *bdp = bdpartidas_criar();
    if (bdt == NULL || bdp == NULL)
    {
        fprintf(stderr, "Erro critico: Falha ao alocar memoria.\n");
        return 1;
    }

    // Carrega dados dos arquivos CSV
    if (bdtimes_carregar(bdt, "times.csv") != 0)
    {
        fprintf(stderr, "Nao foi possivel carregar 'times.csv'. Encerrando.\n");
        return 1;
    }

    // Tenta carregar o cenario 3
    // Mudar as informações no arquivo "partidas.csv" para testar
    if (bdpartidas_carregar(bdp, "partidas.csv") != 0)
    {
        fprintf(stderr, "Nao foi possivel carregar 'partidas.csv'. Encerrando.\n");
        return 1;
    }

    // Processa estatisticas
    // Itera por todas as partidas carregadas e atualiza os times
    for (int i = 0; i < bdpartidas_get_num_partidas(bdp); i++)
    {
        Partida *p = bdpartidas_buscar_por_indice(bdp, i);
        bdtimes_processar_partida(bdt, p->time1_id, p->gols_time1, p->time2_id, p->gols_time2);
    }

    // Loop do menu
    char opcao = ' ';
    do
    {
        exibir_menu();
        char buffer[10];
        ler_string(buffer, 10);
        opcao = (buffer[0] != '\0') ? toupper(buffer[0]) : ' ';

        printf("\n");
        switch (opcao)
        {
        case '1':
            do_consultar_time(bdt);
            break;
        case '2':
            do_consultar_partidas(bdp, bdt);
            break;
        case '3':
        case '4':
        case '5':
            printf("Opcao desabilitada nesta versao (Parte I).\n");
            break;
        case '6':
            do_imprimir_tabela(bdt);
            break;
        case 'Q':
            printf("Encerrando o sistema...\n");
            break;
        default:
            printf("Opcao invalida. Tente novamente.\n");
            break;
        }
        printf("\n");

    } while (opcao != 'Q');

    // Libera a memoria
    bdtimes_liberar(bdt);
    bdpartidas_liberar(bdp);

    return 0;
}

// Implementacao das funcoes do menu

void exibir_menu()
{
    printf("--- Sistema de Gerenciamento de Partidas ---\n");
    printf("1 - Consultar time\n");
    printf("2 - Consultar partidas\n");
    printf("3 - Atualizar partida (Desabilitado)\n");
    printf("4 - Remover partida (Desabilitado)\n");
    printf("5 - Inserir partida (Desabilitado)\n");
    printf("6 - Imprimir tabela de classificacao\n");
    printf("Q - Sair\n");
    printf("Escolha uma opcao: ");
}

// Consulta time por prefixo
void do_consultar_time(BDTimes *bdt)
{
    char prefixo[50];
    printf("[Sistema]\n");
    printf("Digite o nome ou prefixo do time: "); // [cite: 136]
    ler_string(prefixo, 50);

    int encontrados = 0;

    // Imprime o cabecalho
    printf("\n[Sistema]\n");
    printf("%-2s %-12s %-3s %-3s %-3s %-3s %-3s %-3s %-3s\n",
           "ID", "Time", "V", "E", "D", "GM", "GS", "S", "PG");

    // Busca por prefixo
    for (int i = 0; i < bdtimes_get_num_times(bdt); i++)
    {
        Time *time = bdtimes_buscar_por_indice(bdt, i);
        if (strncasecmp(time->nome, prefixo, strlen(prefixo)) == 0)
        {
            time_imprimir_estatisticas(time);
            encontrados++;
        }
    }

    if (encontrados == 0)
    {
        printf("Nenhum time encontrado com o prefixo '%s'.\n", prefixo);
    }
}

// Consulta partidas por time
void do_consultar_partidas(BDPartidas *bdp, BDTimes *bdt)
{
    char buffer[10];
    int modo = 0;

    printf("[Sistema]\n");
    printf("Escolha o modo de consulta:\n");
    printf("1 - Por time mandante\n");
    printf("2 - Por time visitante\n");
    printf("3 - Por time mandante ou visitante\n");
    printf("4 - Retornar ao menu principal\n");
    printf("Opcao: ");

    ler_string(buffer, 10);
    sscanf(buffer, "%d", &modo);

    if (modo < 1 || modo > 3)
    {
        return;
    }

    char prefixo[50];
    printf("[Sistema]\n");
    printf("Digite o nome: ");
    ler_string(prefixo, 50);

    int encontrados = 0;

    printf("\n[Sistema]\n");
    printf("%-2s %-12s %5s %-12s\n", "ID", "Time1", "Placar", "Time2");

    // Passa por todas as partidas
    for (int i = 0; i < bdpartidas_get_num_partidas(bdp); i++)
    {
        Partida *p = bdpartidas_buscar_por_indice(bdp, i);
        Time *t1 = bdtimes_buscar_por_id(bdt, p->time1_id);
        Time *t2 = bdtimes_buscar_por_id(bdt, p->time2_id);

        int achou = 0;
        // Busca por prefixo
        int t1_match = (strncasecmp(t1->nome, prefixo, strlen(prefixo)) == 0);
        int t2_match = (strncasecmp(t2->nome, prefixo, strlen(prefixo)) == 0);

        switch (modo)
        {
        case 1: // Mandante
            if (t1_match)
                achou = 1;
            break;
        case 2: // Visitante
            if (t2_match)
                achou = 1;
            break;
        case 3: // Os dois
            if (t1_match || t2_match)
                achou = 1;
            break;
        }

        if (achou)
        {
            // Imprime no console
            printf("%-2d %-12s %d x %d %-12s\n",
                   p->id,
                   t1->nome,
                   p->gols_time1,
                   p->gols_time2,
                   t2->nome);
            encontrados++;
        }
    }

    if (encontrados == 0)
    {
        printf("Nenhuma partida encontrada para '%s'.\n", prefixo);
    }
}

// Imprime tabela de classificacao
void do_imprimir_tabela(BDTimes *bdt)
{
    printf("[Sistema]\n");
    printf("Imprimindo classificacao...\n\n");

    // Imprime cabecalho
    printf("%-2s %-12s %-3s %-3s %-3s %-3s %-3s %-3s %-3s\n",
           "ID", "Time", "V", "E", "D", "GM", "GS", "S", "PG");

    // Passa por todos os times
    for (int i = 0; i < bdtimes_get_num_times(bdt); i++)
    {
        Time *time = bdtimes_buscar_por_indice(bdt, i);
        time_imprimir_estatisticas(time);
    }
}
