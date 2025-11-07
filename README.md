# Trabalho de Programação - Campeonato Computacional de Futebol

Este projeto é a implementação da Parte I do trabalho de programação, focado em um sistema simplificado em linguagem C para gerenciamento de dados de um campeonato de futebol[cite: 9].

## Como Executar

O projeto foi desenvolvido e testado em ambiente Linux (Ubuntu 22.04 com GCC 11)[cite: 215]. É necessário ter o `gcc` e o `make` instalados.

1.  **Arquivos de Dados:** Certifique-se de que os arquivos `times.csv` e `partidas.csv` estejam no mesmo diretório dos arquivos-fonte.
    * `times.csv` (Deve conter os 10 times ).
    * `partidas.csv` (Use o conteúdo de `partidas_completo.csv`  para o Cenário 3, `partidas_parcial.csv` para o Cenário 2, ou `partidas_vazio.csv` para o Cenário 1 ).

2.  **Compilar:** Abra um terminal no diretório do projeto e execute o `make`:
    ```bash
    make
    ```
    Isso irá compilar todos os arquivos `.c` e gerar um executável chamado `campeonato`.

3.  **Executar:** Inicie o programa:
    ```bash
    ./campeonato
    ```

4.  **Limpar:** Para remover os arquivos compilados (`.o` e o executável):
    ```bash
    make clean
    ```

## Estrutura do Repositório (TADs)

O código foi modularizado usando Tipos Abstratos de Dados (TADs) para separar responsabilidades [cite: 26, 202], conforme sugerido no documento do trabalho[cite: 103, 110, 114, 118].

* **`main.c`**: Contém o menu principal, o loop de execução e a lógica de orquestração (carregamento inicial, processamento de estatísticas e chamada das funcionalidades).
* **`time.h` / `time.c` (TAD Time)**:
    * Modela uma única equipe.
    * Armazena ID, Nome e as estatísticas acumuladas (V, E, D, GM, GS).
    * Fornece funções para calcular dados derivados (Pontos e Saldo de Gols)[cite: 105].
* **`partida.h` (TAD Partida)**:
    * Modela um único jogo.
    * Armazena os campos lidos do CSV: ID, IDs dos times e o placar[cite: 115].
* **`bdtimes.h` / `bdtimes.c` (TAD BDTimes)**:
    * Gerencia a coleção de todos os times.
    * Usa um **vetor estático** de 10 `Time` (conforme `NUM_TIMES 10` [cite: 11]).
    * Responsável por carregar `times.csv` [cite: 111] e processar os resultados das partidas para atualizar as estatísticas de cada time.
* **`bdpartidas.h` / `bdpartidas.c` (TAD BDPartidas)**:
    * Gerencia a coleção de todas as partidas.
    * Usa um **vetor estático** de `Partida` (com `MAX_PARTIDAS 100`), conforme a especificação[cite: 22].
    * Responsável por carregar `partidas.csv`.

## Principais Decisões de Implementação

1.  **Vetor Estático vs. Dinâmico**: Embora "Alocação dinâmica" [cite: 25] seja uma competência listada, a especificação da "Parte I" menciona explicitamente "um vetor estático de registros... com um tamanho pré-definido" [cite: 22] para as partidas. Segui essa diretriz. O `BDTimes` usa um vetor estático de 10 posições (tamanho fixo do campeonato [cite: 11]) e `BDPartidas` um vetor estático de 100 (para comportar os 90 jogos do cenário completo ). A alocação dinâmica foi usada apenas para criar as próprias estruturas dos TADs (ex: `bdtimes_criar`).
2.  **Cálculo de Estatísticas**: As estatísticas (V, E, D, etc.) são calculadas **uma única vez** na inicialização do programa. O `main.c`, após carregar times e partidas, itera por todas as partidas e chama `bdtimes_processar_partida` para cada uma. Isso acumula os valores no `BDTimes`.
3.  **Vantagem do Cálculo na Carga**: As funcionalidades 1 (Consultar Time) e 6 (Imprimir Tabela) tornam-se muito rápidas, pois apenas leem os valores já calculados, sem precisar reprocessar as partidas a cada consulta[cite: 124, 182].
4.  **Busca por Prefixo**: As buscas nas funções 1 e 2 usam `strncasecmp` (uma variação *case-insensitive* do `strncmp`) para implementar a busca por prefixo[cite: 125, 132], melhorando a usabilidade.
