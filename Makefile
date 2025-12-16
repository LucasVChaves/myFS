ifeq ($(OS),Windows_NT)
    # Windows
    CC = gcc
    RM = del /Q
    EXE = myfs.exe
    RMDIR = rmdir /S /Q
else
    # Unix/Linux
    CC = gcc
    RM = rm -f
    EXE = myfs
    RMDIR = rm -rf
endif

# Flags de compilação
CFLAGS = -Wall -Wextra -std=c99 -D_POSIX_C_SOURCE=199309L
LDFLAGS =

# Arquivos fonte e objeto
SOURCES = util.c disk.c inode.c vfs.c myfs.c main.c
HEADERS = util.h disk.h inode.h vfs.h myfs.h
OBJECTS = $(SOURCES:.c=.o)

# Alvo principal
all: $(EXE)

# Regra para criar o executável
$(EXE): $(OBJECTS)
	$(CC) $(OBJECTS) -o $(EXE) $(LDFLAGS)

# Regra para compilar arquivos .c em .o
%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

# Alvo de limpeza
clean:
ifeq ($(OS),Windows_NT)
	-$(RM) $(OBJECTS) 2>NUL
	-$(RM) $(EXE) 2>NUL
else
	$(RM) $(OBJECTS) $(EXE)
endif

# Alvo para recompilar tudo
rebuild: clean all

# Alvo para mostrar ajuda
help:
	@echo "Alvos disponíveis:"
	@echo "  all     - Compila o projeto (padrão)"
	@echo "  clean   - Remove arquivos de build"
	@echo "  rebuild - Limpa e recompila tudo"
	@echo "  help    - Mostra esta mensagem"

.PHONY: all clean rebuild help
