# Variables
CC = gcc  # Define el compilador a usar, en este caso gcc para C.
CFLAGS = $(shell pkg-config --cflags gtk+-3.0)  # Opciones de compilación para GTK+ 3.0.
LDFLAGS = $(shell pkg-config --libs gtk+-3.0)  # Enlace para las bibliotecas de GTK+ 3.0.
TARGET = exe  # Nombre del archivo ejecutable.
SOURCE = oldmaid.c  # Archivo fuente del proyecto.
GLADE = oldmaid.glade  # Archivo Glade del proyecto.

# Objetivo predeterminado
all: p1

# Compilación del proyecto
p1: $(SOURCE)
	$(CC) $(CFLAGS) -o $(TARGET) $(SOURCE) $(LDFLAGS)

# Limpiar archivos generados
clean:
	rm -f $(TARGET) *.o

# Declarar objetivos ficticios
.PHONY: all p1 clean