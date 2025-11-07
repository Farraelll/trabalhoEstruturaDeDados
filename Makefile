# Makefile para o Trabalho de Programacao

# Compilador e flags
CC = gcc
# Flags: -Wall (todos os warnings), -g (debug symbols), -std=c99
# Removi -Wno-unused-function
CFLAGS = -Wall -g -std=c99

# Arquivos fonte (.c)
# Removido "partida.c" pois nao existe (apenas partida.h)
SRCS = main.c time.c bdtimes.c bdpartidas.c

# Arquivos objeto (derivados dos .c)
OBJS = $(SRCS:.c=.o)

# Nome do executavel final
TARGET = campeonato

# Regra padrao (o que 'make' faz)
all: $(TARGET)

# Regra para linkar o executavel final
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

# --- Regras de compilacao especificas ---
# Isso garante que ele recompile se um header que ele inclui for mudado.

main.o: main.c bdtimes.h bdpartidas.h
	$(CC) $(CFLAGS) -c main.c -o main.o

time.o: time.c time.h
	$(CC) $(CFLAGS) -c time.c -o time.o
	
bdtimes.o: bdtimes.c bdtimes.h time.h
	$(CC) $(CFLAGS) -c bdtimes.c -o bdtimes.o
	
bdpartidas.o: bdpartidas.c bdpartidas.h partida.h
	$(CC) $(CFLAGS) -c bdpartidas.c -o bdpartidas.o

# Regra 'clean' para limpar os arquivos compilados
clean:
	rm -f $(TARGET) $(OBJS)

# Regra 'run' para compilar e executar
run: all
	./$(TARGET)